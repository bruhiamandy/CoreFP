// Fill out your copyright notice in the Description page of Project Settings.


#include "CoreController.h"
#include "CoreCharacter.h"
#include "CoreUserSettings.h"

#include "Camera/CameraComponent.h"
#include "GameFramework/CharacterMovementComponent.h"

#include "UObject/ConstructorHelpers.h"

#include "InputActionValue.h"
#include "InputMappingContext.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "Components/CapsuleComponent.h"

ACoreController::ACoreController() {
	static ConstructorHelpers::FObjectFinder<UInputMappingContext> IMCClassFinder(
		TEXT("/Game/Inputs/IMC_Default.IMC_Default"));

	DefaultMappingContext = IMCClassFinder.Object;
}

void ACoreController::BeginPlay() {
	Super::BeginPlay();
	if (UEnhancedInputLocalPlayerSubsystem* InputSubsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(GetLocalPlayer())) {
		InputSubsystem->AddMappingContext(DefaultMappingContext, 0);
	}

//	BindCrouch();
}

void ACoreController::Tick(float DeltaTime) {
	Super::Tick(DeltaTime);
}

void ACoreController::SetupInputComponent() {
	Super::SetupInputComponent();
	if (UEnhancedInputComponent* EIC = Cast<UEnhancedInputComponent>(InputComponent)) {

	}
}

/*

void ACoreController::BindCrouch() {
	ACoreCharacter* Chr = GetPawn<ACoreCharacter>();
	WalkHeight = Chr->GetCapsuleComponent()->GetUnscaledCapsuleHalfHeight();
	
	if (CrouchCurve) {
		FOnTimelineFloat Progress;
		Progress.BindUFunction(this, FName("UpdateCrouch"));
		CrouchTimeline.AddInterpFloat(CrouchCurve, Progress);
	}
}

void ACoreController::BeginCrouch() {
	ACoreCharacter* Chr = GetPawn<ACoreCharacter>();
	if (Chr->CanCrouch() && !CrouchTimeline.IsPlaying() && Chr->GetCharacterMovement()->IsMovingOnGround()) {
        if (Chr->GetCharacterMovement()->MaxWalkSpeed >= Chr->GetSprintSpeed()) {
            Chr->GetCharacterMovement()->MaxWalkSpeed = Chr->GetCrouchSpeed();
        }
        bIsCrouching = true;
        CrouchTimeline.Play();
	}
}

void ACoreController::UpdateCrouch(float Alpha) {
	ACoreCharacter* Chr = GetPawn<ACoreCharacter>();
	
	const float Target = bIsCrouching ? CrouchHeight : WalkHeight;
	const float Height = FMath::Lerp(WalkHeight, Target, Alpha);

	Chr->GetCapsuleComponent()->SetCapsuleHalfHeight(Height);
	Chr->GetCharacterMovement()->bWantsToCrouch = bIsCrouching;
}

void ACoreController::EndCrouch() {
	if (CrouchTimeline.IsPlaying() || bIsCrouching) {
		CrouchTimeline.Reverse();
		bIsCrouching = false;
	}
}

*/