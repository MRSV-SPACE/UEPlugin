#include "ConfigurationForm/EnvironmentPropertyConfigWidget.h"

#include "PropertyCustomizationHelpers.h"
#include "ConfigurationForm/StaticConfigurationInput.h"

void SEnvironmentPropertyConfigWidget::Construct(const FArguments& InArgs)
{
	ParentBox = InArgs._ParentBox;

	ChildSlot
	[
		SNew(SBorder)
		.BorderBackgroundColor(FColor::White)
		.Padding(10.0f)
		[
			SNew(SVerticalBox)
			+SVerticalBox::Slot()
			[
				SNew(SHorizontalBox)
				+SHorizontalBox::Slot()
				[
					SNew(SBorder)
					.BorderBackgroundColor(FColor::White)
					.Padding(5.0f)
					[
						SNew( SEditableText )
						.HintText(FText::FromString("Property Name..."))
					]
				]
				+SHorizontalBox::Slot()
				.AutoWidth()
				.Padding(5.0f, 0.0f)
				[
					SNew(SButton)
					.Text(FText::FromString("X"))
					.OnClicked(this, &SEnvironmentPropertyConfigWidget::OnRemoveClicked)
				]
			]
			+SVerticalBox::Slot()
			[
				SNew(SStaticConfigurationInput)
				.LabelText(FText::FromString("Type"))
			]
			+SVerticalBox::Slot()
			[
				SNew(SHorizontalBox)
				+SHorizontalBox::Slot()
				.VAlign(VAlign_Center)
				.HAlign(HAlign_Left)
				[
					SNew( STextBlock )
					.Text( FText::FromString("Asset to update") )
				]
				+SHorizontalBox::Slot()
				.VAlign(VAlign_Center)
				.HAlign(HAlign_Left)
				[
					SNew(SObjectPropertyEntryBox)
					.DisplayBrowse(true)
					.DisplayThumbnail(true)
					.AllowedClass(AActor::StaticClass())
					.EnableContentPicker(true)
					.OnObjectChanged(this, &SEnvironmentPropertyConfigWidget::OnActorSelected)
					.ObjectPath(this, &SEnvironmentPropertyConfigWidget::GetCurrentObjectPath)
				]
				+SHorizontalBox::Slot()
				.VAlign(VAlign_Center)
				.HAlign(HAlign_Left)
				[
					SNew(SComboBox<TSharedPtr<FString>>)
					.HasDownArrow(true)
					.OptionsSource(&ActorProperties) // Source for the dropdown
					.OnGenerateWidget(this, &SEnvironmentPropertyConfigWidget::GeneratePropertyDropdownItem)
					.OnSelectionChanged(this, &SEnvironmentPropertyConfigWidget::OnActorPropertySelected) // Handle selection
					.Content()
					[
						SNew(STextBlock)
						.Text(this, &SEnvironmentPropertyConfigWidget::GetPropertyInputHint) // Display the current selection
					]
				]
			]
		]
	];
}

FReply SEnvironmentPropertyConfigWidget::OnRemoveClicked()
{
	if (ParentBox.IsValid())
	{
		ParentBox->RemoveSlot(AsShared());
	}
	return FReply::Handled();
}

void SEnvironmentPropertyConfigWidget::OnActorSelected(const FAssetData& AssetData)
{
	// Handle the selected object here
	UE_LOG(LogTemp, Warning, TEXT("Object Selected: %s"), *AssetData.GetAsset()->GetPathName());
	ChosenActor = Cast<AActor>(AssetData.GetAsset());
	//Clear slots
	ActorProperties = TArray<TSharedPtr<FString>>();
	//Reset chosen property
	ChosenProperty.Reset();
	//Get Properties and fill asset property picker
	// Return the current object path as a string
	if (ChosenActor != nullptr)
	{
		// Get the UClass of the asset
		UClass* ActorClass = ChosenActor->GetClass();
		for (TFieldIterator<FProperty> It(ActorClass); It; ++It)
		{
			FProperty* Property = *It;
			// We can filter for specific property types if needed
			if (Property->HasAnyPropertyFlags(UP::EditAnywhere))
			{
				// Add the property name to the list
				ActorProperties.Add(MakeShared<FString>(Property->GetName()));
			}
		}
	}
}

void SEnvironmentPropertyConfigWidget::OnActorPropertySelected(TSharedPtr<FString> NewSelection,
	ESelectInfo::Type SelectInfo)
{
	if(NewSelection.IsValid())
	{
		UE_LOG(LogTemp, Warning, TEXT("Actor Property Selected: %s"), **NewSelection);
		ChosenProperty = NewSelection;
	}
}

FString SEnvironmentPropertyConfigWidget::GetCurrentObjectPath() const
{
	// Return the current object path as a string
	return ChosenActor != nullptr ? ChosenActor->GetPathName() : FString();
}

TSharedRef<SWidget> SEnvironmentPropertyConfigWidget::GeneratePropertyDropdownItem(TSharedPtr<FString> InItem)
{
	return SNew(STextBlock)
		.Text(FText::FromString(*InItem));
}

// Gets the currently selected property name for display
FText SEnvironmentPropertyConfigWidget::GetPropertyInputHint() const
{
	if (ChosenProperty != nullptr)
	{
		return FText::FromString(*ChosenProperty);
	}
	return FText::FromString("Choose actor property...");
}