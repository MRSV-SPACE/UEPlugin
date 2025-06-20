#include "MRSVSpaceEditorPlugin.h"

#include "ConfigurationDataHandler.h"
#include "MRSVSpaceEditorPluginStyle.h"
#include "MRSVSpaceEditorPluginCommands.h"
#include "Widgets/Docking/SDockTab.h"
#include "Widgets/Layout/SBox.h"
#include "Widgets/Text/STextBlock.h"
#include "ToolMenus.h"
#include "EnvironmentForm/EnvironmentConfigurationWidget.h"
#include "MetaDataUtils.h"

/**
 * A constant name for the Plugin
 */
static const FName MRSVSpacePluginTabName("MRSV*Space Plugin");

TSharedPtr<ConfigurationDataHandler> FMRSVSpacePluginModule::DataHandler;

void FMRSVSpacePluginModule::StartupModule()
{
	// Initialize the Plugin Style
	FMRSVSpaceEditorPluginStyle::Initialize();
	// Load the Texture Objects
	FMRSVSpaceEditorPluginStyle::ReloadTextures();
	// Register the Plugin UI Commands
	FMRSVSpaceEditorPluginCommands::Register();
	// Create Shareable instance of UI CommandList
	PluginCommands = MakeShareable(new FUICommandList);
	// Map Actions from MRSV Plugin Commands
	PluginCommands->MapAction(
		// Pass UI Commands for Open Plugin Window
		FMRSVSpaceEditorPluginCommands::Get().OpenPluginWindow,
		// On Button Click execute Handler
		FExecuteAction::CreateStatic(PluginButtonClicked),
		// Allow action execution
		FCanExecuteAction());
	// Register Callback which is executed after startup
	UToolMenus::RegisterStartupCallback(
		// As Callback registers the Menu Items
		FSimpleMulticastDelegate::FDelegate::CreateRaw(this, &FMRSVSpacePluginModule::RegisterMenus));
	// Create the TabSpawner for the Plugin Tab
	FGlobalTabmanager::Get()->RegisterNomadTabSpawner(
		// Set the Plugin Tab Name
		MRSVSpacePluginTabName,
		// When Tab should be spawner, call function for creating tab object
		FOnSpawnTab::CreateStatic(OnSpawnPluginTab))
		// Set Display Name for Title
		.SetDisplayName(FText::FromString(MRSVSpacePluginTabName.ToString()))
		// Default Tab is hidden
		.SetMenuType(ETabSpawnerMenuType::Hidden);
	// Load environment data
	DataHandler = MakeShareable(new ConfigurationDataHandler(
		FPaths::Combine(FPaths::ProjectSavedDir(), TEXT("MRSV/metadata.json"))));

}

void FMRSVSpacePluginModule::ShutdownModule()
{
	// Unregister and close all plugin related things
	UToolMenus::UnRegisterStartupCallback(this);
	UToolMenus::UnregisterOwner(this);
	FMRSVSpaceEditorPluginStyle::Shutdown();
	FMRSVSpaceEditorPluginCommands::Unregister();
	FGlobalTabmanager::Get()->UnregisterNomadTabSpawner(MRSVSpacePluginTabName);
}

TSharedPtr<TArray<FControl>> FMRSVSpacePluginModule::GetDefaultControls(FString StoragePath)
{
	//Only load if metadata file already exists
	if(IFileManager::Get().FileExists(*StoragePath))
	{
		//Create JsonObject and JsonString
		TSharedPtr<FJsonObject> JsonObject;
		FString JsonString;
		//Load File to Json String
		FFileHelper::LoadFileToString(JsonString, *StoragePath);
		if (
			//Deserialize Json to JsonObject
			FJsonSerializer::Deserialize(TJsonReaderFactory<>::Create(JsonString), JsonObject)
			//Check JsonObject is valid after deserialization
			&& JsonObject.IsValid())
		{
			return MakeShared<TArray<FControl>>(MetaDataUtils::ManuallyLoadControls(JsonObject));

		}
		else
		{
			UE_LOG(LogTemp,Error,TEXT("%s"),TEXT("Failed to load default controls config"));
		}
		
	}
	else
	{
		UE_LOG(LogTemp,Warning,TEXT("%s"),TEXT("No default controls config provider"));
	}
	return nullptr;
}


TSharedRef<SDockTab> FMRSVSpacePluginModule::OnSpawnPluginTab(const FSpawnTabArgs& SpawnTabArgs)
{
	// Create plugin tab
	return SNew(SDockTab)
		// Tab is a Nomad Tab
		.TabRole(NomadTab)
		// Content Padding 10px
		.ContentPadding(10.0f)
		[
			// Content is a ConfigurationWidget
			SNew(SEnvironmentConfigurationWidget)
			.EnvironmentData(DataHandler->GetEnvironment())
			.DefaultControls(GetDefaultControls(FPaths::Combine(FPaths::ProjectPluginsDir(),TEXT("mrsv-unreal-plugin/MRSV/DefaultControls.json"))))
			.OnSave_Lambda([]
			{
				DataHandler->Save();
			})
		];
}

void FMRSVSpacePluginModule::PluginButtonClicked()
{
	// Try to invoke new tab
	FGlobalTabmanager::Get()->TryInvokeTab(MRSVSpacePluginTabName);
}

void FMRSVSpacePluginModule::RegisterMenus()
{
	// Owner will be used for cleanup in call to UToolMenus::UnregisterOwner
	FToolMenuOwnerScoped OwnerScoped(this);
	// Adds a new Menu Point for the MRSV*Space Plugin
	UToolMenus::Get()
		// Get MainMenu for Build
		->ExtendMenu("LevelEditor.MainMenu.Build")
		// Add new section for MRSV*Space
		->AddSection("MRSVSpace", FText::FromString("MRSV*Space"))
		// Add Menu Entry with UI Command List
		.AddMenuEntryWithCommandList(FMRSVSpaceEditorPluginCommands::Get().OpenPluginWindow, PluginCommands);
	// Adds a new Button in the Tool Bar
	FToolMenuSection& Section = UToolMenus::Get()
		// Get the Toolbar Menu
		->ExtendMenu("LevelEditor.LevelEditorToolBar.ModesToolBar")
		// Add a Section for MRSV*Space
		->AddSection("MRSV*Space");
	// Create a new Entry in the Section
	FToolMenuEntry& Entry = Section.AddEntry(
		// Create Entry as ToolBarButton
		FToolMenuEntry::InitToolBarButton(FMRSVSpaceEditorPluginCommands::Get().OpenPluginWindow));
	// Set Entry UI Command List
	Entry.SetCommandList(PluginCommands);
	// Add a separator to the new Section to split from other content
	Section.AddSeparator("MRSVSpace");
}

IMPLEMENT_MODULE(FMRSVSpacePluginModule, MRSVSpacePlugin)