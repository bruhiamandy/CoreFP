// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "SubtitlesInterface.h"
#include "Components/TextBlock.h"
#include "SubtitlesBlock.generated.h"

/**
 * 
 */
UCLASS()
class COREFP_API USubtitlesBlock : public UTextBlock, public ISubtitlesInterface
{
	GENERATED_BODY()
	
};
