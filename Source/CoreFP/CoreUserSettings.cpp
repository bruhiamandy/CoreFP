// Fill out your copyright notice in the Description page of Project Settings.


#include "CoreUserSettings.h"

UCoreUserSettings* UCoreUserSettings::GetCoreUserSettings() {
	return Cast<UCoreUserSettings>(GetGameUserSettings());
}
