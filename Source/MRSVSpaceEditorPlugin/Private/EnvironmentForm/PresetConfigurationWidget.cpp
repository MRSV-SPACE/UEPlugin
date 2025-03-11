#include "EnvironmentForm/PresetConfigurationWidget.h"

#include "EnvironmentForm/PresetSliderControlWidget.h"
#include "HelperWidgets/SearchableDropdownWidget.h"
#include "HelperWidgets/StaticConfigurationInput.h"

//Define rounded border for widget
FSlateBrush* RoundedBrush = new FSlateBrush();

void SPresetConfigurationWidget::Construct(const FArguments& InArgs)
{
	//Store params
	PresetData = InArgs._PresetData;
	ControlsList = *InArgs._Controls;
	RemoveCallback = InArgs._OnRemove;

	//Define the RoundedBrush
	RoundedBrush->TintColor = FLinearColor(0.3f, 0.3f, 0.3f, 0.1f);
	RoundedBrush->DrawAs = ESlateBrushDrawType::Type::RoundedBox;
	RoundedBrush->OutlineSettings.CornerRadii = FVector4(8.0f, 8.0f, 8.0f, 8.0f);
	RoundedBrush->OutlineSettings.RoundingType = ESlateBrushRoundingType::FixedRadius;
	
	//Construct basic view
	ChildSlot
	[
		SNew(SBorder)
		.BorderImage(RoundedBrush)
		.Padding(10.0f)
		[
			SNew(SVerticalBox)
			+SVerticalBox::Slot()
			.HAlign(HAlign_Fill)
			.VAlign(VAlign_Center)
			.AutoHeight()
			.Padding(5.0f)
			[
				SNew(SHorizontalBox)
				+SHorizontalBox::Slot()
				.HAlign(HAlign_Fill)
				.VAlign(VAlign_Center)
				.Padding(0.0f, 0.0f, 5.0f, 0.0f)
				.FillWidth(1.0f)
				[
					SNew(SBorder)
					.BorderImage(RoundedBrush)
					.Padding(5.0f)
					[
						SNew( SEditableText )
						.Font(DefaultFont)
						.HintText(FText::FromString("Preset Name..."))
						.Text(FText::FromString(PresetData->Name))
						.OnTextChanged_Lambda([this](const FText& NewText)
						{
							PresetData->Name = NewText.ToString();
						})
					]
				]
				+SHorizontalBox::Slot()
				.HAlign(HAlign_Right)
				.VAlign(VAlign_Center)
				.AutoWidth()
				[
					//Remove button
					SNew(SButton)
					.Text(FText::FromString("x"))
					.ButtonColorAndOpacity(FLinearColor::Red)
					.ForegroundColor(FLinearColor::White) 
					.OnClicked(this, &SPresetConfigurationWidget::OnRemoveClicked)
				]
			]
			
			+SVerticalBox::Slot()
			.HAlign(HAlign_Fill)
			.VAlign(VAlign_Center)
			.AutoHeight()
			.Padding(5.0f)
			[
				//Control Container
				SAssignNew(ControlContainer,SVerticalBox)
			]
		]
	];
	OnConstructCompleted();
}

/**
 *Once Construct is completed
 */
void SPresetConfigurationWidget::OnConstructCompleted()
{
	for (int i = 0; i < ControlsList.Num(); i++)
	{
		AddControl(&ControlsList[i]);
	}
}

/**
 * Add the control in the control container
 * @param Control : the concern control
 */
void SPresetConfigurationWidget::AddControl(FControl* Control)
{
	if (ControlContainer.IsValid())
	{
		DefinePresetItemValue(Control);

		//get control details
		FString controlName = Control->Name;
		double controlMin = Control->Details.Min;
		double controlMax = Control->Details.Max;
		TArray<FString> controlChoices = Control->Details.Options;

		//Switch on the type of the control to add the control with the correct value modifier
		switch(Control->Type.GetValue())
		{
			case SLIDER_FREE_HORIZONTAL:
				AddSliderControl(controlName, controlMin, controlMax);
				break;
			case SLIDER_FREE_VERTICAL:
				AddSliderControl(controlName, controlMin, controlMax);
				break;
			case SLIDER_NOTCH_HORIZONTAL:
				AddSliderControl(controlName, controlMin, controlMax);
				break;
			case SLIDER_NOTCH_VERTICAL:
				AddSliderControl(controlName, controlMin, controlMax);
				break;
			case JOYSTICK:
				//NOTHING TO DO
				UE_LOG(LogTemp,Warning,TEXT("Joystick hasn't been implemented yet (nothing to do)"));
				break;
			case TOGGLE:
				AddToggleControl(controlName);
				break;
			case CONTAINER:
				//NOTHING TO DO
				UE_LOG(LogTemp,Warning,TEXT("Container hasn't been implemented (nothing to do)"));
				break;
			case CHOICE:
				if (controlChoices.Num()>0)
				{
					AddChoiceSelectorControl(controlName,controlChoices);
				}
				break;
			default:
				UE_LOG(LogTemp,Warning,TEXT("Preset Configuration Widget::AddControl() Switch went in default. something went wrong !"));
				break;
		}
	}
}
/**
 * Add the Toggle control with a toggle button
 * @param name name of the control
 */
void SPresetConfigurationWidget::AddToggleControl(FString name)
{
	ECheckBoxState currentState = ECheckBoxState::Unchecked; 

	//Check if a value already exists in the preset data
	if (TSharedPtr<FJsonValue>* FoundValue = PresetData->Values.Find(name))
	{
		if ((*FoundValue)->Type == EJson::Boolean)
		{
			//defines state from preset data
			currentState = (*FoundValue)->AsBool() ? ECheckBoxState::Checked : ECheckBoxState::Unchecked;
		}
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("Key '%s' not found in Values."), *name);
	}
	
	ControlContainer->AddSlot()
	.Padding(0.0f,5.0f)
	[
		SNew(SStaticConfigurationInput)
		.LabelText(FText::FromString(name))
		[
			SNew(SCheckBox)
			.IsChecked(currentState)
			.OnCheckStateChanged_Lambda([this, name](ECheckBoxState NewState) {
				TSharedPtr<FJsonValue> newValue;

				//Adapt the state of the checkbox to a FJsonValueBoolean
				bool bNewValue = (NewState == ECheckBoxState::Checked);
				newValue = MakeShared<FJsonValueBoolean>(FJsonValueBoolean(bNewValue));

				//Updates the preset data
				PresetData->Values.Add(name, newValue);

			})
		]
	];
}

/**
 * Add the slider control. Custom widget with a slider and a textbox
 * @param name name of the control
 * @param minValue min value of the slider
 * @param maxValue max value of the slider
 */
void SPresetConfigurationWidget::AddSliderControl(FString name, double minValue, double maxValue)
{
	ControlContainer->AddSlot()
	.Padding(0.0f,5.0f)
	[
		SNew(SStaticConfigurationInput)
		.LabelText(FText::FromString(name))
		[
			SNew(SPresetSliderControlWidget)
			.MinValue(minValue)
			.MaxValue(maxValue)
			.PresetData(PresetData)
			.PresetValueName(name)
		]
	];
}
/**
 * Add the choice selector Control (custom widget (searchable dropdown list) 
 * @param name name of the control
 * @param choices list of choices defined 
 */
void SPresetConfigurationWidget::AddChoiceSelectorControl(FString name, TArray<FString> choices)
{
	// Convert options to an array of shared pointer so the choices can be used for the dropdown widget
	TArray<TSharedPtr<SSearchableDropdownWidget<FString>::FDropdownItem>> adaptedChoices;
	for (const FString& choice : choices)
	{
		SSearchableDropdownWidget<FString>::FDropdownItem item;
		item.Name = choice;
		item.Key = choice;
		adaptedChoices.Add(MakeShared<SSearchableDropdownWidget<FString>::FDropdownItem>(item));
	}

	// Defines the default value for the dropdown (if the list is empty, null pointer) 
	TSharedPtr<SSearchableDropdownWidget<FString>::FDropdownItem> currentValue = adaptedChoices.Num() > 0 ? adaptedChoices[0] : nullptr;

	// Get the value from the preset data
	if (TSharedPtr<FJsonValue>* FoundValue = PresetData->Values.Find(name))
	{
		if ((*FoundValue)->Type == EJson::String) // Check if it's a string 
		{
			FString StringValue = (*FoundValue)->AsString();
			for (const auto& Choice : adaptedChoices)
			{
				if (Choice->Name == StringValue) // Associate the string to a value
				{
					currentValue = Choice;
					break;
				}
			}
		}
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("Key '%s' not found in Values."), *name);
	}
	
	ControlContainer->AddSlot()
	.Padding(0.0f, 5.0f)
	[
			SNew(SStaticConfigurationInput)
			.LabelText(FText::FromString(name))
			[
				SNew(SSearchableDropdownWidget<FString>)
				.OptionsSource(&adaptedChoices)
				.SelectedOption(currentValue)
				.OnSelectionChanged_Lambda([this, name](TSharedPtr<FString> NewSelection)
				{
					//Updates the value in the preset data (covert the NewSelection to an FJsonValue)
					FString SelectedValue = *NewSelection;
					TSharedPtr<FJsonValue> SelectionValue = MakeShared<FJsonValueString>(SelectedValue);
					PresetData->Values.Add(name, SelectionValue);
				})
			]
	];
}

/**
 * Removes the preset from the list 
 * @return Reply for handled events
 */
FReply SPresetConfigurationWidget::OnRemoveClicked()
{
	if (RemoveCallback.ExecuteIfBound(AsShared()))
	{
		return FReply::Handled();
	}
	UE_LOG(LogTemp, Warning, TEXT("No callback registred for removing control"))
	return FReply::Unhandled();
}

/**
 * Define a default value for the preset data
 * This needs to be done otherwise the value could be a null pointer resulting in a crash
 * Also since the FJsonValue depends on the type of the control, this needs to be define in the first place 
 * @param Control Concerned control
 */
void SPresetConfigurationWidget::DefinePresetItemValue(FControl* Control)
{
	//Default values 
	double averageValue = (Control->Details.Min+Control->Details.Max)/2;
	TSharedPtr<FJsonValue> defaultValue;

	//Change the value of defaultValue depending on the control type
	switch(Control->Type.GetValue())
	{
	case SLIDER_FREE_HORIZONTAL:
		defaultValue = MakeShared<FJsonValueNumber>(FJsonValueNumber(averageValue));
		break;
	case SLIDER_FREE_VERTICAL:
		defaultValue = MakeShared<FJsonValueNumber>(FJsonValueNumber(averageValue));
		break;
	case SLIDER_NOTCH_HORIZONTAL:
		defaultValue = MakeShared<FJsonValueNumber>(FJsonValueNumber(averageValue));
		break;
	case SLIDER_NOTCH_VERTICAL:
		defaultValue = MakeShared<FJsonValueNumber>(FJsonValueNumber(averageValue));
		break;
	case JOYSTICK:
		//TODO: See what will need to be implemented for the joystick
			break;
	case TOGGLE:
		defaultValue = MakeShared<FJsonValueBoolean>(FJsonValueBoolean(false));
		break;
	case CHOICE:
		//disable the possibly to define a default value in case there's no options in the list 
		if (Control->Details.Options.Num() >0)
		{
			defaultValue = MakeShared<FJsonValueString>(FJsonValueString(Control->Details.Options[0]));
		}
		break;

	case CONTAINER:
		UE_LOG(LogTemp,Warning,TEXT("Tried to define an ItemValue for the container which isn't possible."));
		break;
	default:
		//Shouldn't go through here
		UE_LOG(LogTemp,Warning,TEXT("Preset Configuration Widget::DefinePresetItemValue() Switch went in default. something went wrong !"));
		break;
	}

	FString controlName = Control->Name;

	//Set the default value of the preset data
	if (PresetData->Values.Find(controlName) == nullptr)
	{
		PresetData->Values.Add(controlName, defaultValue);
	}
}


