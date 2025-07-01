// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "SubtitlesInterface.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FSubtitleUpdate, const FText&, SubtitleText);

// This class does not need to be modified.
UINTERFACE(MinimalAPI, BlueprintType, meta=(CannotImplementInterfaceInBlueprint))
class USubtitlesInterface : public UInterface
{
	GENERATED_BODY()
};

/**
 * 
 */
class COREFP_API ISubtitlesInterface
{
	GENERATED_BODY()

	// Add interface functions to this class. This is the class that will be inherited to implement this interface.
public:
    UFUNCTION(BlueprintCallable, Category="Hook") virtual void UpdateSubtitleText(const FText& SubtitleText);
    UFUNCTION(BlueprintCallable, Category="Hook") virtual void SetAutoUpdateText(bool AutoUpdateText);
    UFUNCTION(BlueprintCallable, Category="Hook") virtual bool GetAutoUpdateText() const;
    
private:
    bool bAutoUpdateText = true;
};
