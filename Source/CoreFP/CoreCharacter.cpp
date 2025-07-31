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
#include "PhysicsEngine/PhysicsHandleComponent.h"

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
    static ConstructorHelpers::FObjectFinder<UInputAction> RunAClassFinder(
        TEXT("/Game/Inputs/IA_Run.IA_Run"));
    static ConstructorHelpers::FObjectFinder<UInputAction> CrouchAClassFinder(
        TEXT("/Game/Inputs/IA_Crouch.IA_Crouch"));
    static ConstructorHelpers::FObjectFinder<UCurveFloat> CrouchCurveClassFinder(
        TEXT("/Game/Blueprints/Curves/C_Crouch.C_Crouch"));

    static ConstructorHelpers::FObjectFinder<UInputAction> UseClassFinder(
        TEXT("/Game/Inputs/IA_Use.IA_Use"));
    
    DefaultMappingContext = IMCClassFinder.Object;
    MoveCamAction = MCamAClassFinder.Object;
    MoveChrAction = MChrAClassFinder.Object;
    JumpAction = JumpAClassFinder.Object;
    CrouchAction = CrouchAClassFinder.Object;
    RunAction = RunAClassFinder.Object;
    UseAction = UseClassFinder.Object;
    
	GetCapsuleComponent()->InitCapsuleSize(55.0f, 96.0f);
    
	GetCharacterMovement()->NavAgentProps.bCanCrouch = true;
	GetCharacterMovement()->AirControl = 0.5f;
    GetCharacterMovement()->MaxWalkSpeed = WalkSpeed;

	Camera = CreateDefaultSubobject<UCameraComponent>(FName("Camera"));
	Camera->SetupAttachment(GetCapsuleComponent());
	Camera->SetRelativeLocation(FVector(-10.0f, 0.0f, 70.0f));
	Camera->bUsePawnControlRotation = true;

    SetPhysicsHandle(CreateDefaultSubobject<UPhysicsHandleComponent>(TEXT("PhysicsHandle")));
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

    TraceLineForGrab(FName("BaseTrace"), GrabDistance, ECC_Camera, InteractHitResult);
    GrabLocation();
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

        EIC->BindAction(UseAction, ETriggerEvent::Started, this, &ACoreCharacter::ToggleGrab);
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

    FHitResult JumpHitResult;
    FCollisionQueryParams TraceParams;
    TraceParams.AddIgnoredActor(this);
    TraceParams.bTraceComplex = true;
    TraceParams.MobilityType = EQueryMobilityType::Any;

    if (GetWorld()->LineTraceSingleByChannel(JumpHitResult, CamLoc, End, ECC_Visibility, TraceParams)) {
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

void ACoreCharacter::SetHitComponent(UPrimitiveComponent* NewHitComponent) {
    HitComponent = NewHitComponent;
}

void ACoreCharacter::SetPhysicsHandle(UPhysicsHandleComponent* NewPhysicsHandle) {
    PhysicsHandle = NewPhysicsHandle;
}

void ACoreCharacter::SetInteractHitResult(FHitResult NewHitResult) {
    InteractHitResult = NewHitResult;
}

void ACoreCharacter::BeginInteract() {
    if (!bIsGrabbing) {
        /*if (IsValid(GetHitResult().GetActor()) && GetHitResult().GetActor()->GetClass()->ImplementsInterface(UInteractionInterface::StaticClass())) {
            //IInteractionInterface::Execute_Use(GetHitResult().GetActor());
        } */if (IsValid(GetInteractHitResult().GetComponent())) {
            if (GetInteractHitResult().GetComponent()->IsSimulatingPhysics()) {
                SetHitComponent(GetInteractHitResult().GetComponent());
                PhysicsHandle->GrabComponentAtLocationWithRotation(GetHitComponent(), NAME_None, GetHitComponent()->GetComponentLocation(), GetHitComponent()->GetRelativeRotation());
                if (UCoreUserSettings::GetCoreUserSettings()->GetShadowQuality() < 2) {
                    GetPhysicsHandle()->GetGrabbedComponent()->SetCastShadow(false);
                }
                bIsGrabbing = IsValid(PhysicsHandle->GetGrabbedComponent());
            }
        }
    }
}

void ACoreCharacter::GrabLocation() {
    if (bIsGrabbing) {
        GetPhysicsHandle()->SetTargetLocationAndRotation(Camera->GetComponentLocation() + (Camera->GetForwardVector() * GetGrabDistance()), GetActorRotation());
        if (IsValid(GetHitComponent())) {
            GetHitComponent()->SetRelativeRotation(FRotator(0, GetHitComponent()->GetRelativeRotation().Yaw, 0), false, nullptr);
        }
    }
}

void ACoreCharacter::StopGrab() {
    if (bIsGrabbing) {
        GetPhysicsHandle()->GetGrabbedComponent()->SetCastShadow(true);
        GetPhysicsHandle()->ReleaseComponent();
        bIsGrabbing = IsValid(GetPhysicsHandle()->GetGrabbedComponent());
    }
}

void ACoreCharacter::ToggleGrab() {
    if (bIsGrabbing) {
        StopGrab();
        UE_LOG(LogTemp, Warning, TEXT("Stop Grabbing"));
    } else {
        BeginInteract();
        UE_LOG(LogTemp, Warning, TEXT("Grabbing"));
    }
}

void ACoreCharacter::ShootGrab() {
    if (bIsGrabbing) {
        const float Strength = 1000.0f;
        const FVector Velocity = GetCamera()->GetForwardVector() * Strength;

        GetPhysicsHandle()->GetGrabbedComponent()->SetCastShadow(true);
        GetPhysicsHandle()->GetGrabbedComponent()->AddImpulse(Velocity, NAME_None, true);
        GetPhysicsHandle()->ReleaseComponent();
        
        bIsGrabbing = IsValid(GetPhysicsHandle()->GetGrabbedComponent());
    }
}

bool ACoreCharacter::TraceLineForGrab(FName TraceTag, float Distance, ECollisionChannel Channel, FHitResult& OutResult) {
    UWorld* World = GetWorld();
    if (!bIsGrabbing && IsValid(World)) {
        FVector Start = GetCamera()->GetComponentLocation();
        FVector End = (GetCamera()->GetForwardVector() * GetGrabDistance()) + Start;

        FCollisionQueryParams TraceParams;
        TraceParams.AddIgnoredActor(this);
        TraceParams.TraceTag = TraceTag;
        TraceParams.bTraceComplex = true;
        TraceParams.MobilityType = EQueryMobilityType::Dynamic;
		
        return World->LineTraceSingleByChannel(OutResult, Start, End, Channel, TraceParams);
    }

    return false;
}

float ACoreCharacter::GetCameraFOV() {
    UCoreUserSettings* Settings = Cast<UCoreUserSettings>(UCoreUserSettings::GetCoreUserSettings());
    return Settings->GetCameraFOV();
}
