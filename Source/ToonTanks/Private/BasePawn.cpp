// Fill out your copyright notice in the Description page of Project Settings.


#include "BasePawn.h"

#include "Projectile.h"
#include "Components/CapsuleComponent.h"
#include "Components/StaticMeshComponent.h"

// Sets default values
ABasePawn::ABasePawn()
{
 	// Set this pawn to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	// Create a capsule mesh component and set it as the root component
	CapsuleComponent = CreateDefaultSubobject<UCapsuleComponent>(TEXT("Capsule Collider"));
	RootComponent = CapsuleComponent;

	// Create a static mesh component and attach it to the root component
	BaseMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Base Mesh"));
	BaseMesh->SetupAttachment(RootComponent);

	// Create another static mesh component and attach it to previous static mesh component
	TurretMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Turret Mesh"));
	TurretMesh->SetupAttachment(BaseMesh);

	// Create a scene component for projectile spawn point
	ProjectileSpawnPoint = CreateDefaultSubobject<USceneComponent>(TEXT("Projectile Spawn Point"));
	ProjectileSpawnPoint->SetupAttachment(TurretMesh);

}

// Called when the game starts or when spawned
void ABasePawn::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void ABasePawn::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

// Called to bind functionality to input
void ABasePawn::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

}

void ABasePawn::RotateTurret(const FVector& LookAtTarget)
{
	FVector TurretLocation = TurretMesh->GetComponentLocation();
	FRotator TargetDirection = FVector(LookAtTarget - TurretLocation).Rotation();
	TargetDirection.Pitch = 0.0f;
	TargetDirection.Roll = 0.0f;
	FRotator newDirection = FMath::RInterpConstantTo(TurretMesh->GetComponentRotation(), TargetDirection, GetWorld()->DeltaTimeSeconds, TurretRotationSpeed);
	
	TurretMesh->SetWorldRotation(newDirection);
}

void ABasePawn::Fire()
{
	// DrawDebugSphere(GetWorld(), ProjectileSpawnPoint->GetComponentLocation(), 25.0f, 12, FColor::Red, false, 1.0f);
	FVector Location = ProjectileSpawnPoint->GetComponentLocation();
	FRotator Rotation = ProjectileSpawnPoint->GetComponentRotation();
	auto Projectile = GetWorld()->SpawnActor<AProjectile>(ProjectileClass, Location, Rotation);
	Projectile->SetOwner(this);
	Projectile->SetDamage(Damage);
}
	