// Fill out your copyright notice in the Description page of Project Settings.


#include "Projectile.h"

#include "GameFramework/ProjectileMovementComponent.h"
#include "Kismet/GameplayStatics.h"

// Sets default values
AProjectile::AProjectile()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

	Mesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Projectile Mesh"));
	RootComponent = Mesh;


	ProjectileMovement = CreateDefaultSubobject<UProjectileMovementComponent>(TEXT("Projectile Movement"));
	ProjectileMovement->InitialSpeed = 1000.0f;
	ProjectileMovement->MaxSpeed = 2000.0f;
}

// Called when the game starts or when spawned
void AProjectile::BeginPlay()
{
	Super::BeginPlay();

	Mesh->OnComponentHit.AddDynamic(this, &AProjectile::OnHit);
}

// Called every frame
void AProjectile::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void AProjectile::OnHit(UPrimitiveComponent* HitComp, AActor* OtherActor, UPrimitiveComponent* OtherComp,
                        FVector NormalImpulse, const FHitResult& Hit)
{
	GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red,
	                                 FString::Printf(TEXT("Hit: %s"), *OtherActor->GetActorLabel()));
	// if (AController* InstigatorController = GetOwner()->GetInstigatorController())
	// {
	// 	FString ControllerName = InstigatorController->GetName();
	// 	GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, FString::Printf(TEXT("InstigatorController: %s"), *ControllerName));
	// }
	// else
	// {
	// 	GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, TEXT("InstigatorController: nullptr"));
	// }

	if (OtherActor && OtherActor != this && OtherActor != GetOwner() && OtherComp)
	{
		if (AController* InstigatorController = GetOwner()->GetInstigatorController())
		{
			auto DameTypeClass = UDamageType::StaticClass();
			UGameplayStatics::ApplyDamage(OtherActor, Damage, InstigatorController, this, DameTypeClass);

			// Two ways to spawn particle effect, one is to spawn at location, the other is to spawn attached to a component	
			// UGameplayStatics::SpawnEmitterAtLocation(this, HitParticle, GetActorLocation(), GetActorRotation());
			UParticleSystemComponent* SpawnedEmitter = UGameplayStatics::SpawnEmitterAttached(HitParticle, OtherComp, NAME_None, Hit.ImpactPoint, FRotator::ZeroRotator,
			                                      EAttachLocation::KeepWorldPosition, true);
			GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, FString::Printf(TEXT("SpawnedEmitter is attached to: %s"), *OtherComp->GetOwner()->GetActorNameOrLabel()));
		}
	}
	Destroy();
}

void AProjectile::SetDamage(float DamageToSet)
{
	this->Damage = DamageToSet;
}
