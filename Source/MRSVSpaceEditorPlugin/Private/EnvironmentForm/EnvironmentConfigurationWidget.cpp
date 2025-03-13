
#include "EnvironmentForm/EnvironmentConfigurationWidget.h"

#include "ConfigurationDataHandler.h"
#include "EnvironmentForm/ControlListFormWidget.h"
#include "EnvironmentForm/PreviewImageListWidget.h"
#include "EnvironmentForm/PresetListFormWidget.h"
#include "HelperWidgets/StaticConfigurationInput.h"
#include "HelperWidgets/StringSetInputWidget.h"
#include "Widgets/Text/SMultiLineEditableText.h"

void SEnvironmentConfigurationWidget::Construct(const FArguments& InArgs)
{
	// Get parameters
	EnvironmentData = InArgs._EnvironmentData;
	OnSaveDelegate = InArgs._OnSave;
	DefaultControls = InArgs._DefaultControls;
	
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
						if(OnSaveDelegate.ExecuteIfBound())
						{
							return FReply::Handled();
						}
						return FReply::Unhandled();
					})
				]
			]
			/* STATIC INPUTS */
			+ SVerticalBox::Slot()
			.VAlign(VAlign_Top)
			.Padding(0.0f, 0.0f, 0.0f, 5.0f)
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
						.Text(FText::FromString(EnvironmentData->Name))
						.OnTextChanged_Lambda([this](const FText& NewText)
						{
							EnvironmentData->Name = NewText.ToString();
						})
					]
				]
				//Tags section
				+ SVerticalBox::Slot()
				.AutoHeight()
				[
					SNew(SStaticConfigurationInput)
					.LabelText(FText::FromString("Tags"))
					[
						//New String set input widget
						SNew(SStringSetInputWidget)
						.StringList(&EnvironmentData->Tags)
						.IsDefault(false)
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
							.Text(FText::FromString(EnvironmentData->Version.Tag))
							.Font(DefaultFont)
							.OnTextChanged_Lambda([this](const FText& NewText)
							{
								EnvironmentData->Version.Tag = NewText.ToString();
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
							.Text(FText::FromString(EnvironmentData->Version.Revision))
							.OnTextChanged_Lambda([this](const FText& NewText)
							{
								EnvironmentData->Version.Revision = NewText.ToString();
							})
						]
					]
				]
				+SVerticalBox::Slot()
				.AutoHeight()
				[
					SNew(SStaticConfigurationInput)
					.LabelText(FText::FromString("Preview image"))
					[
						SNew(SPreviewImageListWidget)
						.PreviewImagesList(&EnvironmentData->Previews)
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
					SControlListFormWidget::ShowAsPopup(&EnvironmentData->Controls, &*DefaultControls,FText::FromString("Environment Controls"));
					return FReply::Handled();
				})
			]
			/* Preset PopUp Buttons */
			+ SVerticalBox::Slot()
			.HAlign(HAlign_Left)
			.VAlign(VAlign_Center)
			.Padding(0.0f, 5.0f, 0.0f, 5.0f)
			.AutoHeight()
			[
				SNew(SButton)
				.Text(FText::FromString("Configure Presets"))
				.OnClicked_Lambda([this]()
				{
					SPresetListFormWidget::ShowAsPopup(&EnvironmentData->Presets, &EnvironmentData->Controls,FText::FromString("Environment Presets"));
					return FReply::Handled();
				})
			]
	];

}

