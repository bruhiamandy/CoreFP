// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "CoreHealthComponent.generated.h"


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class COREFP_API UCoreHealthComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	UCoreHealthComponent();
	UFUNCTION() static float CalcFallDamage(float Velocity, int VelocityLimit, float FallDamageMultiplier);

	UFUNCTION(BlueprintPure) float GetHealthRatio() { return GetHealth() / GetMaxHealth(); }
	UFUNCTION(BlueprintPure) float GetHealth() { return Health; }
	UFUNCTION(BlueprintPure) float GetMaxHealth() { return MaxHealth; }
	UFUNCTION(BlueprintCallable) void SetHealth(float NewHealth) { Health = NewHealth; }
	UFUNCTION(BlueprintCallable) void SetMaxHealth(float NewMaxHealth) { MaxHealth = NewMaxHealth; }

protected:
	// Called when the game starts
	virtual void BeginPlay() override;
	UFUNCTION() void TakeDamage(AActor* DamagedActor, float Damage, const class UDamageType* DamageType, class AController* InstigatedBy, AActor* DamageCauser);

private:
	UPROPERTY(EditAnywhere) float MaxHealth;
	UPROPERTY() float Health;		
};
