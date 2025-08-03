// Fill out your copyright notice in the Description page of Project Settings.


#include "CoreHealthComponent.h"

// Sets default values for this component's properties
UCoreHealthComponent::UCoreHealthComponent() {
	PrimaryComponentTick.bCanEverTick = false;
	SetMaxHealth(100.0f);
	SetHealth(GetMaxHealth());
}

float UCoreHealthComponent::CalcFallDamage(float Velocity, int VelocityLimit, float FallDamageMultiplier) {
	if (Velocity > 0.0f) {
		return FMath::Clamp((Velocity - VelocityLimit) * FallDamageMultiplier, 0.0f, 9999.0f);
	}

	return 0;
}

void UCoreHealthComponent::TakeDamage(AActor* DamagedActor, float Damage, const class UDamageType* DamageType,
	class AController* InstigatedBy, AActor* DamageCauser) {
	SetHealth(FMath::Clamp(GetHealth() - Damage, 0.0f, GetMaxHealth()));
	if (IsValid(GetOwner()) && GetHealth() <= 0) { GetOwner()->Destroy(); }
}

void UCoreHealthComponent::BeginPlay() {
	Super::BeginPlay();
	GetOwner()->OnTakeAnyDamage.AddDynamic(this, &UCoreHealthComponent::TakeDamage);
}
