#include "Form/DynamicControlDetailsForm.h"

#include "Form/ControlListFormWidget.h"
#include "Form/HelperWidgets/StaticConfigurationInput.h"
#include "Widgets/Input/SNumericEntryBox.h"

void SDynamicControlDetailsForm::Construct(const FArguments& InArgs)
{
	//Get params
	ControlDetails = InArgs._ControlDetails.Get();
	//Create view
	ChildSlot
	[
		SNew(SVerticalBox)
		//Slot for slider inputs
		+SVerticalBox::Slot()
		.AutoHeight()
		[
			//INPUTS FOR SLIDER TYPES
			SAssignNew(SliderInputs, SVerticalBox)
			.Visibility(EVisibility::Collapsed)
			+SVerticalBox::Slot()
			.AutoHeight()
			[
				SNew(SStaticConfigurationInput)
				.LabelText(FText::FromString("Minimum Value"))
				[
					SNew(SNumericEntryBox<double>)
					.AllowSpin(true)
					.Value_Lambda([this]()
					{
						return ControlDetails->Min;
					})
					.OnValueChanged_Lambda([this](double NewValue)
					{
						ControlDetails->Min = NewValue;
					})
				]
			]
			+SVerticalBox::Slot()
			.AutoHeight()
			[
				SNew(SStaticConfigurationInput)
				.LabelText(FText::FromString("Maximum Value"))
				[
					SNew(SNumericEntryBox<double>)
					.Value_Lambda([this]()
					{
						return ControlDetails->Max;
					})
					.AllowSpin(true)
					.OnValueChanged_Lambda([this](double NewValue)
					{
						ControlDetails->Max = NewValue;
					})
				]
			]
		]
		//Slot for Slider / Joystick inputs
		+SVerticalBox::Slot()
		.AutoHeight()
		[
			//INPUTS FOR SLIDER & JOYSTICK TYPES
			SAssignNew(SliderAndJoyStickInputs, SVerticalBox)
			.Visibility(EVisibility::Collapsed)
			+SVerticalBox::Slot()
			.AutoHeight()
			[
				SNew(SStaticConfigurationInput)
				.LabelText(FText::FromString("Default Value"))
				[
					SNew(SNumericEntryBox<double>)
					.AllowSpin(true)
					.Value_Lambda([this]()
					{
						return ControlDetails->CurrentValue;
					})
					.OnValueChanged_Lambda([this](double NewValue)
					{
						ControlDetails->CurrentValue = NewValue;
					})
				]
			]
		]
		//Slot for Toggle inputs
		+SVerticalBox::Slot()
		.AutoHeight()
		[
			//INPUTS FOR TOGGLE TYPES
			SAssignNew(ToggleInputs, SVerticalBox)
			.Visibility(EVisibility::Collapsed)
			+SVerticalBox::Slot()
			.AutoHeight()
			[
				SNew(SStaticConfigurationInput)
				.LabelText(FText::FromString("Default Value (Toggle)"))
				[
					SNew(SCheckBox)
					.IsChecked(ControlDetails->Toggled)
					.OnCheckStateChanged_Lambda([this](ECheckBoxState NewState)
					{
						ControlDetails->Toggled = NewState == ECheckBoxState::Checked;
					})
				]
			]
		]
		//Slot for Toggle Expanded Options inputs
		+SVerticalBox::Slot()
		.AutoHeight()
		[
			//INPUTS FOR TOGGLE EXPANDED OPTS TYPES
			SAssignNew(ToggleExpandedOptionsInputs, SVerticalBox)
			.Visibility(EVisibility::Collapsed)
			+SVerticalBox::Slot()
			.AutoHeight()
			[
				SNew(SStaticConfigurationInput)
				.LabelText(FText::FromString("Options"))
				[
					/* Configuration PopUp Buttons */
					SNew(SButton)
					.Text(FText::FromString("Configure Expanded Option Controls"))
					.OnClicked_Lambda([this]()
					{
						//Create popup and add handle event
						SControlListFormWidget::ShowAsPopup(MakeShareable(&ControlDetails->Options), FText::FromString("Expanded Option Controls"));
						return FReply::Handled();
					})
				]
			]
		]
	];
	//Set current type
	SetType(InArgs._InitalType);
}

void SDynamicControlDetailsForm::SetType(EControlType* InType)
{
	//First hide all inputs
	SliderInputs->SetVisibility(EVisibility::Collapsed);
	SliderAndJoyStickInputs->SetVisibility(EVisibility::Collapsed);
	ToggleInputs->SetVisibility(EVisibility::Collapsed);
	ToggleExpandedOptionsInputs->SetVisibility(EVisibility::Collapsed);
	//Check if valid type
	if(InType != nullptr)
	{
		switch (*InType)
		{
		case SLIDER_FREE_VERT:
		case SLIDER_FREE_HORIZ:
		case SLIDER_NOTCH_VERT:
		case SLIDER_NOTCH_HORIZ:
			SliderInputs->SetVisibility(EVisibility::Visible);
			break;
		case JOYSTICK:
			SliderInputs->SetVisibility(EVisibility::Visible);
			SliderAndJoyStickInputs->SetVisibility(EVisibility::Visible);
			break;
		case TOGGLE_BTN:
			ToggleInputs->SetVisibility(EVisibility::Visible);
			break;
		case TOGGLE_BTN_EXP_OPT:
			ToggleExpandedOptionsInputs->SetVisibility(EVisibility::Visible);
			break;
		default:
			UE_LOG(LogTemp, Error, TEXT("Invalid Control Type given"));
		}
	}
}
