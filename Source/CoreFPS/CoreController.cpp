// Fill out your copyright notice in the Description page of Project Settings.


#include "CoreController.h"
#include "CoreUserSettings.h"

#include "UObject/ConstructorHelpers.h"

#include "InputActionValue.h"
#include "InputMappingContext.h"
#include "EnhancedInputComponent.h"

ACoreController::ACoreController() {
	static ConstructorHelpers::FObjectFinder<UInputMappingContext> IMCClassFinder(
		TEXT("/Game/Inputs/IMC_Default.IMC_Default"));
	static ConstructorHelpers::FObjectFinder<UInputAction> MCamAClassFinder(
		TEXT("/Game/Inputs/IA_MoveCam.IA_MoveCam"));
	static ConstructorHelpers::FObjectFinder<UInputAction> MChrAClassFinder(
		TEXT("/Game/Inputs/IA_MoveChr.IA_MoveChr"));

	DefaultMappingContext = IMCClassFinder.Object;
	MoveCamAction = MCamAClassFinder.Object;
	MoveChrAction = MChrAClassFinder.Object;
}

void ACoreController::BeginPlay() {
	Super::BeginPlay();

	/*
	Sensitivity = GetSensitivity();
	SensitivityYaw = GetSensitivityYaw();
	SensitivityPitch = GetSensitivityPitch();
	*/
}

void ACoreController::Tick(float DeltaTime) {
	Super::Tick(DeltaTime);
}

void ACoreController::SetupInputComponent() {
	Super::SetupInputComponent(); 
	if (UEnhancedInputComponent* EIC = Cast<UEnhancedInputComponent>(InputComponent)) {
		EIC->BindAction(MoveCamAction, ETriggerEvent::Triggered, this, &ACoreController::MoveCam);
		EIC->BindAction(MoveChrAction, ETriggerEvent::Triggered, this, &ACoreController::MoveChr);
	}
}

void ACoreController::MoveChr(const FInputActionValue& Value) {
	FVector2D MoveVec = Value.Get<FVector2D>();

	GetPawn()->AddMovementInput(GetPawn()->GetActorRightVector(), MoveVec.X);
	GetPawn()->AddMovementInput(GetPawn()->GetActorForwardVector(), MoveVec.Y);
}

float ACoreController::GetSensitivity() {
	return UCoreUserSettings::GetCoreUserSettings()->GetSensitivity();
}

float ACoreController::GetSensitivityYaw() {
	return UCoreUserSettings::GetCoreUserSettings()->GetSensitivityYaw();
}

float ACoreController::GetSensitivityPitch() {
	return UCoreUserSettings::GetCoreUserSettings()->GetSensitivityPitch();
}

void ACoreController::MoveCam(const FInputActionValue& Value) {
	FVector2D CamVec = Value.Get<FVector2D>();
	
	AddYawInput(CamVec.X * SensitivityYaw * GetWorld()->GetDeltaSeconds());
	AddPitchInput(CamVec.Y * SensitivityPitch * GetWorld()->GetDeltaSeconds());
}
