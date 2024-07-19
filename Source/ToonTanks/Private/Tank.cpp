// Fill out your copyright notice in the Description page of Project Settings.


#include "Tank.h"

#include "Camera/CameraComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "Kismet/GameplayStatics.h"

ATank::ATank()
{
	PrimaryActorTick.bCanEverTick = true;

	// Use the controller rotation yaw
	bUseControllerRotationYaw = true;

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
	PlayerControllerRef = Cast<APlayerController>(GetController());
	// Show the mouse cursor
	PlayerControllerRef->bShowMouseCursor = true;
}

void ATank::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	// Add Input Mapping Context
	if (APlayerController* PlayerController = Cast<APlayerController>(GetController()))
	{
		if (UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<
			UEnhancedInputLocalPlayerSubsystem>(PlayerController->GetLocalPlayer()))
		{
			Subsystem->AddMappingContext(IMC_Tank, 0);
		}
	}

	// Set up input actions
	if (UEnhancedInputComponent* EnhancedInputComponent = Cast<UEnhancedInputComponent>(PlayerInputComponent))
	{
		EnhancedInputComponent->BindAction(IA_Move, ETriggerEvent::Triggered, this, &ATank::Move);
		// EnhancedInputComponent->BindAction(IA_Look, ETriggerEvent::Triggered, this, &ATank::Look);
	}
}

void ATank::Tick(float DeltaSeconds)
{
	GetHitResultUnderCursor();
}


void ATank::Move(const FInputActionValue& Value)
{
	// Get the movement vector from the input action value
	FVector2D MovementVector = Value.Get<FVector2D>();

	/* No need to manually calculate the forward and right vectors since we are using the controller rotation yaw
	// Get yaw rotation from the controller
	FRotator ControllerRotationYaw(0, GetControlRotation().Yaw, 0);

	// Only if the actor not using the controller rotation yaw
	// Convert the Rotator to Rotation Matrix
	FRotationMatrix RotationMatrixYaw(ControllerRotationYaw);

	// Get the forward vector and right vector from the Rotation Matrix
	FVector ForwardVector = RotationMatrixYaw.GetUnitAxis(EAxis::X);
	FVector RightVector = RotationMatrixYaw.GetUnitAxis(EAxis::Y);
	*/

	// Get the forward and right vectors from the actor
	// Only if the actor using the controller rotation yaw
	FVector ForwardVector = GetActorForwardVector();
	FVector RightVector = GetActorRightVector();

	// Only move the actor if the movement vector is not zero, so that the actor can rotate in place (around the Z axis)
	// May or may not keep this behavior depending on the gameplay I want to achieve
	// Keep it for now
	if (MovementVector.Y > 0)
	{
		FVector DeltaLocation = ForwardVector * MovementVector.Y * MoveSpeed * GetWorld()->GetDeltaSeconds() + RightVector *
			MovementVector.X * MoveSpeed * GetWorld()->GetDeltaSeconds();
		AddActorWorldOffset(DeltaLocation, true);
		
		// Set the controller yaw input based on the movement vector's X value (A/D or Left/Right)
		AddControllerYawInput(MovementVector.X * RotationSpeed.X * GetWorld()->GetDeltaSeconds());
	}
	else if (MovementVector.Y < 0)
	{
		FVector DeltaLocation = ForwardVector * MovementVector.Y * MoveSpeed * GetWorld()->GetDeltaSeconds() + RightVector *
			MovementVector.X * MoveSpeed * GetWorld()->GetDeltaSeconds();
		AddActorWorldOffset(DeltaLocation, true);
		
		// Set the controller yaw input based on the movement vector's X value (A/D or Left/Right)
		// When moving backwards, the rotation should be reversed, to simulate the tank turning around
		AddControllerYawInput(-MovementVector.X * RotationSpeed.X * GetWorld()->GetDeltaSeconds());		
	}
}

// Not used, at least for now
void ATank::Look(const FInputActionValue& Value)
{
	// Print on screen the value of the input action
	// if (GEngine)
	// {
	// 	GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, FString::Printf(TEXT("Look: %s"), *Value.ToString()));
	// }

	FVector2D LookVector = Value.Get<FVector2D>();
	AddControllerYawInput(LookVector.X * RotationSpeed.X * GetWorld()->GetDeltaSeconds());
	AddControllerPitchInput(LookVector.Y * RotationSpeed.Y * GetWorld()->GetDeltaSeconds());
}

void ATank::GetHitResultUnderCursor()
{
	FHitResult OutHit;
	PlayerControllerRef->GetHitResultUnderCursor(ECollisionChannel::ECC_Visibility, false, OutHit);
	DrawDebugSphere(GetWorld(), OutHit.ImpactPoint, 25.0f, 12, FColor::Red, false, 1.0f);
}
