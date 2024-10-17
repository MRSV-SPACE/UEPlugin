#pragma once

#include "ConfigurationData.h"

class MRSVSPACEMETADATA_API ConfigurationDataHandler
{
public:
	void Save() const;
	FEnvironment* GetEnvironment() const;
	
	static ConfigurationDataHandler& GetInstance();
	
	//Remove copy constructor for singleton pattern
	ConfigurationDataHandler(ConfigurationDataHandler const&) = delete;
private:
	void Load();
	ConfigurationDataHandler();
	static FString GetMetaFilePath();
	FEnvironment* EnvironmentData = new FEnvironment();
};
