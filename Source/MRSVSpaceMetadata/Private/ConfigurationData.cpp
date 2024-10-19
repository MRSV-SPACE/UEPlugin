#include "ConfigurationData.h"

bool FPreset::operator==(const FPreset& Other) const
{
	return Id == Other.Id;
}

bool FControl::operator==(const FControl& Other) const
{
	return Id == Other.Id;
}

bool FEnvironment::operator==(const FEnvironment& Other) const
{
	return Id.Equals(Other.Id);
}

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