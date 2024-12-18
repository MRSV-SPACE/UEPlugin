#pragma once

#include "ConfigurationData.h"

/**
 * The widget for the input form of a list of controls
 */
class SPresetListFormWidget : public SCompoundWidget
{
public:
	SLATE_BEGIN_ARGS(SPresetListFormWidget) {}
		// The initial list of presets
		SLATE_ARGUMENT(TArray<FPreset>*, PresetList)
		// The initial list of controls
		SLATE_ARGUMENT(TArray<FControl>*, ControlList)
	SLATE_END_ARGS()

	
	void Construct(const FArguments& InArgs);
	/**
	 * Reload the form and updated the control list for the presets
	 * Same as if the user closed and reopened the form
	 * @return Reply for handled events
	 */
	FReply ReloadForm() const;

	/**
	 * Showing the form as a new popup window
	 *
	 * @param InitalList The initial list of presets
	 * @param InitialControlList The initial list of controls
	 * @param PopupTitle The title of the popup window
	 * @param PopUpSize The size of the popup window
	 * @return The created control list form widget
	 */
	static TSharedRef<SPresetListFormWidget> ShowAsPopup(
		TArray<FPreset>* InitalList,
		TArray<FControl>* InitialControlList,
		FText PopupTitle,
		FVector2D PopUpSize = FVector2D(800, 400));

private:
	/**
	 * The container widget for the control form widgets
	 */
	TSharedPtr<SScrollBox> PropertyConfigFormContainer;

	/**
	 * The internal list of controls
	 */
	TArray<FPreset>* PresetList = nullptr;

	/**
	 * The internal list of controls
	 */
	TArray<FControl>* ControlList = nullptr;

	/**
	 * The default font size
	 */
	FSlateFontInfo DefaultFont = FCoreStyle::GetDefaultFontStyle("Regular", 15);

	/**
	 * Function for adding a property configuration form to the list
	 *
	 * @return The reply if the event is handled (Used for handled events)
	 */
	FReply AddPropertyConfigurationForm() const;

	/**
	 * Function to add every existing preset to the form
	 */
	void AddExistingPreset() const;
};
