#pragma once

class MRSVSPACECOMPONENTS_API FMRSVSpaceComponentsModule : public IModuleInterface
{
	virtual void StartupModule() override;
	virtual void ShutdownModule() override;
};
