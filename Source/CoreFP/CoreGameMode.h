// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameMode.h"
#include "CoreGameMode.generated.h"

/**
 * 
 */
UCLASS()
class COREFP_API ACoreGameMode : public AGameMode
{
	GENERATED_BODY()

public:
	ACoreGameMode();

protected:
	virtual void BeginPlay() override;
};
