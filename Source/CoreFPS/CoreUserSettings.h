// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameUserSettings.h"
#include "CoreUserSettings.generated.h"

/**
 * 
 */
UCLASS()
class COREFPS_API UCoreUserSettings : public UGameUserSettings
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintCallable) static UCoreUserSettings* GetCoreUserSettings();
	
	UFUNCTION(BlueprintPure, Category="Input") float GetSensitivity() const { return Sensitivity; }
	UFUNCTION(BlueprintPure, Category="Input") float GetSensitivityYaw() const { return SensitivityYaw; }
	UFUNCTION(BlueprintPure, Category="Input") float GetSensitivityPitch() const { return SensitivityPitch; }

	UFUNCTION(BlueprintCallable, Category="Input") void SetSensitivity(float NewSensitivity) { Sensitivity = NewSensitivity; }
	UFUNCTION(BlueprintCallable, Category="Input") void SetSensitivityYaw(float NewSensitivity) {SensitivityYaw = NewSensitivity; }
	UFUNCTION(BlueprintCallable, Category="Input") void SetSensitivityPitch(float NewSensitivity) { SensitivityPitch = NewSensitivity; }
	
private:
	UPROPERTY(Config) float Sensitivity = 1.2f;
	UPROPERTY(Config) float SensitivityYaw = 1.2f;
	UPROPERTY(Config) float SensitivityPitch = 1.2f;
};
