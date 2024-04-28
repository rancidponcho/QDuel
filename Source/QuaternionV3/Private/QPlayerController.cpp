// Fill out your copyright notice in the Description page of Project Settings.


#include "QPlayerController.h"
#include "InputAction.h"
#include "InputActionValue.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "GameFramework/SpringArmComponent.h"
#include "QControlledActor.h"

AQPlayerController::AQPlayerController()
{
}

void AQPlayerController::BeginPlay()
{
	Super::BeginPlay();

	BindInputs();
	// SpawnPlayer(FVector(0.0f, 0.0f, 0.0f), FRotator(0.0f, 0.0f, 0.0f));
	SpawnShip(FVector(500.0f, 500.0f, 500.0f), FRotator(0.0f, 0.0f, 0.0f));
	Possessed = Ship;
	
	SetViewTargetWithBlend(Possessed, 0);	// use player camera
}

void AQPlayerController::SpawnShip(FVector Location, FRotator Rotation)
{
	Ship = GetWorld()->SpawnActor<AQControlledActor>(ShipClassBP, Location, Rotation);
}

void AQPlayerController::BindInputs()
{

	/* ====== SETUP INPUT ===== */ // TODO: move this shit 
	// Get a reference to the EnhancedInputComponent
	EnhancedInputComponent = Cast<UEnhancedInputComponent>(InputComponent);
	checkf(EnhancedInputComponent,
		TEXT("Unable to get reference to the EnhancedInputComponent."));

	// get local player subsystem
	UEnhancedInputLocalPlayerSubsystem* InputSubsystem =
		ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(GetLocalPlayer());
	checkf(InputSubsystem, TEXT("Unable to get reference to the EnhancedInputLocalPlayerSubsystem."));

	// Wipe existing mappings and add ours
	checkf(InputMappingContext, TEXT("InputMappingContext was not specified."));
	InputSubsystem->ClearAllMappings();
	InputSubsystem->AddMappingContext(InputMappingContext, 0);

	// BIND INPUTS
	if (ActionLook)
		EnhancedInputComponent->BindAction(ActionLook, ETriggerEvent::Triggered, this,
			&AQPlayerController::HandleLook);


	if (ActionPointAt)
		EnhancedInputComponent->BindAction(ActionPointAt, ETriggerEvent::Triggered, this,
			&AQPlayerController::HandlePointAt);

	if (ActionMove)
		EnhancedInputComponent->BindAction(ActionMove, ETriggerEvent::Triggered, this,
			&AQPlayerController::HandleMove);

	if (ActionRoll)
		EnhancedInputComponent->BindAction(ActionRoll, ETriggerEvent::Triggered, this,
			&AQPlayerController::HandleRoll);
}

void AQPlayerController::HandleLook(const FInputActionValue& InputActionValue)
{
	Possessed->Look(InputActionValue.Get<FVector2D>());
}

void AQPlayerController::HandlePointAt(const FInputActionValue& InputActionValue)
{
	Possessed->PointAt(InputActionValue.Get<bool>());
}

void AQPlayerController::HandleMove(const FInputActionValue& InputActionValue)
{
	Possessed->MoveXYZ(InputActionValue.Get<FVector>());
}

void AQPlayerController::HandleRoll(const FInputActionValue& InputActionValue)
{
	Possessed->Roll(InputActionValue.Get<float>());
}