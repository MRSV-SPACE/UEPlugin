#pragma once

#include "ConfigurationData.h"

class SDynamicControlDetailsForm : public SCompoundWidget
{
public:
	SLATE_BEGIN_ARGS(SDynamicControlDetailsForm) {}
		SLATE_ATTRIBUTE(FControlDetails*, ControlDetails)
		SLATE_ARGUMENT(EControlType*, InitalType)
	SLATE_END_ARGS()

	void Construct(const FArguments& InArgs);
	void SetType(EControlType* InType);
private:
	FControlDetails* ControlDetails = nullptr;
	
	TSharedPtr<SVerticalBox> SliderInputs;
	TSharedPtr<SVerticalBox> SliderAndJoyStickInputs;
	TSharedPtr<SVerticalBox> ToggleInputs;
	TSharedPtr<SVerticalBox> ToggleExpandedOptionsInputs;
};
