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

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	virtual void HandleDestruction() override;

private:

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Components", meta=(AllowPrivateAccess = "true"))
	USpringArmComponent* SpringArm;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Components", meta=(AllowPrivateAccess = "true"))
	UCameraComponent* Camera;

	UPROPERTY(EditAnywhere, Category="Input")
	UInputAction* IA_Move;

	UPROPERTY(EditAnywhere, Category="Input")
	UInputAction* IA_Fire;

	UPROPERTY(EditAnywhere, Category="Input")
	UInputMappingContext* IMC_Tank;

	APlayerController* TankPlayerController;

	void Move(const FInputActionValue& Value);
	
	bool GetHitResultUnderCursor(FHitResult& OutHit);

public:
	UPROPERTY(EditAnywhere, Category="Movement")
	float MoveSpeed = 500.0f;

	UPROPERTY(EditAnywhere, Category="Movement")
	FVector2D RotationSpeed = FVector2D(100.0f, 100.0f);

	
};
