// Fill out your copyright notice in the Description page of Project Settings.


#include "Tank.h"

#include "Camera/CameraComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h" 
#include "Kismet/GameplayStatics.h"

ATank::ATank()
{
	// Use the controller rotation yaw
	// bUseControllerRotationYaw = true;
	
	SpringArm = CreateDefaultSubobject<USpringArmComponent>(TEXT("Spring Arm"));
	SpringArm->SetupAttachment(RootComponent);
	SpringArm->bUsePawnControlRotation = true;
	SpringArm->bEnableCameraLag = true;

	Camera = CreateDefaultSubobject<UCameraComponent>(TEXT("Camera"));
	Camera->SetupAttachment(SpringArm, USpringArmComponent::SocketName);
	Camera->bUsePawnControlRotation = false;
}

void ATank::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	// Add Input Mapping Context
	if (APlayerController* PlayerController = Cast<APlayerController>(GetController()))
	{
		if (UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(PlayerController->GetLocalPlayer()))
		{
			Subsystem->AddMappingContext(IMC_Tank, 0);
		}
	}

	// Set up input actions
	if (UEnhancedInputComponent* EnhancedInputComponent = Cast<UEnhancedInputComponent>(PlayerInputComponent))
	{
		EnhancedInputComponent->BindAction(IA_Move, ETriggerEvent::Triggered, this, &ATank::Move);
		EnhancedInputComponent->BindAction(IA_Look, ETriggerEvent::Triggered, this, &ATank::Look);
	}
}

void ATank::Move(const FInputActionValue& Value)
{
	// Print on screen the value of the input action
	// if (GEngine)
	// {
	// 	GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, FString::Printf(TEXT("Move: %s"), *Value.ToString()));
	// }
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

	// Set the rotation of the tank to the movement directions
	FRotator TargetRotation = FVector(ForwardVector * MovementVector.Y + RightVector * MovementVector.X).Rotation();
	FRotator NewRotation = FMath::RInterpConstantTo(GetActorRotation(), TargetRotation, GetWorld()->GetDeltaSeconds(), RotationSpeed.X * 10);  // Multiply by 10 to make it faster
	SetActorRotation(NewRotation);

	
	/*
	// Only if the actor using the controller rotation yaw
	FVector ForwardVector = GetActorForwardVector();
	FVector RightVector = GetActorRightVector();
	*/
	
	FVector DeltaLocation = ForwardVector * MovementVector.Y * MoveSpeed * GetWorld()->GetDeltaSeconds() + RightVector * MovementVector.X * MoveSpeed * GetWorld()->GetDeltaSeconds();
	AddActorWorldOffset(DeltaLocation);
	
}

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


