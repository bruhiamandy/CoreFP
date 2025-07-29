// Fill out your copyright notice in the Description page of Project Settings.


#include "CoreCharacter.h"

#include "CoreController.h"
#include "CoreUserSettings.h"

#include "Camera/CameraComponent.h"
#include "Components/CapsuleComponent.h"
#include "GameFramework/CharacterMovementComponent.h"

#include "UObject/ConstructorHelpers.h"

#include "InputActionValue.h"
#include "InputMappingContext.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"

// Sets default values
ACoreCharacter::ACoreCharacter() {
	PrimaryActorTick.bCanEverTick = true;
    
    static ConstructorHelpers::FObjectFinder<UInputMappingContext> IMCClassFinder(
        TEXT("/Game/Inputs/IMC_Character.IMC_Character"));
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
    static ConstructorHelpers::FObjectFinder<UInputAction> RunAClassFinder(
        TEXT("/Game/Inputs/IA_Run.IA_Run"));
    
    DefaultMappingContext = IMCClassFinder.Object;
    MoveCamAction = MCamAClassFinder.Object;
    MoveChrAction = MChrAClassFinder.Object;
    JumpAction = JumpAClassFinder.Object;
    CrouchAction = CrouchAClassFinder.Object;
    RunAction = RunAClassFinder.Object;
    
	GetCapsuleComponent()->InitCapsuleSize(55.0f, 96.0f);
    
	GetCharacterMovement()->NavAgentProps.bCanCrouch = true;
	GetCharacterMovement()->AirControl = 0.5f;
    GetCharacterMovement()->MaxWalkSpeed = WalkSpeed;

	Camera = CreateDefaultSubobject<UCameraComponent>(FName("Camera"));
	Camera->SetupAttachment(GetCapsuleComponent());
	Camera->SetRelativeLocation(FVector(-10.0f, 0.0f, 70.0f));
	Camera->bUsePawnControlRotation = true;
}

float ACoreCharacter::GetCurrentSpeed() {
    return GetCharacterMovement()->MaxWalkSpeed;
}

// Called when the game starts or when spawned
void ACoreCharacter::BeginPlay() {
	Super::BeginPlay();
    Camera->SetFieldOfView(FMath::Clamp(GetCameraFOV(), 75.0f, 120.0f));
    
    if (ACoreController* PlayerController = Cast<ACoreController>(Controller)) {
        if (UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(PlayerController->GetLocalPlayer())) {
                Subsystem->AddMappingContext(DefaultMappingContext, 0);
        }
    }
}

void ACoreCharacter::EndPlay(const EEndPlayReason::Type EndPlayReason) {
	Super::EndPlay(EndPlayReason);
}

void ACoreCharacter::Landed(const FHitResult& Hit) {
	Super::Landed(Hit);
}

// Called every frame
void ACoreCharacter::Tick(float DeltaTime) {
	Super::Tick(DeltaTime);

}

// Called to bind functionality to input
void ACoreCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent) {
    Super::SetupPlayerInputComponent(PlayerInputComponent);
    if (UEnhancedInputComponent* EIC = Cast<UEnhancedInputComponent>(PlayerInputComponent)){
        EIC->BindAction(MoveCamAction, ETriggerEvent::Triggered, this, &ACoreCharacter::MoveCam);
        EIC->BindAction(MoveChrAction, ETriggerEvent::Triggered, this, &ACoreCharacter::MoveChr);

        EIC->BindAction(JumpAction, ETriggerEvent::Started, this, &ACoreCharacter::BeginJump);
        EIC->BindAction(JumpAction, ETriggerEvent::Completed, this, &ACoreCharacter::EndJump);

        EIC->BindAction(RunAction, ETriggerEvent::Started, this, &ACoreCharacter::BeginRun);
        EIC->BindAction(RunAction, ETriggerEvent::Completed, this, &ACoreCharacter::EndRun);
    }
}

float ACoreCharacter::GetSensitivity() {
    return UCoreUserSettings::GetCoreUserSettings()->GetSensitivity();
}

float ACoreCharacter::GetSensitivityYaw() {
    return UCoreUserSettings::GetCoreUserSettings()->GetSensitivityYaw();
}

float ACoreCharacter::GetSensitivityPitch() {
    return UCoreUserSettings::GetCoreUserSettings()->GetSensitivityPitch();
}

void ACoreCharacter::MoveCam(const FInputActionValue& Value) {
    FVector2D CamVec = Value.Get<FVector2D>();

    if (Controller != nullptr) {
        AddControllerYawInput(CamVec.X * (GetSensitivityYaw() * 100.0f) * GetWorld()->GetDeltaSeconds());
        AddControllerPitchInput(CamVec.Y * (GetSensitivityPitch() * 100.0f) * GetWorld()->GetDeltaSeconds());
    }
}

void ACoreCharacter::MoveChr(const FInputActionValue& Value) {
    FVector2D MoveVec = Value.Get<FVector2D>();

    if (Controller != nullptr) {
        AddMovementInput(GetActorRightVector(), MoveVec.X);
        AddMovementInput(GetActorForwardVector(), MoveVec.Y);        
    }
}

void ACoreCharacter::BeginJump() {
    FVector CamLoc = GetCamera()->GetComponentLocation();
    FVector End = CamLoc + FVector(0.0f, 0.0f, 96.0f);

    FHitResult HitResult;
    FCollisionQueryParams TraceParams;
    TraceParams.AddIgnoredActor(this);
    TraceParams.bTraceComplex = true;
    TraceParams.MobilityType = EQueryMobilityType::Any;

    if (GetWorld()->LineTraceSingleByChannel(HitResult, CamLoc, End, ECC_Visibility, TraceParams)) {
        GetCharacterMovement()->JumpZVelocity = GetVelocity().Size();
        JumpMaxHoldTime = 0.0f;
    } else {
        GetCharacterMovement()->JumpZVelocity = 420.0f;
        JumpMaxHoldTime = 0.025f;
    }
	
    Jump();
}

void ACoreCharacter::EndJump() {
    StopJumping();
}

void ACoreCharacter::BeginRun() {
    UE_LOG(LogTemp, Log, TEXT("ACoreCharacter::BeginRun execute"));
    if(!IsCrouching()) {
        if(!GetCharacterMovement()->IsFalling()) {
            UE_LOG(LogTemp, Log, TEXT("ACoreCharacter::BeginRun success"));
            GetCharacterMovement()->MaxWalkSpeed = GetWalkSpeed() * 3.0f;
            SetRunning(true);
        }
    }
}

void ACoreCharacter::EndRun() {
    UE_LOG(LogTemp, Log, TEXT("ACoreCharacter::EndRun execute"));
    if(IsRunning()) {
        UE_LOG(LogTemp, Log, TEXT("ACoreCharacter::EndRun success"));
        GetCharacterMovement()->MaxWalkSpeed = GetWalkSpeed();
        SetRunning(false);
    }
}

float ACoreCharacter::GetCameraFOV() {
    UCoreUserSettings* Settings = Cast<UCoreUserSettings>(UCoreUserSettings::GetCoreUserSettings());
    return Settings->GetCameraFOV();
}
