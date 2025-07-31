// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "JsonScoreWidget.generated.h"


UCLASS()
class MODULARFPS_API UJsonScoreWidget : public UUserWidget
{
	GENERATED_BODY()
public:
	UPROPERTY(meta = (BindWidget)) class UTextBlock* ScoreText;
	void UpdateScore(int32 NewScore);
	
};
