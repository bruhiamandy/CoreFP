#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "CoreCharacter.generated.h"

class UInputAction;
class UInputMappingContext;

struct FInputActionValue;

class UCameraComponent;

UCLASS()
class COREFP_API ACoreCharacter : public ACharacter
{
	GENERATED_BODY()

public:
	ACoreCharacter();
    
	UFUNCTION() UCameraComponent* GetCamera() { return Camera; };

	UFUNCTION(BlueprintPure) float GetCurrentSpeed();
    UFUNCTION(BlueprintPure) float GetWalkSpeed() { return WalkSpeed; }
    UFUNCTION(BlueprintPure) float GetSprintSpeed() { return SprintSpeed; }
    UFUNCTION(BlueprintPure) float GetCrouchSpeed() { return CrouchSpeed; }
    
    UFUNCTION(BlueprintPure) bool IsCrouching() { return bIsCrouching; }
    UFUNCTION(BlueprintCallable) void SetCrouch(bool bCrouching) { bIsCrouching = bCrouching; }
    
    UFUNCTION(BlueprintPure) bool IsRunning() { return bIsRunning; }
    UFUNCTION() void SetRunning(bool bRunning) { bIsRunning = bRunning; }

	UFUNCTION(BlueprintPure) bool IsZooming() { return bIsZooming; }
	UFUNCTION(BlueprintCallable) void SetZooming(bool bZooming) { bIsZooming = bZooming; }

	void MoveCam(const FInputActionValue& Value);
	void MoveChr(const FInputActionValue& Value);
	UFUNCTION() void BeginJump();
	UFUNCTION() void EndJump();

	UFUNCTION() void BeginRun();
	UFUNCTION(BlueprintCallable) void EndRun();

	UFUNCTION(BlueprintPure) static float GetCameraFOV();

	UFUNCTION() static float GetSensitivity();
	UFUNCTION() static float GetSensitivityYaw();
	UFUNCTION() static float GetSensitivityPitch();
    
protected:
	virtual void BeginPlay() override;
	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;
	virtual void Landed(const FHitResult& Hit) override;
	virtual void Tick(float DeltaTime) override;
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

private:
    UPROPERTY() UInputMappingContext* DefaultMappingContext;
    UPROPERTY() UInputAction* MoveCamAction;
    UPROPERTY() UInputAction* MoveChrAction;
    UPROPERTY() UInputAction* JumpAction;
    UPROPERTY() UInputAction* CrouchAction;
    UPROPERTY() UInputAction* RunAction;
    
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true")) UCameraComponent* Camera;
	UPROPERTY() float CameraFOV;
    
    UPROPERTY() float WalkSpeed = 250.0f;
    UPROPERTY() float SprintSpeed = 500.0f;
    UPROPERTY() float CrouchSpeed = 125.0f;
	
    UPROPERTY() bool bIsCrouching;
    UPROPERTY() bool bIsRunning;
	UPROPERTY() bool bIsZooming;
};
