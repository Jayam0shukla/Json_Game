#include "JsonDataManager.h"
#include "HttpModule.h"
#include "Json.h"
#include "JsonUtilities.h"
#include "JsonBoxActor.h"
#include "Kismet/GameplayStatics.h"
#include "Interfaces/IHttpRequest.h"
#include "Interfaces/IHttpResponse.h"

// Parses an array of numbers into an FColor (expects 3 or 4 values)
FColor ParseColorArray(const TArray<TSharedPtr<FJsonValue>>& ColorArray)
{
    if (ColorArray.Num() >= 3)
    {
        uint8 R = ColorArray[0]->AsNumber();
        uint8 G = ColorArray[1]->AsNumber();
        uint8 B = ColorArray[2]->AsNumber();
        uint8 A = (ColorArray.Num() >= 4) ? ColorArray[3]->AsNumber() : 255;

        return FColor(R, G, B, A);
    }
    return FColor::Black;
}

// Parses an array into a FVector (expects 3 values)
FVector ParseVectorArray(const TArray<TSharedPtr<FJsonValue>>& Array)
{
    return (Array.Num() >= 3)
        ? FVector(Array[0]->AsNumber(), Array[1]->AsNumber(), Array[2]->AsNumber())
        : FVector::ZeroVector;
}

// Parses an array into a FRotator (expects 3 values)
FRotator ParseRotatorArray(const TArray<TSharedPtr<FJsonValue>>& Array)
{
    return (Array.Num() >= 3)
        ? FRotator(Array[0]->AsNumber(), Array[1]->AsNumber(), Array[2]->AsNumber())
        : FRotator::ZeroRotator;
}

void UJsonDataManager::FetchData()
{
    const FString URL = TEXT("https://raw.githubusercontent.com/CyrusCHAU/Varadise-Technical-Test/refs/heads/main/data.json");

    TSharedRef<IHttpRequest> Request = FHttpModule::Get().CreateRequest();
    Request->SetURL(URL);
    Request->SetVerb("GET");
    Request->SetHeader(TEXT("Content-Type"), TEXT("application/json"));
    Request->OnProcessRequestComplete().BindUObject(this, &UJsonDataManager::OnResponseReceived);
    Request->ProcessRequest();
}

void UJsonDataManager::OnResponseReceived(FHttpRequestPtr Request, FHttpResponsePtr Response, bool bWasSuccessful)
{
    if (!bWasSuccessful || !Response.IsValid())
    {
        UE_LOG(LogTemp, Error, TEXT("HTTP request failed"));
        return;
    }

    FString JsonString = Response->GetContentAsString();
    UE_LOG(LogTemp, Warning, TEXT("Received JSON: %s"), *JsonString);

    TSharedPtr<FJsonObject> RootObject;
    TSharedRef<TJsonReader<>> Reader = TJsonReaderFactory<>::Create(JsonString);
    FMyData ParsedData;

    if (!FJsonSerializer::Deserialize(Reader, RootObject) || !RootObject.IsValid())
    {
        UE_LOG(LogTemp, Error, TEXT("Failed to deserialize JSON object"));
        return;
    }

    // Parse box types
    for (const auto& TypeValue : RootObject->GetArrayField(TEXT("types")))
    {
        TSharedPtr<FJsonObject> TypeObj = TypeValue->AsObject();
        FBoxType BoxType;

        BoxType.Name = TypeObj->GetStringField(TEXT("name"));
        BoxType.Health = TypeObj->GetIntegerField(TEXT("health"));
        BoxType.Score = TypeObj->GetIntegerField(TEXT("score"));
        BoxType.Color = ParseColorArray(TypeObj->GetArrayField(TEXT("color")));

        ParsedData.Types.Add(BoxType);
    }

    // Parse box objects
    for (const auto& ObjValue : RootObject->GetArrayField(TEXT("objects")))
    {
        TSharedPtr<FJsonObject> Obj = ObjValue->AsObject();
        FBoxObject BoxObj;

        BoxObj.Type = Obj->GetStringField(TEXT("type"));

        const TSharedPtr<FJsonObject>& TransformObj = Obj->GetObjectField(TEXT("transform"));
        BoxObj.Location = ParseVectorArray(TransformObj->GetArrayField(TEXT("location")));
        BoxObj.Rotation = ParseRotatorArray(TransformObj->GetArrayField(TEXT("rotation")));
        BoxObj.Scale = ParseVectorArray(TransformObj->GetArrayField(TEXT("scale")));

        ParsedData.Objects.Add(BoxObj);
    }

    // Spawn actors
    for (const auto& Obj : ParsedData.Objects)
    {
        const FBoxType* BoxType = ParsedData.Types.FindByPredicate([&](const FBoxType& T) {
            return T.Name == Obj.Type;
            });

        if (!BoxType) continue;

        AJsonBoxActor* Box = GetWorld()->SpawnActor<AJsonBoxActor>(AJsonBoxActor::StaticClass());
        if (!Box)
        {
            UE_LOG(LogTemp, Error, TEXT("Failed to spawn JsonBoxActor"));
            continue;
        }

        Box->Initialize(*BoxType, Obj);
    }

    UE_LOG(LogTemp, Warning, TEXT("Box spawning complete."));
}