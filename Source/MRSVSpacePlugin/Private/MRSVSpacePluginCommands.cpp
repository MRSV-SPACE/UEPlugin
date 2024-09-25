// Copyright Epic Games, Inc. All Rights Reserved.

#include "MRSVSpacePluginCommands.h"

#define LOCTEXT_NAMESPACE "FMRSVSpacePluginModule"

void FMRSVSpacePluginCommands::RegisterCommands()
{
	UI_COMMAND(OpenPluginWindow, "MRSV*Space Plugin", "MRSV*Space Plugin for Environment Configuration", EUserInterfaceActionType::Button, FInputChord());
}

#undef LOCTEXT_NAMESPACE
