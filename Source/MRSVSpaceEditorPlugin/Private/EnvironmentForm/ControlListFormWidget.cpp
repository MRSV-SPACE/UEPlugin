
#include "EnvironmentForm/ControlListFormWidget.h"

#include "EnvironmentForm/ControlConfigurationWidget.h"

void SControlListFormWidget::Construct(const FArguments& InArgs)
{
	//Store params
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
				.Text( FText::FromString("Environment Properties") )
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
				.ToolTipText(FText::FromString("Add a new property to the environment"))
				.OnClicked_Raw(this, &SControlListFormWidget::AddPropertyConfigurationForm)
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
	AddExistingControls();
}

FReply SControlListFormWidget::AddPropertyConfigurationForm() const
{
	//Add Control to list
	int Index = ControlList->Add(FControl());
	FControl* CurrentControl = &(*ControlList)[Index];
	//Add slot in configuration form
	PropertyConfigFormContainer->AddSlot()
	.Padding(0, 0.0f, 0.0f, 5.0f)
	[
		SNew(SControlConfigurationWidget)
		.ControlData(CurrentControl)
		.OnRemove_Lambda([this, CurrentControl](TSharedRef<SWidget> FormWidget) {
			//Remove item in the control list
			// This needs to be done this way because when using any of the remove function it makes UE crash (not related to the remove function itself)
			TArray<FControl> NewControls;
			for (FControl& TempControl : *ControlList)
			{
				if (TempControl != *CurrentControl)
				{
					NewControls.Add(TempControl);
				}
			}

			ControlList->Empty();
			ControlList->Append(NewControls);
			// Remove widget
			PropertyConfigFormContainer->RemoveSlot(FormWidget);
		})
	];

	//Reload the form so it's fully updated before removing anything
	ReloadForm();
	
	//Add available control to the environment (TODO)
	//UMRSVControlsComponent::AddAvailableControl(FName("Test Config"));
	return FReply::Handled();
}

TSharedRef<SControlListFormWidget> SControlListFormWidget::ShowAsPopup(TArray<FControl>* InitalList, FText PopUpTitle, FVector2D PopUpSize)
{
	//Create widget
	TSharedRef<SControlListFormWidget> PopupContent = SNew(SControlListFormWidget)
		.ControlList(InitalList);
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

void SControlListFormWidget::ReloadForm() const
{
	PropertyConfigFormContainer->ClearChildren();
	AddExistingControls();
}

void SControlListFormWidget::AddExistingControls() const
{
	uint64 counter = 0;
	for (FControl& Control : *ControlList)
	{
		PropertyConfigFormContainer->AddSlot()
		.Padding(0, 0.0f, 0.0f, 5.0f)
		[
			SNew(SControlConfigurationWidget)
			.ControlData(&Control)
			.OnRemove_Lambda([this, Control](TSharedRef<SWidget> FormWidget) {
				// Remove item in control list
				TArray<FControl> NewControls;
				for (FControl& TempControl : *ControlList)
				{
					if (TempControl != Control)
					{
						NewControls.Add(TempControl);
					}
				}
				
				ControlList->Empty();
				ControlList->Append(NewControls);
				
				// Remove widget
				PropertyConfigFormContainer->RemoveSlot(FormWidget);
			})
		];
		counter++;
	}
}
