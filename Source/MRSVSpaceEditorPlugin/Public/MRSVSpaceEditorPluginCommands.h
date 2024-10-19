#pragma once

#include "CoreMinimal.h"
#include "Framework/Commands/Commands.h"
#include "MRSVSpaceEditorPluginStyle.h"

/**
 * The UI commands for the MRSV Space Plugin Registration
 */
class FMRSVSpaceEditorPluginCommands : public TCommands<FMRSVSpaceEditorPluginCommands>
{
public:
	
	FMRSVSpaceEditorPluginCommands()
		: TCommands(TEXT("MRSVSpacePlugin"), NSLOCTEXT("Contexts", "MRSVSpacePlugin", "MRSV*Space Plugin"), NAME_None, FMRSVSpaceEditorPluginStyle::GetStyleSetName())
	{
	}

	// Override the register commands function to define the UI commands
	virtual void RegisterCommands() override;

	/**
	 * CommandInfo object for the plugin window UI commands
	 */
	TSharedPtr< FUICommandInfo > OpenPluginWindow;
};