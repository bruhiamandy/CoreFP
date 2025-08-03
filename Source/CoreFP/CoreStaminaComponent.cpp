// Fill out your copyright notice in the Description page of Project Settings.


#include "CoreStaminaComponent.h"

// Sets default values for this component's properties
UCoreStaminaComponent::UCoreStaminaComponent() {
	PrimaryComponentTick.bCanEverTick = true;
	SetMaxStamina(100.0f);
}


void UCoreStaminaComponent::DrainStamina() {
	SetStamina(FMath::Clamp(GetStamina() - DrainStaminaRate, 0.0f, GetMaxStamina()));
}

void UCoreStaminaComponent::RegenStamina() {
	SetStamina(FMath::Clamp(GetStamina() + RegenStaminaRate, 0.0f, GetMaxStamina()));
}

// Called when the game starts
void UCoreStaminaComponent::BeginPlay() {
	Super::BeginPlay();
	SetStamina(GetMaxStamina());	
}