// Copyright Epic Games, Inc. All Rights Reserved.

#include "MRSVSpaceEditorPluginCommands.h"

#define LOCTEXT_NAMESPACE "FMRSVSpacePluginModule"

void FMRSVSpaceEditorPluginCommands::RegisterCommands()
{
	UI_COMMAND(OpenPluginWindow, "MRSV*Space Plugin", "MRSV*Space Plugin for Environment Configuration", EUserInterfaceActionType::Button, FInputChord());
}

#undef LOCTEXT_NAMESPACE
