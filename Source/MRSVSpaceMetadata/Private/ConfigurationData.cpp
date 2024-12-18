#include "ConfigurationData.h"

uint32 GetTypeHash(const FPreset& Preset)
{
	return FCrc::MemCrc32(&Preset.Id, sizeof(Preset.Id));
}

uint32 GetTypeHash(const FControl& Control)
{
	return FCrc::MemCrc32(&Control.Id, sizeof(FString));
}

uint32 GetTypeHash(const FEnvironment& Environment)
{
	return FCrc::MemCrc32(&Environment.Id, sizeof(FString));
}