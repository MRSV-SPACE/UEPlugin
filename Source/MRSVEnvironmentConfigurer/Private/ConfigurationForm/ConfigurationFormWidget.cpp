#include "ConfigurationForm/ConfigurationFormWidget.h"

#include "ConfigurationForm/EnvironmentPropertyConfigWidget.h"
#include "ConfigurationForm/StaticConfigurationInput.h"

void SConfigurationFormWidget::Construct(const FArguments& InArgs)
{
	ChildSlot
	[
		/* FORM CONTAINER */
		SNew( SVerticalBox )
			/* TITLE */
			+ SVerticalBox::Slot()
			.HAlign(HAlign_Left)
			.VAlign(VAlign_Top)
			.Padding(10.0f)
			.AutoHeight()
			[
				SNew(SHorizontalBox)
				+ SHorizontalBox::Slot()
				.HAlign(HAlign_Left)
				.VAlign(VAlign_Center)
				.AutoWidth()
				[
					SNew(STextBlock)
					.Text(FText::FromName("MRSV*Space Artist Environment Configuration Tool"))
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
					.ToolTipText(FText::FromString("Save the current environment data"))
					.OnClicked_Raw(this, &SConfigurationFormWidget::SaveConfiguration)
				]
			]
			/* STATIC INPUTS */
			+ SVerticalBox::Slot()
			.VAlign(VAlign_Top)
			.AutoHeight()
			.Padding(10.0f)
			[
				SNew(SVerticalBox)
				+ SVerticalBox::Slot()
				[
					SNew( SStaticConfigurationInput )
					.LabelText(FText::FromString("Environment Name"))
				]
				+ SVerticalBox::Slot()
				[
					SNew( SStaticConfigurationInput )
					.LabelText(FText::FromString("Artist Name"))
				]
			]
			/* ENV PROPERTY DYNAMIC FORM CONTAINER */
			+ SVerticalBox::Slot()
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
						.OnClicked_Raw(this, &SConfigurationFormWidget::AddPropertyConfigurationForm)
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
}

FReply SConfigurationFormWidget::AddPropertyConfigurationForm() const
{
	PropertyConfigFormContainer->AddSlot()
		.Padding(0, 0.0f, 0.0f, 5.0f)
		[
			SNew(SEnvironmentPropertyConfigWidget)
			.ParentBox(PropertyConfigFormContainer)
		];
	return FReply::Handled();
}

FReply SConfigurationFormWidget::SaveConfiguration() const
{
	UE_LOG(LogTemp, Log, TEXT("Saving environment configuration"));
	// Create a new JSON object
	TSharedPtr<FJsonObject> JsonObject = MakeShareable(new FJsonObject());

	// Set values
	JsonObject->SetStringField("name", "Beautiful environment 1");
	JsonObject->SetStringField("artist", "Artist XY");

	// Convert the JSON object to a string
	FString OutputString;
	TSharedRef<TJsonWriter<>> Writer = TJsonWriterFactory<>::Create(&OutputString);
    
	if (FJsonSerializer::Serialize(JsonObject.ToSharedRef(), Writer))
	{
		// The path to the JSON file (relative to the project's Content directory)
		FString FilePath = FPaths::Combine(FPaths::ProjectSavedDir(), TEXT("MRSV/metadata.json"));

		// Save the JSON string to a file
		if (FFileHelper::SaveStringToFile(OutputString, *FilePath))
		{
			UE_LOG(LogTemp, Warning, TEXT("JSON file saved successfully."));
		}
		else
		{
			UE_LOG(LogTemp, Error, TEXT("Failed to save JSON file."));
		}
	}
	// Save `OutputString` to a file
	return FReply::Handled();
}