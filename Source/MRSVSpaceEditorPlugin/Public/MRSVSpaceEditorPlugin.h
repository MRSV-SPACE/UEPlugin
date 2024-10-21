#pragma once

#include "CoreMinimal.h"
#include "ConfigurationDataHandler.h"
#include "EnvironmentForm/EnvironmentConfigurationWidget.h"
#include "Modules/ModuleManager.h"

/**
 * The main MRSV*Space Plugin Module which registers Plugin Menu Options.
 */
class FMRSVSpacePluginModule : public IModuleInterface
{
public:

	/**
	 * Will be executed when the plugin is loaded into memory
	 */
	virtual void StartupModule() override;

	/**
	 * Will be executed when plugin is removed from memory
	 */
	virtual void ShutdownModule() override;
	
	/** This function will be bound to Command (by default it will bring up plugin window) */
	static void PluginButtonClicked();
private:
	/**
	 * Registers all the Menu Options for the Plugin.
	 * 1. Toolbar Logo Button
	 * 2. MainMenu "Build" new Section
	 */
	void RegisterMenus();

	
	/**
	 * Is called when the plugin tab should be spawned. Defines the content of the window
	 * as the ConfigurationWidget
	 * 
	 * @param SpawnTabArgs Arguments given on SpawnTab
	 * @return A Ref to the new Tab spawned
	 */
	static TSharedRef<SDockTab> OnSpawnPluginTab(const FSpawnTabArgs& SpawnTabArgs);
	
	/**
	 * The UI Commands for the Plugin
	 */
	TSharedPtr<FUICommandList> PluginCommands;

	/**
	 * The environment data ptr
	 */
	static TSharedPtr<ConfigurationDataHandler> ConfigurationDataHandler;
};
