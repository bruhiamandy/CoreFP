// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
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

	void MoveCam(const FInputActionValue& Value);
	void MoveChr(const FInputActionValue& Value);
	void BeginJump();
	void EndJump();
	
	static float GetSensitivity();
	static float GetSensitivityYaw();
	static float GetSensitivityPitch();

protected:
	virtual void BeginPlay() override;
	virtual void Tick(float DeltaTime) override;
	virtual void SetupInputComponent() override;

private:
	UPROPERTY() UInputMappingContext* DefaultMappingContext;
	UPROPERTY() UInputAction* MoveCamAction;
	UPROPERTY() UInputAction* MoveChrAction;
	UPROPERTY() UInputAction* JumpAction;

	UPROPERTY() float Sensitivity = 1.2f;
	UPROPERTY() float SensitivityYaw = 1.2f;
	UPROPERTY() float SensitivityPitch = 1.2f;
};
