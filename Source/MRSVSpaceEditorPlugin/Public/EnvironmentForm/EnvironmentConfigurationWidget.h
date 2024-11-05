#pragma once
#include "ConfigurationData.h"

DECLARE_DELEGATE(FOnSave)

/**
 * The main widget for the environment configuration form
 */
class SEnvironmentConfigurationWidget : public SCompoundWidget
{
public:
	SLATE_BEGIN_ARGS(SEnvironmentConfigurationWidget) {}
		// Parameter for ptr to environment data
		SLATE_ARGUMENT(TSharedPtr<FEnvironment>, EnvironmentData)

		// Event for handling saving
		SLATE_EVENT(FOnSave, OnSave)
	SLATE_END_ARGS()

	void Construct(const FArguments& InArgs);
private:
	/**
	 * The environment data
	 */
	TSharedPtr<FEnvironment> EnvironmentData;

	/**
	 * The delegate for handling saving
	 */
	FOnSave OnSaveDelegate;
	
	/**
	 * The default font style
	 */
	FSlateFontInfo DefaultFont = FCoreStyle::GetDefaultFontStyle("Regular", 10);
};
