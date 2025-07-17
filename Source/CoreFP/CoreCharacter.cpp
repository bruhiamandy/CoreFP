// Fill out your copyright notice in the Description page of Project Settings.


#include "CoreCharacter.h"
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
        TEXT("/Game/Inputs/IMC_Default.IMC_Default"));
    static ConstructorHelpers::FObjectFinder<UInputAction> RunAClassFinder(
        TEXT("/Game/Inputs/IA_Run.IA_Run"));
    
    DefaultMappingContext = IMCClassFinder.Object;
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

// Called when the game starts or when spawned
void ACoreCharacter::BeginPlay() {
	Super::BeginPlay();
    UCoreUserSettings* Settings = Cast<UCoreUserSettings>(UCoreUserSettings::GetCoreUserSettings());
    Camera->SetFieldOfView(Settings->GetCameraFOV());
    
    if (APlayerController* PlayerController = Cast<APlayerController>(Controller)) {
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
        EIC->BindAction(RunAction, ETriggerEvent::Started, this, &ACoreCharacter::BeginRun);
        EIC->BindAction(RunAction, ETriggerEvent::Canceled, this, &ACoreCharacter::EndRun);
    }
}

void ACoreCharacter::BeginRun() {
    if(!IsCrouching()) {
        if(!GetCharacterMovement()->IsFalling()) {
            GetCharacterMovement()->MaxWalkSpeed = GetSprintSpeed();
            SetRunning(true);
        }
    }
    
    EndRun();
}

void ACoreCharacter::EndRun() {
    if(IsRunning()) {
        GetCharacterMovement()->MaxWalkSpeed = GetWalkSpeed();
        SetRunning(false);
    }
}
