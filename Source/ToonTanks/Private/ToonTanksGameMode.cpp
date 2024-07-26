// Fill out your copyright notice in the Description page of Project Settings.


#include "ToonTanksGameMode.h"

#include "Tank.h"
#include "TankPlayerController.h"
#include "Tower.h"
#include "Kismet/GameplayStatics.h"


void AToonTanksGameMode::BeginPlay()
{
	Super::BeginPlay();
	HandleGameStart();
	TowerCount = GetTowerCount();
}

void AToonTanksGameMode::ActorDie(AActor* DeadActor)
{
	if (DeadActor == PlayerTank)
	{
		PlayerTank->HandleDestruction();
		TankPlayerController->SetPlayerInputEnabled(false);
		GameOver(false);
	}
	else if (ATower* DestroyedTower = Cast<ATower>(DeadActor))
	{
		DestroyedTower->HandleDestruction();
		TowerCount--;
		if (TowerCount == 0)
		{
			GameOver(true);
		}
	}
}

void AToonTanksGameMode::HandleGameStart()
{
	PlayerTank = Cast<ATank>(UGameplayStatics::GetPlayerPawn(this, 0));
	TankPlayerController = Cast<ATankPlayerController>(UGameplayStatics::GetPlayerController(this, 0));

	StartGame();  // Call Blueprint Event
	
	if (TankPlayerController)
	{
		TankPlayerController->SetPlayerInputEnabled(false);

		FTimerHandle PlayerEnableTimerHandle;
		FTimerDelegate PlayerEnableTimerDelegate = FTimerDelegate::CreateUObject(
			TankPlayerController, &ATankPlayerController::SetPlayerInputEnabled, true);
		GetWorld()->GetTimerManager().SetTimer(PlayerEnableTimerHandle, PlayerEnableTimerDelegate, StartDelay, false);
	}
}

int32 AToonTanksGameMode::GetTowerCount()
{
	TArray<AActor*> Towers;
	UGameplayStatics::GetAllActorsOfClass(GetWorld(), ATower::StaticClass(), Towers);
	return Towers.Num();
}