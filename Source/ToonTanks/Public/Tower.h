// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BasePawn.h"
#include "Tower.generated.h"

/**
 * 
 */
class ATank;
struct FTimerHandle;
UCLASS()
class TOONTANKS_API ATower : public ABasePawn
{
	GENERATED_BODY()

public:
	virtual void Tick(float DeltaTime) override;

protected:
	virtual void BeginPlay() override;

	virtual void HandleDestruction() override;

private:
	ATank* Tank;

	FTimerHandle FireRateTimerHandle;

	UPROPERTY(EditAnywhere, Category="Turret")
	float FireRate = 2.0f;

	UPROPERTY(EditAnywhere, Category="Turret")
	float FireRange = 500.0f;

	bool InFiringRange();

	void CheckFireCondition();
};
