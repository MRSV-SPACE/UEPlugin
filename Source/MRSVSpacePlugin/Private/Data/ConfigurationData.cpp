#include "Data/ConfigurationData.h"

bool FPreset::operator==(const FPreset& Other) const
{
	return Id == Other.Id;
}

uint32 GetTypeHash(const FPreset& Preset)
{
	return FCrc::MemCrc32(&Preset.Id, sizeof(Preset.Id));
}

bool FControl::operator==(const FControl& Other) const
{
	return Id == Other.Id;
}

uint32 GetTypeHash(const FControl& Control)
{
	return FCrc::MemCrc32(&Control.Id, sizeof(FString));
}
