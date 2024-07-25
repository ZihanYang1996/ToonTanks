// Fill out your copyright notice in the Description page of Project Settings.


#include "Tank.h"

#include "Camera/CameraComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"

ATank::ATank()
{
	PrimaryActorTick.bCanEverTick = true;

	// Use the controller rotation yaw
	// bUseControllerRotationYaw = true;

	SpringArm = CreateDefaultSubobject<USpringArmComponent>(TEXT("Spring Arm"));
	SpringArm->SetupAttachment(RootComponent);
	SpringArm->bUsePawnControlRotation = false;
	SpringArm->bEnableCameraLag = true;

	Camera = CreateDefaultSubobject<UCameraComponent>(TEXT("Camera"));
	Camera->SetupAttachment(SpringArm, USpringArmComponent::SocketName);
	Camera->bUsePawnControlRotation = false;
}

void ATank::BeginPlay()
{
	Super::BeginPlay();

	// Set the player controller to the tank
	TankPlayerController = Cast<APlayerController>(GetController());

	DefaultSpringArmRotation = SpringArm->GetComponentRotation();
}

void ATank::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	// Set up input actions
	if (UEnhancedInputComponent* EnhancedInputComponent = Cast<UEnhancedInputComponent>(PlayerInputComponent))
	{
		EnhancedInputComponent->BindAction(IA_Move, ETriggerEvent::Triggered, this, &ATank::Move);
		EnhancedInputComponent->BindAction(IA_Fire, ETriggerEvent::Started, this, &ATank::Fire);
	}
}

void ATank::Tick(float DeltaSeconds)
{
	FHitResult OutHit;
	if (GetHitResultUnderCursor(OutHit))
	{
		RotateTurret(OutHit.ImpactPoint);
	}
	RotateController(DeltaSeconds);
	UpdateSpringArmYaw();
}


void ATank::Move(const FInputActionValue& Value)
{
	// Get the movement vector from the input action value
	FVector2D MovementVector = Value.Get<FVector2D>();

	// Get yaw rotation from the controller
	FRotator ControllerRotationYaw(0, GetControlRotation().Yaw, 0);

	// Only if the actor not using the controller rotation yaw
	// Convert the Rotator to Rotation Matrix
	FRotationMatrix RotationMatrixYaw(ControllerRotationYaw);

	// Get the forward vector and right vector from the Rotation Matrix
	FVector ForwardVector = RotationMatrixYaw.GetUnitAxis(EAxis::X);
	FVector RightVector = RotationMatrixYaw.GetUnitAxis(EAxis::Y);
	
	
	FVector DeltaLocation = ForwardVector * MovementVector.Y * MoveSpeed * GetWorld()->GetDeltaSeconds() +
		RightVector * MovementVector.X * MoveSpeed * GetWorld()->GetDeltaSeconds();
	AddActorWorldOffset(DeltaLocation, true);

	// Calculate and set rotation
	FRotator NewRotation = FMath::RInterpTo(GetActorRotation(), DeltaLocation.Rotation(), GetWorld()->GetDeltaSeconds(), TankRotationSpeed);
	SetActorRotation(NewRotation);
}


bool ATank::GetHitResultUnderCursor(FHitResult& OutHit)
{
	return TankPlayerController->GetHitResultUnderCursor(ECollisionChannel::ECC_Visibility, false, OutHit);
}

void ATank::HandleDestruction()
{
	Super::HandleDestruction();
	GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Red, TEXT("Tank Destroyed!"));
	SetActorHiddenInGame(true);
	SetActorTickEnabled(false);
}

void ATank::UpdateSpringArmYaw()
{
	SpringArm->SetWorldRotation(FRotator(DefaultSpringArmRotation.Pitch, GetControlRotation().Yaw, 0));
}

void ATank::RotateController(float dt)
{
	float MouseX;
	float MouseY;
	FVector2D ScreenSize;
	if (TankPlayerController->GetMousePosition(MouseX, MouseY))
	{
		GEngine->GameViewport->GetViewportSize(ScreenSize);
		if (MouseX < ScreenEdgeThreshold)
		{
			AddControllerYawInput(-1.0f * CameraRotationSpeed * dt);
		}
		else if (MouseX > ScreenSize.X - ScreenEdgeThreshold)
		{
			AddControllerYawInput(1.0f * CameraRotationSpeed * dt);
		}
	}
}