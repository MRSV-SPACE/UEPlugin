
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
	uint64 counter = 0;
    for (FControl& Control : *ControlList)
	{
		PropertyConfigFormContainer->AddSlot()
		.Padding(0, 0.0f, 0.0f, 5.0f)
		[
			SNew(SControlConfigurationWidget)
			.ControlData(&Control)
			.OnRemove_Lambda([this, counter](TSharedRef<SWidget> FormWidget) {
				// Remove item in control list
				ControlList->RemoveAt(counter);
				// Remove widget
				PropertyConfigFormContainer->RemoveSlot(FormWidget);
			})
		];
    	counter++;
	}
}

FReply SControlListFormWidget::AddPropertyConfigurationForm() const
{
	//Create new control object
	FControl Control = FControl();
	//Add Control to list
	int Index = ControlList->Add(Control);
	//Add slot in configuration form
	PropertyConfigFormContainer->AddSlot()
		.Padding(0, 0.0f, 0.0f, 5.0f)
		[
			SNew(SControlConfigurationWidget)
			.ControlData(&Control)
			.OnRemove_Lambda([this, Index](TSharedRef<SWidget> FormWidget) {
				// Remove item in control list
				ControlList->RemoveAt(Index);
				// Remove widget
				PropertyConfigFormContainer->RemoveSlot(FormWidget);
			})
		];
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
