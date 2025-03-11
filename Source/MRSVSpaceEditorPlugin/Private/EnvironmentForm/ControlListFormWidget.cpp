
#include "EnvironmentForm/ControlListFormWidget.h"

#include "EnvironmentForm/ControlConfigurationWidget.h"

void SControlListFormWidget::Construct(const FArguments& InArgs)
{
	//Store params
	DefaultControlsList = InArgs._DefaultControls;
	numberOfDefault = DefaultControlsList->Num();
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
		.isDefault(false)
		.OnRemove_Lambda([this, CurrentControl](TSharedRef<SWidget> FormWidget) {
			// Remove item in control list
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

TSharedRef<SControlListFormWidget> SControlListFormWidget::ShowAsPopup(TArray<FControl>* InitalList, TArray<FControl>* DefaultControlsList, FText PopUpTitle, FVector2D PopUpSize)
{
	//Create widget
	TSharedRef<SControlListFormWidget> PopupContent = SNew(SControlListFormWidget)
		.ControlList(InitalList)
		.DefaultControls(DefaultControlsList);
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

TSharedRef<SControlListFormWidget> SControlListFormWidget::ShowAsPopup(TArray<FControl>* InitalList, FText PopUpTitle, FVector2D PopUpSize)
{
	//Create widget
	TSharedRef<SControlListFormWidget> PopupContent = SNew(SControlListFormWidget)
		.ControlList(InitalList)
		.DefaultControls(nullptr);
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

	//Checks if the default controls are all in the list of controls
	bool bContainsAllDefaults = true;
	for (const FControl& DefaultControl : *DefaultControlsList)
	{
		if (!ControlList->Contains(DefaultControl))
		{
			bContainsAllDefaults = false;
			break; 
		}
	}
	//If the default controls aren't all in the list, recreate the list with first the default controls then the user controls
	if (!bContainsAllDefaults)
	{
		TArray<FControl>* AllControls = new TArray<FControl>();
		AllControls->Append(*DefaultControlsList);
		AllControls->Append(*ControlList);
	
		ControlList-> Empty();
		ControlList-> Append(*AllControls);
		AllControls = nullptr;
	}
	else
	{
		//Set the first controls of the list to default based on the number of default controls
		//(If there's 5 default controls, then the 5 first controls are set to default)
		for (FControl& Control : *ControlList)
		{
			Control.IsDefault = (counter < numberOfDefault);
			counter++;

		}
	}
	
	//Creates all the controls visually
	for (FControl& Control : *ControlList)
	{
		PropertyConfigFormContainer->AddSlot()
		.Padding(0, 0.0f, 0.0f, 5.0f)
		[
			SNew(SControlConfigurationWidget)
			.ControlData(&Control)
			.isDefault(Control.IsDefault)
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
	}
}

