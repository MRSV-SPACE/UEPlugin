#pragma once

#include "ConfigurationData.h"

/**
 * A widget for showing dynamic forms for the control details
 * of each control type. 
 */
class SDynamicControlDetailsForm : public SCompoundWidget
{
public:
	SLATE_BEGIN_ARGS(SDynamicControlDetailsForm) {}
		// The pointer to the control details storage object
		SLATE_ATTRIBUTE(FControlDetails*, ControlDetails)
		// The initial type selected
		SLATE_ARGUMENT(EControlType*, InitalType)
	SLATE_END_ARGS()

	void Construct(const FArguments& InArgs);

	/**
	 * Change the control type selected
	 * 
	 * @param InType The new type to select
	 */
	void SetType(EControlType* InType);
private:
	/**
	 * The pointer to the control details storage object
	 */
	FControlDetails* ControlDetails = nullptr;

	/**
	 * The container widget for the inputs of slider contols
	 */
	TSharedPtr<SVerticalBox> SliderInputs;

	/**
	 * The container widget for the inputs of a container control
	 */
	TSharedPtr<SVerticalBox> ContainerInputs;
	
	/**
     * The container widget for the inputs of an option control
     */
	TSharedPtr<SVerticalBox> OptionInputs;
};
