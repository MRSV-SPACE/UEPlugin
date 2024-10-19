#include "EnvironmentForm/DynamicControlDetailsForm.h"

#include "EnvironmentForm/ControlListFormWidget.h"
#include "HelperWidgets/StaticConfigurationInput.h"
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
		//Slot for Toggle Expanded Options inputs
		+SVerticalBox::Slot()
		.AutoHeight()
		[
			//INPUTS FOR OPTION CONTROL
			SAssignNew(OptionInputs, SVerticalBox)
			.Visibility(EVisibility::Collapsed)
			+SVerticalBox::Slot()
			.AutoHeight()
			[
				SNew(SHorizontalBox)
				+SHorizontalBox::Slot()
				.AutoWidth()
				[
					SNew(SButton)
					.Text(FText::FromString("Configure Options (TODO)"))
				]
			]
		]
		//Slot for Toggle Expanded Options inputs
		+SVerticalBox::Slot()
		.AutoHeight()
		[
			//INPUTS FOR CONTAINER CONTROL
			SAssignNew(ContainerInputs, SVerticalBox)
			.Visibility(EVisibility::Collapsed)
			+SVerticalBox::Slot()
			.AutoHeight()
			[
				SNew(SHorizontalBox)
				+SHorizontalBox::Slot()
				.AutoWidth()
				[
					SNew(SButton)
					.Text(FText::FromString("Configure Controls"))
					.OnClicked_Lambda([this]()
					{
						//Create popup and add handle event
						SControlListFormWidget::ShowAsPopup(MakeShareable(&ControlDetails->Controls), FText::FromString("Configure Dropdown Options"), FVector2D(700, 300));
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
	OptionInputs->SetVisibility(EVisibility::Collapsed);
	ContainerInputs->SetVisibility(EVisibility::Collapsed);
	//Check if valid type
	if(InType != nullptr)
	{
		switch (*InType)
		{
			case SLIDER_FREE_VERTICAL:
			case SLIDER_FREE_HORIZONTAL:
			case SLIDER_NOTCH_VERTICAL:
			case SLIDER_NOTCH_HORIZONTAL:
				SliderInputs->SetVisibility(EVisibility::Visible);
				break;
			case CONTAINER:
				ContainerInputs->SetVisibility(EVisibility::Visible);
				break;
			case CHOICE:
				OptionInputs->SetVisibility(EVisibility::Visible);
				break;
			case JOYSTICK:
			case TOGGLE:
				// No inputs open
				break;
			default:
				UE_LOG(LogTemp, Error, TEXT("Invalid Control Type given"));
		}
	}
}
