#pragma once
#include "ConfigurationData.h"

/**
 * The main widget for the environment configuration form
 */
class SEnvironmentConfigurationWidget : public SCompoundWidget
{
public:
	SLATE_BEGIN_ARGS(SEnvironmentConfigurationWidget) {}
		// Parameter for ptr to environment data
		SLATE_ARGUMENT(TSharedPtr<FEnvironment>, EnvironmentData)
	SLATE_END_ARGS()

	void Construct(const FArguments& InArgs);
private:
	/**
	 * The environment data
	 */
	TSharedPtr<FEnvironment> EnvironmentData;
	
	/**
	 * The default font style
	 */
	FSlateFontInfo DefaultFont = FCoreStyle::GetDefaultFontStyle("Regular", 10);
};
