#pragma once

#include "DynamicControlDetailsForm.h"
#include "HelperWidgets/SearchableDropdownWidget.h"

DECLARE_DELEGATE_OneParam(FOnRemoveCallback, TSharedRef<SWidget>)

/**
 * The Control Configuration Widget to configure an environment control
 */
class SPresetConfigurationWidget : public SCompoundWidget
{
public:
	SLATE_BEGIN_ARGS(SPresetConfigurationWidget) {}
		// Reference to the control object
		SLATE_ARGUMENT(FPreset*, PresetData)
		SLATE_ARGUMENT(TArray<FControl>*, Controls)
		// Callback delegate for removing the control
		SLATE_EVENT(FOnRemoveCallback, OnRemove)
	SLATE_END_ARGS()

	void Construct(const FArguments& InArgs);

private:
	/**
	 * Once the construction of the slate is completed
	 */
	void OnConstructCompleted();
	
	/**
	 * Add the control to the preset
	 * @param Control Concerned control
	 */
	void AddControl(FControl* Control);
	
	/**
	 * Add the Toggle Control
	 * @param name name of the control
	 */
	void AddToggleControl(::FString name);
	
	/**
	 * Add the slider control (custom widget with a slider and a textbox)
	 * @param name name of the control
	 * @param min min value of the slider
	 * @param max max value of the slider
	 */
	void AddSliderControl(FString name, double min, double max);
	
	/**
	 * Add choice Selector Control (custom widget of searchable dropdown list)
	 * @param name name of the control
	 * @param choices list of choices for the dropdown list
	 */
	void AddChoiceSelectorControl(::FString name, TArray<FString> choices);
	
	/**
	 * Defines the value of the TMap values in the preset
	 * Since the value is a JsonValue, it needs to be define at first what will be there
	 * otherwise it could be a null pointer resulting in a crash
	 * @param Control Concerned control
	 */
	void DefinePresetItemValue(FControl* Control);
	
	
	/**
	 * The pointer to the control data object
	 */
	FPreset* PresetData = nullptr;

	/*
	 * List of all the controls the user defined in the "configure control" panel 
	 */
	TArray<FControl> ControlsList;

	/*
	 * Control container in the preset 
	 */
	TSharedPtr<SVerticalBox> ControlContainer;

	/**
	 * The default label font
	 */
	FSlateFontInfo LabelFont = FCoreStyle::GetDefaultFontStyle("Regular", 10);

	/**
	 * The default font style
	 */
	FSlateFontInfo DefaultFont = FCoreStyle::GetDefaultFontStyle("Regular", 10);
	
	/**
	 * The callback delegate to remove the current control
	 */
	FOnRemoveCallback RemoveCallback;
	
	/**
	 * Function to handle when the remove button is clicked
	 *
	 * @return Reply for handled events
	 */
	FReply OnRemoveClicked();
};
