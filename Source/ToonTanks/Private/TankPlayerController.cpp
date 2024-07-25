// Fill out your copyright notice in the Description page of Project Settings.


#include "TankPlayerController.h"

#include "EnhancedInputSubsystems.h"

void ATankPlayerController::SetupInputComponent()
{
	SetPlayerInputEnabled(true);
}

void ATankPlayerController::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);
	RotateController(DeltaSeconds);
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

void ATankPlayerController::RotateController(float dt)
{
	float MouseX;
	float MouseY;
	FVector2D ScreenSize;
	if (GetMousePosition(MouseX, MouseY))
	{
		GEngine->GameViewport->GetViewportSize(ScreenSize);
		if (MouseX < EdgeThreshold)
		{
			AddYawInput(-1.0f * RotationSpeed * dt);
			UpdateRotation(dt);
		}
		else if (MouseX > ScreenSize.X - EdgeThreshold)
		{
			AddYawInput(1.0f * RotationSpeed * dt);
			UpdateRotation(dt);
		}
	}
}
