// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "BoxDestroyedWidget.generated.h"


UCLASS()
class MODULARFPS_API UBoxDestroyedWidget : public UUserWidget
{
	GENERATED_BODY()

public:

	UFUNCTION(BlueprintCallable)
	void ShowBoxDestructionMessage();
	
};
