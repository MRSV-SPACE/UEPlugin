// Copyright Epic Games, Inc. All Rights Reserved.

#include "MRSVEnvironmentConfigurer.h"

#include "MRSVEnvironmentConfigurerStyle.h"
#include "MRSVEnvironmentConfigurerCommands.h"
#include "Widgets/Docking/SDockTab.h"
#include "Widgets/Layout/SBox.h"
#include "Widgets/Text/STextBlock.h"
#include "ToolMenus.h"
#include "ConfigurationForm/ConfigurationFormWidget.h"

static const FName MRSVEnvironmentConfigurerTabName("MRSV Environment Configurer");

#define LOCTEXT_NAMESPACE "FMRSVEnvironmentConfigurerModule"

void FMRSVEnvironmentConfigurerModule::StartupModule()
{
	// This code will execute after your module is loaded into memory; the exact timing is specified in the .uplugin file per-module
	
	FMRSVEnvironmentConfigurerStyle::Initialize();
	FMRSVEnvironmentConfigurerStyle::ReloadTextures();

	FMRSVEnvironmentConfigurerCommands::Register();
	
	PluginCommands = MakeShareable(new FUICommandList);

	PluginCommands->MapAction(
		FMRSVEnvironmentConfigurerCommands::Get().OpenPluginWindow,
		FExecuteAction::CreateRaw(this, &FMRSVEnvironmentConfigurerModule::PluginButtonClicked),
		FCanExecuteAction());

	UToolMenus::RegisterStartupCallback(FSimpleMulticastDelegate::FDelegate::CreateRaw(this, &FMRSVEnvironmentConfigurerModule::RegisterMenus));
	
	FGlobalTabmanager::Get()->RegisterNomadTabSpawner(MRSVEnvironmentConfigurerTabName, FOnSpawnTab::CreateRaw(this, &FMRSVEnvironmentConfigurerModule::OnSpawnPluginTab))
		.SetDisplayName(LOCTEXT("FMRSVEnvironmentConfigurerTabTitle", "MRSV Environment Configurer"))
		.SetMenuType(ETabSpawnerMenuType::Hidden);
}

void FMRSVEnvironmentConfigurerModule::ShutdownModule()
{
	// This function may be called during shutdown to clean up your module.  For modules that support dynamic reloading,
	// we call this function before unloading the module.

	UToolMenus::UnRegisterStartupCallback(this);

	UToolMenus::UnregisterOwner(this);

	FMRSVEnvironmentConfigurerStyle::Shutdown();

	FMRSVEnvironmentConfigurerCommands::Unregister();

	FGlobalTabmanager::Get()->UnregisterNomadTabSpawner(MRSVEnvironmentConfigurerTabName);
}

TSharedRef<SDockTab> FMRSVEnvironmentConfigurerModule::OnSpawnPluginTab(const FSpawnTabArgs& SpawnTabArgs)
{
	return SNew(SDockTab)
		.TabRole(NomadTab)
		.ContentPadding(10.0f)
		[
			SNew(SConfigurationFormWidget)
		];
}

void FMRSVEnvironmentConfigurerModule::PluginButtonClicked()
{
	FGlobalTabmanager::Get()->TryInvokeTab(MRSVEnvironmentConfigurerTabName);
}

void FMRSVEnvironmentConfigurerModule::RegisterMenus()
{
	// Owner will be used for cleanup in call to UToolMenus::UnregisterOwner
	FToolMenuOwnerScoped OwnerScoped(this);

	{
		UToolMenu* Menu = UToolMenus::Get()->ExtendMenu("LevelEditor.MainMenu.Window");
		{
			FToolMenuSection& Section = Menu->FindOrAddSection("WindowLayout");
			Section.AddMenuEntryWithCommandList(FMRSVEnvironmentConfigurerCommands::Get().OpenPluginWindow, PluginCommands);
		}
	}

	{
		UToolMenu* ToolbarMenu = UToolMenus::Get()->ExtendMenu("LevelEditor.LevelEditorToolBar.ModesToolBar");
		{
			FToolMenuSection& Section = ToolbarMenu->FindOrAddSection("File");
			{
				FToolMenuEntry& Entry = Section.AddEntry(FToolMenuEntry::InitToolBarButton(FMRSVEnvironmentConfigurerCommands::Get().OpenPluginWindow));
				Entry.SetCommandList(PluginCommands);
			}
		}
	}
}

#undef LOCTEXT_NAMESPACE
	
IMPLEMENT_MODULE(FMRSVEnvironmentConfigurerModule, MRSVEnvironmentConfigurer)