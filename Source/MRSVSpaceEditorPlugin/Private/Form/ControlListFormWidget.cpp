
#include "Form/ControlListFormWidget.h"

#include "Form/ControlConfigurationWidget.h"

void SControlListFormWidget::Construct(const FArguments& InArgs)
{
	//Store params
	InternalControlList = InArgs._InitialControlList;
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
	for (int32 Index = 0; Index < InternalControlList->Num(); ++Index)
	{
		PropertyConfigFormContainer->AddSlot()
		.Padding(0, 0.0f, 0.0f, 5.0f)
		[
			SNew(SControlConfigurationWidget)
			.ParentBox(PropertyConfigFormContainer)
			//Pass ref to control data
			.ControlDataIndex(Index)
			.ControlList(InternalControlList)
		];
	}
}

FReply SControlListFormWidget::AddPropertyConfigurationForm() const
{
	//Add slot in configuration form
	PropertyConfigFormContainer->AddSlot()
		.Padding(0, 0.0f, 0.0f, 5.0f)
		[
			SNew(SControlConfigurationWidget)
			.ParentBox(PropertyConfigFormContainer)
			//Create new control in metadata and pass ID
			.ControlDataIndex(InternalControlList->Add(FControl()))
			.ControlList(InternalControlList)
		];
	//Add available control to the environment (TODO)
	//UMRSVControlsComponent::AddAvailableControl(FName("Test Config"));
	return FReply::Handled();
}

TSharedRef<SControlListFormWidget> SControlListFormWidget::ShowAsPopup(TSharedPtr<TArray<FControl>> InitalList, FText PopUpTitle, FVector2D PopUpSize)
{
	//Create widget
	TSharedRef<SControlListFormWidget> PopupContent = SNew(SControlListFormWidget)
		.InitialControlList(InitalList);
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
