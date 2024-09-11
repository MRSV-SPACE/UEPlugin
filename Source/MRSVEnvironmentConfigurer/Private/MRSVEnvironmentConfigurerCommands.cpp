// Copyright Epic Games, Inc. All Rights Reserved.

#include "MRSVEnvironmentConfigurerCommands.h"

#define LOCTEXT_NAMESPACE "FMRSVEnvironmentConfigurerModule"

void FMRSVEnvironmentConfigurerCommands::RegisterCommands()
{
	UI_COMMAND(OpenPluginWindow, "MRSVEnvironmentConfigurer", "MRSV*Space Environment Configurator", EUserInterfaceActionType::Button, FInputChord());
}

#undef LOCTEXT_NAMESPACE
