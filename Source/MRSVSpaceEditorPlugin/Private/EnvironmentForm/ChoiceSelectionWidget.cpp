#include "EnvironmentForm/ChoiceSelectionWidget.h"

#include "Components/CheckBox.h"

void SChoiceSelectionWidget::Construct(const FArguments& InArgs)
{
	//Get params
	ChoiceName = InArgs._ChoiceName;
	ChoiceList = InArgs._ChoiceList;
	CheckStatus = InArgs._IsChecked;
	
	//Define rounded border for tag widget
	FSlateBrush* RoundedBrushTag = new FSlateBrush();
	RoundedBrushTag->TintColor = FLinearColor(0.3f, 0.3f, 0.3f, 0.3f);
	RoundedBrushTag->DrawAs = ESlateBrushDrawType::Type::RoundedBox;
	RoundedBrushTag->OutlineSettings.CornerRadii = FVector4(8.0f, 8.0f, 8.0f, 8.0f);
	RoundedBrushTag->OutlineSettings.RoundingType = ESlateBrushRoundingType::FixedRadius;

	//Create view
	ChildSlot
	[
		SNew(SBorder)
		.BorderImage(RoundedBrushTag)
		.Padding(5.0f, 3.0f)
		[
			SNew(SHorizontalBox)
			+ SHorizontalBox::Slot()
			.AutoWidth()
			.VAlign(VAlign_Center)
			.Padding(0.0f, 0.0f, 4.0f, 0)
			[
				//STextBlock with the name of the choice
				SNew(STextBlock)
				.Text(FText::FromString(ChoiceName))
			]
			+SHorizontalBox::Slot()
			.VAlign(VAlign_Center)
			.AutoWidth()
			[
				SNew(SCheckBox)
				//depending on the given state, changes the check status 
				.IsChecked(CheckStatus ? ECheckBoxState::Checked : ECheckBoxState::Unchecked)
				.OnCheckStateChanged(this, &SChoiceSelectionWidget::OnStateChange)
			]
		]
	];

}

/**
 * Function called when the check state is changed
 * Add or remove the choice from the list of checked choices
 * @param NewState New state of the checkbox
 */
void SChoiceSelectionWidget::OnStateChange(ECheckBoxState NewState)
{
	if (NewState == ECheckBoxState::Checked)
	{
		ChoiceList->AddUnique(ChoiceName);
	}
	else
	{
		ChoiceList->Remove(ChoiceName);
	}
	
	//Sort the list to ensure every item is always at the same place
	ChoiceList->Sort();
}
