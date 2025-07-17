// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameUserSettings.h"
#include "CoreUserSettings.generated.h"

/**
 * 
 */
UCLASS()
class COREFP_API UCoreUserSettings : public UGameUserSettings
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintCallable) static UCoreUserSettings* GetCoreUserSettings();
	
	UFUNCTION(BlueprintPure, Category="Input") float GetSensitivity() const { return Sensitivity; }
	UFUNCTION(BlueprintPure, Category="Input") float GetSensitivityYaw() const { return SensitivityYaw; }
	UFUNCTION(BlueprintPure, Category="Input") float GetSensitivityPitch() const { return SensitivityPitch; }
    
    UFUNCTION(BlueprintPure, Category="Camera") float GetCameraFOV() const { return CameraFOV; }
    
    UFUNCTION(BlueprintCallable, Category="Input") void SetSensitivity(float NewSensitivity) {
        Sensitivity = NewSensitivity;
        ApplySettings(false);
    }
    
    UFUNCTION(BlueprintCallable, Category="Input") void SetSensitivityYaw(float NewSensitivity) {
        SensitivityYaw = NewSensitivity;
        ApplySettings(false);
    }
    
    UFUNCTION(BlueprintCallable, Category="Input") void SetSensitivityPitch(float NewSensitivity) {
        SensitivityPitch = NewSensitivity;
        ApplySettings(false);
    }
    
    UFUNCTION(BlueprintCallable, Category="Camera") void SetCameraFOV(float NewFOV) {
        CameraFOV = FMath::Clamp(NewFOV, 75.0f, 120.0f);
        ApplySettings(false);
    }
	
private:
	UPROPERTY(Config) float Sensitivity = 1.2f;
	UPROPERTY(Config) float SensitivityYaw = 1.2f;
	UPROPERTY(Config) float SensitivityPitch = 1.2f;
    
    UPROPERTY(Config, meta=(ClampMin="75.0", ClampMax="120.0")) float CameraFOV = 90.0f;
};
