// Fill out your copyright notice in the Description page of Project Settings.



#include "JsonScoreWidget.h"
#include "Components/TextBlock.h"

void UJsonScoreWidget::UpdateScore(int32 NewScore)
{
	ScoreText->SetText(FText::AsNumber(NewScore));
	UE_LOG(LogTemp, Warning, TEXT("Score updated to %d"), NewScore);
}
