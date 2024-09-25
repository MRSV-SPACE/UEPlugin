// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Framework/Commands/Commands.h"
#include "MRSVSpacePluginStyle.h"

class FMRSVSpacePluginCommands : public TCommands<FMRSVSpacePluginCommands>
{
public:

	FMRSVSpacePluginCommands()
		: TCommands<FMRSVSpacePluginCommands>(TEXT("MRSVSpacePlugin"), NSLOCTEXT("Contexts", "MRSVSpacePlugin", "MRSV*Space Plugin"), NAME_None, FMRSVSpacePluginStyle::GetStyleSetName())
	{
	}

	// TCommands<> interface
	virtual void RegisterCommands() override;

	TSharedPtr< FUICommandInfo > OpenPluginWindow;
};