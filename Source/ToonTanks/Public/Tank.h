// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BasePawn.h"
#include "Tank.generated.h"

/**
 * 
 */
class USpringArmComponent;
class UCameraComponent;
class UInputAction;
class UInputMappingContext;
struct FInputActionValue;

UCLASS()
class TOONTANKS_API ATank : public ABasePawn
{
	GENERATED_BODY()

public:
	ATank();

	virtual void SetupPlayerInputComponent(UInputComponent* PlayerInputComponent) override;

	virtual void Tick(float DeltaSeconds) override;

	virtual void HandleDestruction() override;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

private:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Components", meta=(AllowPrivateAccess = "true"))
	USpringArmComponent* SpringArm;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Components", meta=(AllowPrivateAccess = "true"))
	UCameraComponent* Camera;

	UPROPERTY(EditAnywhere, Category="Input")
	UInputAction* IA_Move;

	UPROPERTY(EditAnywhere, Category="Input")
	UInputAction* IA_Fire;


	APlayerController* TankPlayerController;

	void Move(const FInputActionValue& Value);

	bool GetHitResultUnderCursor(FHitResult& OutHit);

	void UpdateSpringArmYaw();

	FRotator DefaultSpringArmRotation;

public:
	UPROPERTY(EditAnywhere, Category="Movement")
	float MoveSpeed = 500.0f;

	UPROPERTY(EditAnywhere, Category="Movement")
	float TankRotationSpeed = 10.0f;

	UPROPERTY(EditAnywhere, Category="Input")
	float ScreenEdgeThreshold = 100.0f;

	UPROPERTY(EditAnywhere, Category="Movement")
	float CameraRotationSpeed = 100.0f;

	void RotateController(float dt);
};
