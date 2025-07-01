// Fill out your copyright notice in the Description page of Project Settings.


#include "SubtitlesInterface.h"

// Add default functionality here for any ISubtitlesInterface functions that are not pure virtual.

void ISubtitlesInterface::UpdateSubtitleText(const FText& SubtitleText) {}

void ISubtitlesInterface::SetAutoUpdateText(bool AutoUpdateText) {
    bAutoUpdateText = AutoUpdateText;
}

bool ISubtitlesInterface::GetAutoUpdateText() const {
    return bAutoUpdateText;
}
