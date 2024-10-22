#pragma once

class MRSVSPACEMETADATA_API FMRSVSpaceMetadataModule : public IModuleInterface
{
	virtual void StartupModule() override;
	virtual void ShutdownModule() override;
};
