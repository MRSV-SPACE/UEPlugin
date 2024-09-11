// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Framework/Commands/Commands.h"
#include "MRSVEnvironmentConfigurerStyle.h"

class FMRSVEnvironmentConfigurerCommands : public TCommands<FMRSVEnvironmentConfigurerCommands>
{
public:

	FMRSVEnvironmentConfigurerCommands()
		: TCommands<FMRSVEnvironmentConfigurerCommands>(TEXT("MRSVEnvironmentConfigurer"), NSLOCTEXT("Contexts", "MRSVEnvironmentConfigurer", "MRSVEnvironmentConfigurer Plugin"), NAME_None, FMRSVEnvironmentConfigurerStyle::GetStyleSetName())
	{
	}

	// TCommands<> interface
	virtual void RegisterCommands() override;

	TSharedPtr< FUICommandInfo > OpenPluginWindow;
};