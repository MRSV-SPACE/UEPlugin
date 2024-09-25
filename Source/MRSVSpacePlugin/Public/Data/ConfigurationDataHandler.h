#pragma once

#include "ConfigurationData.h"

class ConfigurationDataHandler
{
public:
	void Load();
	void Save();

	FEnvironment* GetEnvironment();
	void SetEnvironment(FEnvironment InEnvironment);
private:
	static FString GetMetaFilePath();
	FEnvironment* EnvironmentData = new FEnvironment();
};
