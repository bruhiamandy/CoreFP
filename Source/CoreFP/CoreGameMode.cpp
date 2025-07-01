// Fill out your copyright notice in the Description page of Project Settings.


#include "CoreGameMode.h"

#include "CoreCharacter.h"
#include "CoreController.h"

#include "UObject/ConstructorHelpers.h"

#include "GameFramework/Actor.h"

ACoreGameMode::ACoreGameMode() {
    static ConstructorHelpers::FObjectFinder<UBlueprint> PawnChr(TEXT("/Game/Blueprints/BP_PlayerCharacter.BP_PlayerCharacter"));
    DefaultPawnClass = (UClass*)PawnChr.Object->GeneratedClass;
	PlayerControllerClass = ACoreController::StaticClass();
}

void ACoreGameMode::BeginPlay() {
	Super::BeginPlay();
	
}

