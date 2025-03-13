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
		// Pointer to the list of default controls
		SLATE_ARGUMENT(TSharedPtr<TArray<FControl>>, DefaultControls)
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
	 * Pointer to the list of all the default controls
	 */
	TSharedPtr<TArray<FControl>> DefaultControls;
	
	/**
	 * The default font style
	 */
	FSlateFontInfo DefaultFont = FCoreStyle::GetDefaultFontStyle("Regular", 10);

	/**
	 * 
	 */
	TSharedPtr<STextBlock> SaveStateText;
	
	/**
	 * 
	 */
	void ChangeSaveStateText(bool SaveState);

	EActiveTimerReturnType HandleHideTextTimer(double InCurrentTime, float InDeltaTime);
};
