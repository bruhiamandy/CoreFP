#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "CoreCharacter.generated.h"

class UCameraComponent;

UCLASS()
class COREFP_API ACoreCharacter : public ACharacter
{
	GENERATED_BODY()

public:
	ACoreCharacter();
    
	UFUNCTION() UCameraComponent* GetCamera() { return Camera; };
    
    UFUNCTION(BlueprintPure) float GetWalkSpeed() { return WalkSpeed; }
    UFUNCTION(BlueprintPure) float GetSprintSpeed() { return SprintSpeed; }
    UFUNCTION(BlueprintPure) float GetCrouchSpeed() { return CrouchSpeed; }
    
    UFUNCTION(BlueprintPure) bool bIsCrouching() { return bIsChrCrouching; }
    UFUNCTION(BlueprintCallable) void SetCrouch(bool bCrouching) { bIsChrCrouching = bCrouching; }

protected:
	virtual void BeginPlay() override;
	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;
	virtual void Landed(const FHitResult& Hit) override;
	virtual void Tick(float DeltaTime) override;
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

private:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true")) UCameraComponent* Camera;
    
    UPROPERTY() float WalkSpeed = 250.0f;
    UPROPERTY() float SprintSpeed = 500.0f;
    UPROPERTY() float CrouchSpeed = 125.0f;
    
    UPROPERTY() bool bIsChrCrouching;
};
