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
	static ConstructorHelpers::FObjectFinder<UInputAction> MCamAClassFinder(
		TEXT("/Game/Inputs/IA_MoveCam.IA_MoveCam"));
	static ConstructorHelpers::FObjectFinder<UInputAction> MChrAClassFinder(
		TEXT("/Game/Inputs/IA_MoveChr.IA_MoveChr"));
	static ConstructorHelpers::FObjectFinder<UInputAction> JumpAClassFinder(
		TEXT("/Game/Inputs/IA_Jump.IA_Jump"));
	static ConstructorHelpers::FObjectFinder<UInputAction> CrouchAClassFinder(
		TEXT("/Game/Inputs/IA_Crouch.IA_Crouch"));
	static ConstructorHelpers::FObjectFinder<UCurveFloat> CrouchCurveClassFinder(
		TEXT("/Game/Blueprints/Curves/C_Crouch.C_Crouch"));

	DefaultMappingContext = IMCClassFinder.Object;
	MoveCamAction = MCamAClassFinder.Object;
	MoveChrAction = MChrAClassFinder.Object;
	JumpAction = JumpAClassFinder.Object;
	CrouchAction = CrouchAClassFinder.Object;
	CrouchCurve = CrouchCurveClassFinder.Object;
}

void ACoreController::BeginPlay() {
	Super::BeginPlay();
	if (UEnhancedInputLocalPlayerSubsystem* InputSubsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(GetLocalPlayer())) {
		InputSubsystem->AddMappingContext(DefaultMappingContext, 0);
	}
	
	Sensitivity = GetSensitivity();
	SensitivityYaw = GetSensitivityYaw();
	SensitivityPitch = GetSensitivityPitch();

	BindCrouch();
}

void ACoreController::Tick(float DeltaTime) {
	Super::Tick(DeltaTime);
}

void ACoreController::SetupInputComponent() {
	Super::SetupInputComponent();
	if (UEnhancedInputComponent* EIC = Cast<UEnhancedInputComponent>(InputComponent))
	{
		EIC->BindAction(MoveCamAction, ETriggerEvent::Triggered, this, &ACoreController::MoveCam);
		EIC->BindAction(MoveChrAction, ETriggerEvent::Triggered, this, &ACoreController::MoveChr);

		EIC->BindAction(JumpAction, ETriggerEvent::Started, this, &ACoreController::BeginJump);
		EIC->BindAction(JumpAction, ETriggerEvent::Completed, this, &ACoreController::EndJump);

		EIC->BindAction(CrouchAction, ETriggerEvent::Started, this, &ACoreController::BeginCrouch);
		EIC->BindAction(CrouchAction, ETriggerEvent::Completed, this, &ACoreController::EndCrouch);
	}
}

void ACoreController::MoveChr(const FInputActionValue& Value)
{
	FVector2D MoveVec = Value.Get<FVector2D>();

	GetPawn()->AddMovementInput(GetPawn()->GetActorRightVector(), MoveVec.X);
	GetPawn()->AddMovementInput(GetPawn()->GetActorForwardVector(), MoveVec.Y);
}

void ACoreController::BeginJump() {
	ACoreCharacter* Chr = GetPawn<ACoreCharacter>();
	
	FVector CamLoc = Chr->GetCamera()->GetComponentLocation();
	FVector End = CamLoc + FVector(0.0f, 0.0f, 96.0f);

	FHitResult HitResult;
	FCollisionQueryParams TraceParams;
	TraceParams.AddIgnoredActor(this);
	TraceParams.bTraceComplex = true;
	TraceParams.MobilityType = EQueryMobilityType::Any;

	if (GetWorld()->LineTraceSingleByChannel(HitResult, CamLoc, End, ECC_Visibility, TraceParams)) {
		Chr->GetCharacterMovement()->JumpZVelocity = Chr->GetVelocity().Size();
		Chr->JumpMaxHoldTime = 0.0f;
	} else {
		Chr->GetCharacterMovement()->JumpZVelocity = 420.0f;
		Chr->JumpMaxHoldTime = 0.025f;
	}
	
	Chr->Jump();
}

void ACoreController::EndJump() {
	ACoreCharacter* Chr = GetPawn<ACoreCharacter>();
	Chr->StopJumping();
}

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
		CrouchTimeline.Play();
		bIsCrouching = true;
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
	
	AddYawInput(CamVec.X * (SensitivityYaw * 100.0f) * GetWorld()->GetDeltaSeconds());
	AddPitchInput(CamVec.Y * (SensitivityPitch * 100.0f) * GetWorld()->GetDeltaSeconds());
}
