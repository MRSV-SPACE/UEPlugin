
#include "EnvironmentForm/PresetListFormWidget.h"

#include "EnvironmentForm/PresetConfigurationWidget.h"

void SPresetListFormWidget::Construct(const FArguments& InArgs)
{
	//Store params
	PresetList = InArgs._PresetList;
	ControlList = InArgs._ControlList;
	
	//Construct view
	ChildSlot
	.Padding(10.0f)
	[
		SNew(SVerticalBox)
		/* ENV PROPERTY FORM HEADER CONTAINER */
		+SVerticalBox::Slot()
		.AutoHeight()
		[
			SNew( SHorizontalBox )
			/* ENV PROPERTY FORM TITLE */
			+SHorizontalBox::Slot()
			.HAlign(HAlign_Left)
			.VAlign(VAlign_Center)
			.AutoWidth()
			[
				SNew( STextBlock )
				.Text( FText::FromString("Presets") )
				.Font(DefaultFont)
			]
			/* ENV PROPERTY FORM "ADD" BUTTON */
			+SHorizontalBox::Slot()
			.HAlign(HAlign_Left)
			.VAlign(VAlign_Center)
			.Padding(5.0f, 0.0f)
			.AutoWidth()
			[
				SNew( SButton )
				.Text(FText::FromString("+"))
				.ToolTipText(FText::FromString("Add a new preset to the environment"))
				.OnClicked_Raw(this, &SPresetListFormWidget::AddPropertyConfigurationForm)
			]
			+SHorizontalBox::Slot()
			.HAlign(HAlign_Right)
			.VAlign(VAlign_Center)
			.FillWidth(1.0f)
			+SHorizontalBox::Slot()
			.HAlign(HAlign_Right)
			.VAlign(VAlign_Center)
			.AutoWidth()
			[
				SNew(SButton)
				.Text(FText::FromString("Reload"))
				.ToolTipText(FText::FromString("Reload the form with the currently defined controls "))
				.OnClicked_Raw(this, &SPresetListFormWidget::ReloadForm)
			]
		]
		/* SCROLL CONTAINER FOR ENV PROPERTY FORMS */
		+SVerticalBox::Slot()
		.VAlign(VAlign_Top)
		.Padding(5.0f)
		[
			SAssignNew( PropertyConfigFormContainer, SScrollBox )
		]
	];
	//Construct Property Forms from stored metadata
	AddExistingPreset();
}
/**
 * Add a new preset to the form
 * @return Reply for handled events
 */
FReply SPresetListFormWidget::AddPropertyConfigurationForm() const
{
	//Add preset to list
	int Index = PresetList->Add(FPreset());
	FPreset* CurrentPreset = &(*PresetList)[Index];
	//Add slot in configuration form
	PropertyConfigFormContainer->AddSlot()
	.Padding(0, 0.0f, 0.0f, 5.0f)
	[
		SNew(SPresetConfigurationWidget)
		.PresetData(CurrentPreset)
		.Controls(ControlList)
		.OnRemove_Lambda([this, CurrentPreset](TSharedRef<SWidget> FormWidget) {
			
			// Remove item in preset list
			// This needs to be done this way because when using any of the remove function it makes UE crash (not related to the remove function itself)
			TArray<FPreset> NewPresets;
			for (FPreset& TempPreset : *PresetList)
			{
				if (TempPreset != *CurrentPreset)
				{
					NewPresets.Add(TempPreset);
				}
			}

			PresetList->Empty();
			PresetList->Append(NewPresets);

			// Remove widget
			PropertyConfigFormContainer->RemoveSlot(FormWidget);
		})
	];

	// Reloading the form when adding a new preset prevent bugs when removing them later
	// if the user add a new preset and remove it without closing the form before,
	// the presets aren't properly saved and so the deleted preset will appear again when reopening the form 
	ReloadForm();
	
	return FReply::Handled();
}
/**
 * Reload the form with the current control
 * @return Reply for handled events
 */
FReply SPresetListFormWidget::ReloadForm() const
{
	//Clear all children and add them again (pretty much the same as if the user closed the panel and reopened it) 
	PropertyConfigFormContainer->ClearChildren();
	AddExistingPreset();
	return FReply::Handled();
}

/**
 * Show the form as popup
 * @param InitialList Initial list of Presets
 * @param InitialControlList Initial list of Controls
 * @param PopUpTitle Title of the popup
 * @param PopUpSize Size of the popup
 * @return 
 */
TSharedRef<SPresetListFormWidget> SPresetListFormWidget::ShowAsPopup(TArray<FPreset>* InitialList, TArray<FControl>* InitialControlList, FText PopUpTitle, FVector2D PopUpSize)
{
	//Create widget
	TSharedRef<SPresetListFormWidget> PopupContent = SNew(SPresetListFormWidget)
		.PresetList(InitialList)
		.ControlList(InitialControlList);
	//Create popup window
	FSlateApplication::Get().AddWindow(
		SNew(SWindow)
		.Title(PopUpTitle)
		.ClientSize(PopUpSize)
		.SizingRule(ESizingRule::UserSized)
		[
			PopupContent
		]);
	//Return Ref to Content
	return PopupContent;
}

/**
 * Add all the existing presets
 */
void SPresetListFormWidget::AddExistingPreset() const
{
	uint64 counter = 0;
	for (FPreset& Preset : *PresetList)
	{
		PropertyConfigFormContainer->AddSlot()
		.Padding(0, 0.0f, 0.0f, 5.0f)
		[
			SNew(SPresetConfigurationWidget)
			.PresetData(&Preset)
			.Controls(ControlList)
			.OnRemove_Lambda([this, Preset](TSharedRef<SWidget> FormWidget) {
				// Remove item in preset list
				// This needs to be done this way because when using any of the remove function it makes UE crash (not related to the remove function itself)
				TArray<FPreset> NewPresets;
				for (FPreset& TempPreset : *PresetList)
				{
					if (TempPreset != Preset)
					{
						NewPresets.Add(TempPreset);
					}
				}

				PresetList->Empty();
				PresetList->Append(NewPresets);
				// Remove widget
				PropertyConfigFormContainer->RemoveSlot(FormWidget);
			})
		];
		counter++;
	}
}
