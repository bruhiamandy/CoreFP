// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/TimelineComponent.h"
#include "GameFramework/PlayerController.h"
#include "CoreController.generated.h"

class UInputAction;
class UInputMappingContext;

struct FInputActionValue;

/**
 * 
 */
UCLASS()
class COREFP_API ACoreController : public APlayerController
{
	GENERATED_BODY()

public:
	ACoreController();

	/*
	void BindCrouch();
	void BeginCrouch();
	void UpdateCrouch(float Alpha);
	void EndCrouch();
	*/

protected:
	virtual void BeginPlay() override;
	virtual void Tick(float DeltaTime) override;
	virtual void SetupInputComponent() override;

private:
	UPROPERTY() UInputMappingContext* DefaultMappingContext;
};
