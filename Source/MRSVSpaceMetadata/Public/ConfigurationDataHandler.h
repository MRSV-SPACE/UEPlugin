#pragma once

#include "ConfigurationData.h"

/**
 * Handler class for the configuration data. Reads and saves the metadata of the
 * environment to the metadata storage file.
 */
class MRSVSPACEMETADATA_API ConfigurationDataHandler
{
public:
	/**
	 * Creates new data handler and loads metadata initially from storage
	 *
	 * @param InPath The file path to the storage metadata file
	 */
	ConfigurationDataHandler(FString InPath);
	
	/**
	 * Loads the metadata from storage
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
	 * The file path to the storage file
	 */
	FString StoragePath;

	/**
	 * Currently cached metadata
	 */
	TSharedPtr<FEnvironment> EnvironmentData = MakeShareable(new FEnvironment());
};
