#pragma once
#include "ConfigurationData.h"
#include "Widgets/Input/SSlider.h"

class SPresetSliderControlWidget : public SCompoundWidget
{
public:
	SLATE_BEGIN_ARGS(SPresetSliderControlWidget) {}
		//min value of the slider
		SLATE_ARGUMENT(double, MinValue)
		//max value of the slider
		SLATE_ARGUMENT(double, MaxValue)
		//preset data
		SLATE_ARGUMENT(FPreset*, PresetData)
		//Name of the value that needs to be change in the preset
		SLATE_ARGUMENT(FString, PresetValueName)
	SLATE_END_ARGS()
	
	void Construct(const FArguments& InArgs);

	/**
	 * slider value
	 * Default value is average of min and max value
	 */
	double sliderValue = (minValue+maxValue)/2;
	
private:
	/**
	 * min value of the slider
	 * Default is 0 (even though it should always be defined in the Configure Control panel)
	 */
	double minValue = 0;
	
	/**
	 * max value of the slider
	 * Default is 1 (even though it should always be defined in the Configure Control panel)
	 */
	double maxValue = 1;

	/**
	 * Preset Data
	 */
	FPreset* PresetData = nullptr;

	/**
	 * Name of the value to be changed in the preset date (Key of the TMap Values in the preset data) 
	 */
	FString valueName;
	
	/**
	 * The default font style
	 */
	FSlateFontInfo SliderTextValueFont = FCoreStyle::GetDefaultFontStyle("Regular", 8);

	/**
	 * pointer to the slider
	 */
	TSharedPtr<SSlider> slider;

	/**
	 * pointer to the textbox
	 */
	TSharedPtr<SEditableTextBox> ETBoxSliderValue ;
};
