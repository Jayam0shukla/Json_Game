// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "MyData.h"
#include "GameFramework/Actor.h"
#include "JsonBoxActor.generated.h"

UCLASS()
class MODULARFPS_API AJsonBoxActor : public AActor
{
    GENERATED_BODY()

public:
    AJsonBoxActor();

protected:
   
    virtual void BeginPlay() override;

    
    UPROPERTY(VisibleAnywhere)
    UStaticMeshComponent* MeshComp;

    int32 CurrentHealth;
    int32 ScoreValue;

public:
    
    virtual void Tick(float DeltaTime) override;

    // Initializes actor using parsed data from JSON
    void Initialize(const FBoxType& BoxType, const FBoxObject& ObjData);

    // Applies damage and handles interaction with instigator
    void ReceiveDamage(int32 Amount, AActor* DamageInstigator);
};