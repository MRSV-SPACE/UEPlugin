// Copyright Epic Games, Inc. All Rights Reserved.

#include "MRSVSpacePlugin.h"

#include "MRSVSpacePluginStyle.h"
#include "MRSVSpacePluginCommands.h"
#include "Widgets/Docking/SDockTab.h"
#include "Widgets/Layout/SBox.h"
#include "Widgets/Text/STextBlock.h"
#include "ToolMenus.h"
#include "Data/ConfigurationDataHandler.h"
#include "Form/ConfigurationWidget.h"

static const FName MRSVSpacePluginTabName("MRSV*Space Artist Plugin");

#define LOCTEXT_NAMESPACE "FMRSVSpacePluginModule"

void FMRSVSpacePluginModule::StartupModule()
{
	// This code will execute after your module is loaded into memory; the exact timing is specified in the .uplugin file per-module
	
	FMRSVSpacePluginStyle::Initialize();
	FMRSVSpacePluginStyle::ReloadTextures();

	FMRSVSpacePluginCommands::Register();
	
	PluginCommands = MakeShareable(new FUICommandList);

	PluginCommands->MapAction(
		FMRSVSpacePluginCommands::Get().OpenPluginWindow,
		FExecuteAction::CreateRaw(this, &FMRSVSpacePluginModule::PluginButtonClicked),
		FCanExecuteAction());

	UToolMenus::RegisterStartupCallback(FSimpleMulticastDelegate::FDelegate::CreateRaw(this, &FMRSVSpacePluginModule::RegisterMenus));
	
	FGlobalTabmanager::Get()->RegisterNomadTabSpawner(MRSVSpacePluginTabName, FOnSpawnTab::CreateRaw(this, &FMRSVSpacePluginModule::OnSpawnPluginTab))
		.SetDisplayName(LOCTEXT("FMRSVSpacePluginTabTitle", "MRSV*Space Plugin"))
		.SetMenuType(ETabSpawnerMenuType::Hidden);
}

void FMRSVSpacePluginModule::ShutdownModule()
{
	// This function may be called during shutdown to clean up your module.  For modules that support dynamic reloading,
	// we call this function before unloading the module.

	UToolMenus::UnRegisterStartupCallback(this);

	UToolMenus::UnregisterOwner(this);

	FMRSVSpacePluginStyle::Shutdown();

	FMRSVSpacePluginCommands::Unregister();

	FGlobalTabmanager::Get()->UnregisterNomadTabSpawner(MRSVSpacePluginTabName);
}

TSharedRef<SDockTab> FMRSVSpacePluginModule::OnSpawnPluginTab(const FSpawnTabArgs& SpawnTabArgs)
{
	// Load metadata file on startup
	ConfigurationDataHandler* DataHandler = new ConfigurationDataHandler();
	DataHandler->Load();
	// Create plugin tab
	return SNew(SDockTab)
		.TabRole(NomadTab)
		.ContentPadding(10.0f)
		[
			SNew(SConfigurationWidget)
			.DataHandler(MakeShareable(DataHandler))
		];
}

void FMRSVSpacePluginModule::PluginButtonClicked()
{
	FGlobalTabmanager::Get()->TryInvokeTab(MRSVSpacePluginTabName);
}

void FMRSVSpacePluginModule::RegisterMenus()
{
	// Owner will be used for cleanup in call to UToolMenus::UnregisterOwner
	FToolMenuOwnerScoped OwnerScoped(this);

	{
		UToolMenu* Menu = UToolMenus::Get()->ExtendMenu("LevelEditor.MainMenu.Build");
		{
			FToolMenuSection& Section = Menu->AddSection("MRSVSpace", FText::FromString("MRSV*Space"));
			Section.AddMenuEntryWithCommandList(FMRSVSpacePluginCommands::Get().OpenPluginWindow, PluginCommands);
		}
	}

	{
		UToolMenu* ToolbarMenu = UToolMenus::Get()->ExtendMenu("LevelEditor.LevelEditorToolBar.ModesToolBar");
		{
			FToolMenuSection& Section = ToolbarMenu->AddSection("MRSV*Space");
			{
				FToolMenuEntry& Entry = Section.AddEntry(FToolMenuEntry::InitToolBarButton(FMRSVSpacePluginCommands::Get().OpenPluginWindow));
				Entry.SetCommandList(PluginCommands);
				Section.AddSeparator("MRSVSpace");
			}
		}
	}
}

#undef LOCTEXT_NAMESPACE
	
IMPLEMENT_MODULE(FMRSVSpacePluginModule, MRSVSpacePlugin)