#pragma once

#include "ConfigurationData.h"

/**
 * Handler class for the configuration data. Reads and saves the metadata of the
 * environment to the metadata storage file.
 *
 * Note: Singleton pattern. Please instantiate using GetInstance() static method
 */
class MRSVSPACEMETADATA_API ConfigurationDataHandler
{
public:
	/**
	 * Singleton method to get instance of configuration data
	 * handler.
	 */
	static ConfigurationDataHandler& GetInstance();
    	
    // Remove copy constructor for singleton pattern
    ConfigurationDataHandler(ConfigurationDataHandler const&) = delete;

	/**
	 * Loads the metadata from storage (Used in constructor)
	 */
	void Load();
	
	/**
	 * Saves the current metadata to the storage
	 */
	void Save() const;

	/**
	 * Returns the currently cached metadata (Doesn't reload from storage)
	 */
	TSharedPtr<FEnvironment> GetEnvironment() const;
private:

	/**
	 * Private constructor fot handler
	 */
	ConfigurationDataHandler();

	/**
	 * Helper method to get filepath to storage file
	 */
	static FString GetMetaFilePath();

	/**
	 * Currently cached metadata
	 */
	TSharedPtr<FEnvironment> EnvironmentData = MakeShareable(new FEnvironment());
};
