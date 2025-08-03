// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "CoreStaminaComponent.generated.h"


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class COREFP_API UCoreStaminaComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UCoreStaminaComponent();

	UFUNCTION(BlueprintPure) float GetStaminaRatio() { return GetStamina() / GetMaxStamina(); }
	UFUNCTION(BlueprintPure) float GetStamina() { return Stamina; }
	UFUNCTION(BlueprintPure) float GetMaxStamina() { return MaxStamina; }

	UFUNCTION(BlueprintCallable) void SetStamina(float NewStamina) { Stamina = NewStamina; };
	UFUNCTION(BlueprintCallable) void SetMaxStamina(float NewMaxStamina) { MaxStamina = NewMaxStamina; };
	
	UFUNCTION(BlueprintCallable) void DrainStamina();
	UFUNCTION(BlueprintCallable) void RegenStamina();

protected:
	virtual void BeginPlay() override;

private:	
	UPROPERTY() float Stamina;
	UPROPERTY(EditAnywhere) float MaxStamina;

	UPROPERTY() float DrainStaminaRate = 1.0f;
	UPROPERTY() float RegenStaminaRate = 2.0f;		
};
