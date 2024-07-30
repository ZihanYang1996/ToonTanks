// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "ToonTanksGameMode.generated.h"

class ATankPlayerController;
class ATank;
/**
 * 
 */

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnTankDeath);

UCLASS()
class TOONTANKS_API AToonTanksGameMode : public AGameModeBase
{
	GENERATED_BODY()
	ATank* PlayerTank;

	ATankPlayerController* TankPlayerController;

protected:
	virtual void BeginPlay() override;

	UFUNCTION(BlueprintImplementableEvent)
	void StartGame();

	UFUNCTION(BlueprintImplementableEvent)
	void GameOver(bool bWonGame);

public:
	void ActorDie(AActor* DeadActor);

	FOnTankDeath OnTankDeath;

private:
	void HandleGameStart();

	UPROPERTY(EditAnywhere, Category="Game Loop")
	float StartDelay = 3.0f;

	int32 TowerCount = 0;

	int32 GetTowerCount();
};
