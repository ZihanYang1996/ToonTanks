// Fill out your copyright notice in the Description page of Project Settings.


#include "ToonTanksGameMode.h"

#include "Tank.h"
#include "TankPlayerController.h"
#include "Tower.h"
#include "Kismet/GameplayStatics.h"


void AToonTanksGameMode::BeginPlay()
{
	Super::BeginPlay();
	PlayerTank = Cast<ATank>(UGameplayStatics::GetPlayerPawn(this, 0));
	TankPlayerController = Cast<ATankPlayerController>(UGameplayStatics::GetPlayerController(this, 0));
}

void AToonTanksGameMode::ActorDie(AActor* DeadActor)
{
	if (DeadActor == PlayerTank)
	{
		PlayerTank->HandleDestruction();
		TankPlayerController->SetPlayerInputEnabled(false);
	}
	else if (ATower* DestroyedTower = Cast<ATower>(DeadActor))
	{
		DestroyedTower->HandleDestruction();
	}
}
