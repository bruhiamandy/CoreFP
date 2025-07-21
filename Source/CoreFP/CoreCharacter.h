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

	void MoveCam(const FInputActionValue& Value);
	void MoveChr(const FInputActionValue& Value);
	void BeginJump();
	void EndJump();

	void BeginRun();
	void EndRun();

	static float GetSensitivity();
	static float GetSensitivityYaw();
	static float GetSensitivityPitch();
    
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
    
    UPROPERTY() float WalkSpeed = 250.0f;
    UPROPERTY() float SprintSpeed = 500.0f;
    UPROPERTY() float CrouchSpeed = 125.0f;
    
    UPROPERTY() bool bIsCrouching;
    UPROPERTY() bool bIsRunning;
};
