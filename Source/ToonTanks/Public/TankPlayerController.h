// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "TankPlayerController.generated.h"

class UInputMappingContext;
/**
 * 
 */
UCLASS()
class TOONTANKS_API ATankPlayerController : public APlayerController
{
	GENERATED_BODY()

public:
	virtual void SetupInputComponent() override;
	void SetPlayerInputEnabled(bool bEnabled);

protected:
	virtual void Tick(float DeltaSeconds) override;

private:
	UPROPERTY(EditAnywhere, Category="Input")
	UInputMappingContext* IMC_Tank;

	// UPROPERTY(EditAnywhere, Category="Input")
	// float EdgeThreshold = 100.0f;
	//
	// UPROPERTY(EditAnywhere, Category="Movement")
	// float RotationSpeed = 100.0f;
	//
	// void RotateController(float dt);
};
