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

public:
	USubtitlesBlock(const FObjectInitializer& ObjectInitializer);

	virtual void UpdateSubtitleText(const FText& SubtitleText) override;

	UPROPERTY(BlueprintAssignable, Category="Hook") FSubtitleUpdate OnSubtitleUpdate;
};
