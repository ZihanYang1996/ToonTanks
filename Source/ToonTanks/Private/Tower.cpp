// Fill out your copyright notice in the Description page of Project Settings.


#include "Tower.h"

#include "Tank.h"
#include "ToonTanksGameMode.h"
#include "Kismet/GameplayStatics.h"

void ATower::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	if (InFiringRange())
	{
		RotateTurret(Tank->GetActorLocation());
	}
	
}

void ATower::BeginPlay()
{
	Super::BeginPlay();
	Tank = Cast<ATank>(UGameplayStatics::GetPlayerPawn(this, 0));

	GetWorld()->GetTimerManager().SetTimer(FireRateTimerHandle, this, &ATower::CheckFireCondition, FireRate, true, -1);

	AToonTanksGameMode* ToonTanksGameMode = Cast<AToonTanksGameMode>(UGameplayStatics::GetGameMode(this));
	if (ToonTanksGameMode)
	{
		// Bind a lambda function to the OnTankDeath event to change bIsTankAlive to false
		ToonTanksGameMode->OnTankDeath.AddDynamic(this, &ATower::TankDied);
		
	}
}

void ATower::CheckFireCondition()
{
	if (InFiringRange() && bIsTankAlive)
	{
		Fire();
	}
}

bool ATower::InFiringRange()
{
	if (Tank)
	{
		float Distance = FVector::Dist(Tank->GetActorLocation(), GetActorLocation());
		return Distance <= FireRange;
	}
	return false;
}

void ATower::HandleDestruction()
{
	Super::HandleDestruction();
	Destroy();
}

void ATower::TankDied()
{
	bIsTankAlive = false;
}