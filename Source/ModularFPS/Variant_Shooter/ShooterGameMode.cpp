// Copyright Epic Games, Inc. All Rights Reserved.


#include "Variant_Shooter/ShooterGameMode.h"
#include "ShooterUI.h"
#include "Kismet/GameplayStatics.h"
#include "Engine/World.h"
#include "JsonDataManager.h"
#include "Variant_Shooter/ShooterPlayerController.h"

void AShooterGameMode::BeginPlay()
{
	Super::BeginPlay();

	// create the UI
	ShooterUI = CreateWidget<UShooterUI>(UGameplayStatics::GetPlayerController(GetWorld(), 0), ShooterUIClass);
	ShooterUI->AddToViewport(0);
	UJsonDataManager* JsonDataManager = NewObject<UJsonDataManager>(this);
	JsonDataManager->FetchData();
}

void AShooterGameMode::IncrementTeamScore(uint8 TeamByte)
{
	// retrieve the team score if any
	int32 Score = 0;
	if (int32* FoundScore = TeamScores.Find(TeamByte))
	{
		Score = *FoundScore;
	}

	// increment the score for the given team
	TeamScores.Add(TeamByte, Score + 1);

	// update the UI
	ShooterUI->BP_UpdateScore(TeamByte, Score);
}

void AShooterGameMode::AddScore(int32 Amount)
{
	CurrentScore += Amount;

	AShooterPlayerController* PC = Cast<AShooterPlayerController>(GetWorld()->GetFirstPlayerController());
	if (PC && PC->ScoreWidget)
	{
		PC->ScoreWidget->UpdateScore(CurrentScore);
	}
}
