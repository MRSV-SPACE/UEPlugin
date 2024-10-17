#pragma once

#include "CoreMinimal.h"
#include "Framework/Commands/Commands.h"
#include "MRSVSpaceEditorPluginStyle.h"

class FMRSVSpaceEditorPluginCommands : public TCommands<FMRSVSpaceEditorPluginCommands>
{
public:

	FMRSVSpaceEditorPluginCommands()
		: TCommands<FMRSVSpaceEditorPluginCommands>(TEXT("MRSVSpacePlugin"), NSLOCTEXT("Contexts", "MRSVSpacePlugin", "MRSV*Space Plugin"), NAME_None, FMRSVSpaceEditorPluginStyle::GetStyleSetName())
	{
	}

	// TCommands<> interface
	virtual void RegisterCommands() override;

	TSharedPtr< FUICommandInfo > OpenPluginWindow;
};