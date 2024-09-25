#include "Form/ConfigurationWidget.h"
#include "Components/MRSVControlsComponent.h"
#include "Form/PropertyConfigurationWidget.h"
#include "Form/HelperWidgets/StaticConfigurationInput.h"
#include "Widgets/Text/SMultiLineEditableText.h"

void SConfigurationWidget::Construct(const FArguments& InArgs)
{
	//Store DataHandler
	DataHandler = InArgs._DataHandler;
	//Construct basic view
	ChildSlot
	.Padding(5.0f)
	[
		/* FORM CONTAINER */
		SNew( SVerticalBox )
			/* TITLE */
			+ SVerticalBox::Slot()
			.HAlign(HAlign_Left)
			.VAlign(VAlign_Top)
			.Padding(0.0f, 0.0f, 0.0f, 10.0f)
			.AutoHeight()
			[
				SNew(SHorizontalBox)
				+ SHorizontalBox::Slot()
				.HAlign(HAlign_Left)
				.VAlign(VAlign_Center)
				.AutoWidth()
				[
					SNew(STextBlock)
					.Text(FText::FromName("MRSV*Space Plugin"))
					.Font(FCoreStyle::GetDefaultFontStyle("Regular", 17))
				]
				+ SHorizontalBox::Slot()
				.HAlign(HAlign_Right)
				.VAlign(VAlign_Center)
				.Padding(5.0f, 0.0f)
				.AutoWidth()
				[
					SNew( SButton )
					.Text(FText::FromString("Save"))
					.ButtonColorAndOpacity(FLinearColor::Green)
					.ToolTipText(FText::FromString("Save the current environment data"))
					.OnClicked_Lambda([this]()
					{
						DataHandler->Save();
						return FReply::Handled();
					})
				]
			]
			/* STATIC INPUTS */
			+ SVerticalBox::Slot()
			.VAlign(VAlign_Top)
			.AutoHeight()
			[
				SNew(SVerticalBox)
				+ SVerticalBox::Slot()
				[
					SNew( SStaticConfigurationInput )
					.LabelText(FText::FromString("Environment Name"))
					[
						SNew( SEditableText )
						.HintText(FText::FromString("Environment Name..."))
						.Text(FText::FromString(DataHandler->GetEnvironment()->Name))
						.OnTextChanged_Lambda([this](const FText& NewText)
						{
							DataHandler->GetEnvironment()->Name = NewText.ToString();
						})
					]
				]
				+ SVerticalBox::Slot()
				.AutoHeight()
				[
					SNew( SStaticConfigurationInput )
					.LabelText(FText::FromString("Version"))
					[
						SNew(SVerticalBox)
						+ SVerticalBox::Slot()
						.VAlign(VAlign_Top)
						.Padding(0.0f, 0.0f, 0.0f, 5.0f)
						[
							SNew( SEditableText )
							.HintText(FText::FromString("Version Tag..."))
							.Text(FText::FromString(DataHandler->GetEnvironment()->Version.Tag))
							.OnTextChanged_Lambda([this](const FText& NewText)
							{
								DataHandler->GetEnvironment()->Version.Tag = NewText.ToString();
							})
						]
						+SVerticalBox::Slot()
						.VAlign(VAlign_Top)
						.AutoHeight()
						[
							SNew( SMultiLineEditableText )
							.AutoWrapText(true)
							.HintText(FText::FromString("Version Revision..."))
							.Text(FText::FromString(DataHandler->GetEnvironment()->Version.Revision))
							.OnTextChanged_Lambda([this](const FText& NewText)
							{
								DataHandler->GetEnvironment()->Version.Revision = NewText.ToString();
							})
						]
					]
				]
			]
			+ SVerticalBox::Slot()
			.AutoHeight()
			.Padding(0.0f, 0.0f, 0.0f, 5.0f)
			[
				SNew( SSeparator )
			]
			/* ENV PROPERTY DYNAMIC FORM CONTAINER */
			+ SVerticalBox::Slot()
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
						.Font(FCoreStyle::GetDefaultFontStyle("Regular", 10))
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
						.OnClicked_Raw(this, &SConfigurationWidget::AddPropertyConfigurationForm)
					]
				]
				/* SCROLL CONTAINER FOR ENV PROPERTY FORMS */
				+SVerticalBox::Slot()
				.VAlign(VAlign_Top)
				.Padding(5.0f)
				[
					SAssignNew( PropertyConfigFormContainer, SScrollBox )
				]
			]
	];
	//Construct Property Forms from stored metadata
	for (int32 Index = 0; Index < DataHandler->GetEnvironment()->Controls.Num(); ++Index)
	{
		PropertyConfigFormContainer->AddSlot()
		.Padding(0, 0.0f, 0.0f, 5.0f)
		[
			SNew(SPropertyConfigurationWidget)
			.ParentBox(PropertyConfigFormContainer)
			//Pass ref to control data
			.ControlDataRef(MakeShareable(&DataHandler->GetEnvironment()->Controls[Index]))
			.DataHandler(DataHandler)
		];
	}
}

FReply SConfigurationWidget::AddPropertyConfigurationForm() const
{
	//Add slot in configuration form
	PropertyConfigFormContainer->AddSlot()
		.Padding(0, 0.0f, 0.0f, 5.0f)
		[
			SNew(SPropertyConfigurationWidget)
			.ParentBox(PropertyConfigFormContainer)
			//Create new control in metadata and pass ID
			.ControlDataRef(MakeShareable(&DataHandler->GetEnvironment()->Controls.Add_GetRef(FControl())))
			.DataHandler(DataHandler)
		];
	//Add available control to the environment
	UMRSVControlsComponent::AddAvailableControl(FName("Test Config"));
	return FReply::Handled();
}