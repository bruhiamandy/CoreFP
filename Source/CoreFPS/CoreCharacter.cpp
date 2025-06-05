// Fill out your copyright notice in the Description page of Project Settings.


#include "CoreCharacter.h"

#include "Camera/CameraComponent.h"
#include "Components/CapsuleComponent.h"
#include "GameFramework/CharacterMovementComponent.h"

// Sets default values
ACoreCharacter::ACoreCharacter() {
	PrimaryActorTick.bCanEverTick = true;
	GetCapsuleComponent()->InitCapsuleSize(55.0f, 96.0f);
	
	GetCharacterMovement()->AirControl = 0.5f;

	Camera = CreateDefaultSubobject<UCameraComponent>(FName("Camera"));
	Camera->SetupAttachment(GetCapsuleComponent());
	Camera->SetRelativeLocation(FVector(-10.0f, 0.0f, 70.0f));
	Camera->bUsePawnControlRotation = true;
}

// Called when the game starts or when spawned
void ACoreCharacter::BeginPlay() {
	Super::BeginPlay();
	
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

}

