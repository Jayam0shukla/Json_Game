// Copyright Epic Games, Inc. All Rights Reserved.


#include "ModularFPSPlayerController.h"
#include "EnhancedInputSubsystems.h"
#include "Engine/LocalPlayer.h"
#include "InputMappingContext.h"
#include "ModularFPSCameraManager.h"

AModularFPSPlayerController::AModularFPSPlayerController()
{
	// set the player camera manager class
	PlayerCameraManagerClass = AModularFPSCameraManager::StaticClass();
}

void AModularFPSPlayerController::SetupInputComponent()
{
	Super::SetupInputComponent();

	// Add Input Mapping Context
	if (UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(GetLocalPlayer()))
	{
		for (UInputMappingContext* CurrentContext : DefaultMappingContexts)
		{
			Subsystem->AddMappingContext(CurrentContext, 0);
		}
	}
}
