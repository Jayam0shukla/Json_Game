// Fill out your copyright notice in the Description page of Project Settings.


#include "JsonBoxActor.h"
#include "Variant_Shooter/ShooterGameMode.h"
#include "Components/StaticMeshComponent.h"
#include "Materials/MaterialInstanceDynamic.h"
#include "Components/WidgetComponent.h"
#include "Variant_Shooter/ShooterPlayerController.h"
#include "Kismet/GameplayStatics.h"
#include "BoxDestroyedWidget.h"

// Sets default values
AJsonBoxActor::AJsonBoxActor()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	MeshComp = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Mesh"));
	RootComponent = MeshComp;
	static ConstructorHelpers::FObjectFinder<UStaticMesh> BoxMesh(TEXT("/Engine/BasicShapes/Cube.Cube"));
	if (BoxMesh.Succeeded()) {
		MeshComp->SetStaticMesh(BoxMesh.Object);
	}
	else {
		UE_LOG(LogTemp, Error, TEXT("Failed to find Cube mesh"));
	}
	if (!MeshComp) {
		UE_LOG(LogTemp, Error, TEXT("MeshComp is null in Initialize()"));
		return;
	}
}

// Called when the game starts or when spawned
void AJsonBoxActor::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AJsonBoxActor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void AJsonBoxActor::Initialize(const FBoxType& BoxType, const FBoxObject& ObjData)
{
	UE_LOG(LogTemp, Warning, TEXT("Setting color: R=%d G=%d B=%d A=%d"),
		BoxType.Color.R, BoxType.Color.G, BoxType.Color.B, BoxType.Color.A);
	if (!MeshComp) {
		UE_LOG(LogTemp, Error, TEXT("MeshComp is null in Initialize"));
		return;
	}
	CurrentHealth = BoxType.Health;
	ScoreValue = BoxType.Score;
	SetActorLocation(ObjData.Location);
	SetActorRotation(ObjData.Rotation);
	SetActorScale3D(ObjData.Scale);

	
	UMaterialInstanceDynamic* DynMat = MeshComp->CreateAndSetMaterialInstanceDynamic(0);
	if (!DynMat) {
		UE_LOG(LogTemp, Error, TEXT("Failed to create material instance"));
		return;
	}
	DynMat->SetVectorParameterValue("BaseColor", BoxType.Color.ReinterpretAsLinear());

}

void AJsonBoxActor::ReceiveDamage(int32 Amount, AActor* DamageInstigator)
{
	CurrentHealth -= Amount;
	UE_LOG(LogTemp, Warning, TEXT("Box took damage: %d"), Amount);
	if (CurrentHealth <= 0) {
		UE_LOG(LogTemp, Warning, TEXT("Box should be destroyed"));
		if (auto* GM = Cast<AShooterGameMode>(GetWorld()->GetAuthGameMode()))
		{
			GM->AddScore(ScoreValue);
		}
		if (auto* PC = Cast<AShooterPlayerController>(UGameplayStatics::GetPlayerController(this, 0))) {
			if (PC->BoxDestroyedWidget) {
				PC->BoxDestroyedWidget->ShowBoxDestructionMessage();
			}
		}
		Destroy();
	}
}

