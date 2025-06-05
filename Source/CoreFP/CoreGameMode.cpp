// Fill out your copyright notice in the Description page of Project Settings.


#include "CoreGameMode.h"

#include "CoreCharacter.h"
#include "CoreController.h"

#include "UObject/ConstructorHelpers.h"

#include "GameFramework/Actor.h"

ACoreGameMode::ACoreGameMode() {
//	static ConstructorHelpers::FClassFinder<APawn> PlayerChrClassFinder(TEXT("/Game/Blueprints/Character"));
	DefaultPawnClass = ACoreCharacter::StaticClass();
	PlayerControllerClass = ACoreController::StaticClass();
}

void ACoreGameMode::BeginPlay() {
	Super::BeginPlay();
	
}