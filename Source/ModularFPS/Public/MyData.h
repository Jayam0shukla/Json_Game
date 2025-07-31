#pragma once

#include "CoreMinimal.h"
#include "MyData.generated.h"

// Represents individual box types with properties like name, color, health, and score
USTRUCT()
struct FBoxType
{
    GENERATED_BODY()

    UPROPERTY() FString Name;
    UPROPERTY() FColor Color;
    UPROPERTY() int32 Health;
    UPROPERTY() int32 Score;

    FBoxType()
        : Name(TEXT(""))
        , Color(FColor::Black)
        , Health(0)
        , Score(0)
    {
    }
};

// Represents placed box objects with transform data and a reference to their type
USTRUCT()
struct FBoxObject
{
    GENERATED_BODY()

    UPROPERTY() FString Type;
    UPROPERTY() FVector Location;
    UPROPERTY() FRotator Rotation;
    UPROPERTY() FVector Scale;

    FBoxObject()
        : Type(TEXT(""))
        , Location(FVector::ZeroVector)
        , Rotation(FRotator::ZeroRotator)
        , Scale(FVector::OneVector)
    {
    }
};

// Container struct holding all box types and objects parsed from JSON
USTRUCT()
struct FMyData
{
    GENERATED_BODY()

    UPROPERTY() TArray<FBoxType> Types;
    UPROPERTY() TArray<FBoxObject> Objects;

    FMyData() {}
};