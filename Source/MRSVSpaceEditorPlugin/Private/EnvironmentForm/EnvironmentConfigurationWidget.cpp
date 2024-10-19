
#include "EnvironmentForm/EnvironmentConfigurationWidget.h"

#include "ConfigurationDataHandler.h"
#include "EnvironmentForm/ControlListFormWidget.h"
#include "HelperWidgets/StaticConfigurationInput.h"
#include "Widgets/Text/SMultiLineEditableText.h"

void SEnvironmentConfigurationWidget::Construct(const FArguments& InArgs)
{
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
						ConfigurationDataHandler::GetInstance().Save();
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
						.Font(DefaultFont)
						.HintText(FText::FromString("Environment Name..."))
						.Text(FText::FromString(ConfigurationDataHandler::GetInstance().GetEnvironment()->Name))
						.OnTextChanged_Lambda([this](const FText& NewText)
						{
							ConfigurationDataHandler::GetInstance().GetEnvironment()->Name = NewText.ToString();
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
							.Text(FText::FromString(ConfigurationDataHandler::GetInstance().GetEnvironment()->Version.Tag))
							.Font(DefaultFont)
							.OnTextChanged_Lambda([this](const FText& NewText)
							{
								ConfigurationDataHandler::GetInstance().GetEnvironment()->Version.Tag = NewText.ToString();
							})
						]
						+SVerticalBox::Slot()
						.VAlign(VAlign_Top)
						.AutoHeight()
						[
							SNew( SMultiLineEditableText )
							.AutoWrapText(true)
							.Font(DefaultFont)
							.HintText(FText::FromString("Version Revision..."))
							.Text(FText::FromString(ConfigurationDataHandler::GetInstance().GetEnvironment()->Version.Revision))
							.OnTextChanged_Lambda([this](const FText& NewText)
							{
								ConfigurationDataHandler::GetInstance().GetEnvironment()->Version.Revision = NewText.ToString();
							})
						]
					]
				]
			]
			/* Configuration PopUp Buttons */
			+ SVerticalBox::Slot()
			.HAlign(HAlign_Left)
			.VAlign(VAlign_Center)
			.AutoHeight()
			[
				SNew(SButton)
				.Text(FText::FromString("Configure Controls"))
				.OnClicked_Lambda([this]()
				{
					SControlListFormWidget::ShowAsPopup(MakeShareable(&ConfigurationDataHandler::GetInstance().GetEnvironment()->Controls), FText::FromString("Environment Controls"));
					return FReply::Handled();
				})
			]
	];
}