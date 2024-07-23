// Fill out your copyright notice in the Description page of Project Settings.


#include "TankPlayerController.h"

#include "EnhancedInputSubsystems.h"

void ATankPlayerController::SetupInputComponent()
{
	SetPlayerInputEnabled(true);
}

void ATankPlayerController::SetPlayerInputEnabled(bool bEnabled)
{
	UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(
		GetLocalPlayer());
	if (!Subsystem)
	{
		return;
	}
	if (bEnabled)
	{
		// Show the mouse cursor
		bShowMouseCursor = true;
		// EnableInput(this);
		Subsystem->AddMappingContext(IMC_Tank, 0);
	}
	else
	{
		// Show the mouse cursor
		bShowMouseCursor = false;
		// DisableInput(this);
		Subsystem->RemoveMappingContext(IMC_Tank);
	}
}
