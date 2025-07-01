// Fill out your copyright notice in the Description page of Project Settings.


#include "SubtitlesBlock.h"

#include "SubtitleManager.h"

USubtitlesBlock::USubtitlesBlock(const FObjectInitializer& ObjectInitializer) {
	if (FSubtitleManager* Manager = FSubtitleManager::GetSubtitleManager()) {
		Manager->OnSetSubtitleText().AddUFunction(this, FName("UpdateSubtitleText"));
	}
}

void USubtitlesBlock::UpdateSubtitleText(const FText& SubtitleText) {
	ISubtitlesInterface::UpdateSubtitleText(SubtitleText);
	SetText(FText::GetEmpty());

	if (GetAutoUpdateText()) {
		SetText(SubtitleText);
	}

	if (OnSubtitleUpdate.IsBound()) {
		OnSubtitleUpdate.Broadcast(SubtitleText);
	}
}
