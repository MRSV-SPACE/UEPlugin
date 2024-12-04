#include "EnvironmentForm/PresetSliderControlWidget.h"

void SPresetSliderControlWidget::Construct(const FArguments& InArgs)
{
	//Define from Args
	minValue = InArgs._MinValue;
	maxValue = InArgs._MaxValue;
	PresetData = InArgs._PresetData;
	valueName = InArgs._PresetValueName;

	//set the value of sliderValue to the average value of min and max (default value) 
	sliderValue = (minValue+maxValue)/2;

	//Check if preset data already has a value and set the slider value to it
	TSharedPtr<FJsonValue> presetDataValuePtr = *PresetData->Values.Find(valueName);
	double value = presetDataValuePtr->AsNumber();
	if(value < minValue)
	{
		value = minValue;
	}
	else if(value > maxValue)
	{
		value = maxValue;
	}
	sliderValue = value;

	//Construct view
	ChildSlot
	[
		SNew(SHorizontalBox)
		+SHorizontalBox::Slot()
		.AutoWidth()
		.HAlign(HAlign_Left)
		[
			SNew(SBox)
			.WidthOverride(60.0f)  
			[
				SAssignNew(ETBoxSliderValue,SEditableTextBox)
				.Font(SliderTextValueFont)
				.Text(FText::AsNumber(sliderValue))
				.OnTextCommitted_Lambda([this](const FText& Text, ETextCommit::Type CommitType)
				{
					//When text is changed, change the value of the slider
					//in case the text is empty -> min value
					//in case text isn't numeric, check if it contains a ',' (since 8,9 isn't considered a numeric)
					//in case text isn't numeric but doesn't contains a ',' -> min value
					float newValue = minValue;
					if (Text.IsEmpty())
					{
						newValue = minValue;
					}
					else
					{
						if (Text.IsNumeric())
						{
							float textToFloatValue =   FCString::Atof(*Text.ToString());
							newValue = textToFloatValue;
						}
						else
						{
							if (Text.ToString().Contains(","))
							{
								//Manage the text to get the values given with a ',' 
								TArray<FString> stringParts;
								const FString separator = ",";
								Text.ToString().ParseIntoArray(stringParts, *separator);
								float textToFloatValue =   FCString::Atof(*stringParts[0]);
								textToFloatValue += FCString::Atof(*stringParts[1])/FMath::Pow(10.0f,stringParts[1].Len());;
								newValue = textToFloatValue;
							}
							else
							{
								newValue = minValue;
							}
						}
						//Check that the new value isn't out of bounds
						if (newValue <minValue)
						{
							newValue = minValue;
						}
						if (newValue > maxValue)
						{
							newValue = maxValue;
						}
					}
					//set the slider value to the new value and update the preset data
					slider->SetValue(newValue);
					PresetData->Values.Add(valueName,MakeShared<FJsonValueNumber>(newValue));
				
					ETBoxSliderValue->SetText(FText::AsNumber(newValue));
				})
			]
		]
		+SHorizontalBox::Slot()
		.AutoWidth()
		.Padding(5.0f,0.0f,0.0f,0.0f)
		[
			SNew(SBox)
			.WidthOverride(20.0f)  
			[
				SNew(STextBlock)
				.Text(FText::FromString(FString::FromInt(minValue)))
			]
		]
		+SHorizontalBox::Slot()
		[
			SNew(SVerticalBox)
			+SVerticalBox::Slot()
			[
				SAssignNew(slider,SSlider)
				.MinValue(minValue)
				.MaxValue(maxValue)
				.Value(sliderValue)
				.OnValueChanged_Lambda([this](float NewSliderValue)
				{
					// round the value the closest 0.01 value 
					sliderValue = FMath::RoundToFloat(NewSliderValue * 100.0f) / 100.0f;
					//update the value of the preset data
					PresetData->Values.Add(valueName,MakeShared<FJsonValueNumber>(sliderValue));

					// update the text of the textbox
					if (ETBoxSliderValue.IsValid())
					{
						ETBoxSliderValue->SetText(FText::AsNumber(sliderValue));
					}
				})
			]
		]
		+SHorizontalBox::Slot()
		.AutoWidth()
		.Padding(5.0f,0.0f)
		[
			SNew(SBox)
			.WidthOverride(30.0f)  
			[
				SNew(STextBlock)
				.Text(FText::FromString(FString::FromInt(maxValue)))
			]
		]
	];
}
