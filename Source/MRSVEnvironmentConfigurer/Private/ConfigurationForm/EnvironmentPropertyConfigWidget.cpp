#include "ConfigurationForm/EnvironmentPropertyConfigWidget.h"
#include "ConfigurationForm/Data/ConfigurationData.h"
#include "PropertyCustomizationHelpers.h"
#include "ConfigurationForm/StaticConfigurationInput.h"

void SEnvironmentPropertyConfigWidget::Construct(const FArguments& InArgs)
{
	ParentBox = InArgs._ParentBox;

	// Define type options
	UEnum* ControlTypeRef = StaticEnum<EControlType>();

	if (ControlTypeRef)
	{
		for (int32 i = 0; i < ControlTypeRef->NumEnums() - 1; ++i)
		{
			FString EnumName = ControlTypeRef->GetDisplayNameTextByIndex(i).ToString();
			TypeOptions.Add(MakeShared<FString>(EnumName));
		}
	}

	ChildSlot
	[
		SNew(SBorder)
		.BorderBackgroundColor(FColor::White)
		.Padding(10.0f)
		[
			SNew(SVerticalBox)
			+SVerticalBox::Slot()
			.Padding(0.0f, 0.0f, 0.0f, 5.0f)
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
					.OnClicked(this, &SEnvironmentPropertyConfigWidget::OnRemoveClicked)
					[
						// Wrap the image in SBox to control its size
						SNew(SBox)
						.HAlign(HAlign_Center)
						.VAlign(VAlign_Center)
						[
							SNew(SImage)
							.Image(FCoreStyle::Get().GetBrush("Symbols.X"))
						]
					]
				]
			]
			+SVerticalBox::Slot()
			.AutoHeight()
			[
				SNew(SStaticConfigurationInput)
				.LabelText(FText::FromString("Type"))
				[
					SNew(SComboBox<TSharedPtr<FString>>)
					.HasDownArrow(true)
					.OptionsSource(&TypeOptions) // Source for the dropdown
					.OnGenerateWidget(this, &SEnvironmentPropertyConfigWidget::GenerateDropdownItem)
					.OnSelectionChanged(this, &SEnvironmentPropertyConfigWidget::OnTypeSelected) // Handle selection
					.Content()
					[
						SNew(STextBlock)
						.Text(this, &SEnvironmentPropertyConfigWidget::GetTypeInputHint) // Display the current selection
					]
				]
			]
			+SVerticalBox::Slot()
			.AutoHeight()
			[
				SNew(SStaticConfigurationInput)
				.LabelText(FText::FromString("Updatable Actor"))
				[
					SNew(SVerticalBox)
					+SVerticalBox::Slot()
					.VAlign(VAlign_Center)
					.HAlign(HAlign_Left)
					.Padding(0.0f, 0.0f, 0.0f, 5.0f)
					.AutoHeight()
					[
						SNew(SObjectPropertyEntryBox)
						.DisplayBrowse(true)
						.DisplayThumbnail(true)
						.AllowedClass(AActor::StaticClass())
						.EnableContentPicker(true)
						.OnObjectChanged(this, &SEnvironmentPropertyConfigWidget::OnActorSelected)
						.ObjectPath(this, &SEnvironmentPropertyConfigWidget::GetCurrentObjectPath)
					]
					+SVerticalBox::Slot()
					.VAlign(VAlign_Center)
					.HAlign(HAlign_Left)
					.Padding(0.0f, 0.0f, 0.0f, 5.0f)
					.AutoHeight()
					[
						SAssignNew(ActorPropertyInput, SComboBox<TSharedPtr<FString>>)
						.HasDownArrow(true)
						.IsEnabled(false)
						.OptionsSource(&ActorProperties) // Source for the dropdown
						.OnGenerateWidget(this, &SEnvironmentPropertyConfigWidget::GenerateDropdownItem)
						.OnSelectionChanged(this, &SEnvironmentPropertyConfigWidget::OnActorPropertySelected) // Handle selection
						.Content()
						[
							SNew(STextBlock)
							.Text(this, &SEnvironmentPropertyConfigWidget::GetPropertyInputHint) // Display the current selection
						]
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
	ActorProperties.Empty();
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
		// Enable property input
		ActorPropertyInput->SetEnabled(true);
	} else
	{
		// Disable property input
		ActorPropertyInput->SetEnabled(false);
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

void SEnvironmentPropertyConfigWidget::OnTypeSelected(TSharedPtr<FString> NewSelection,
	ESelectInfo::Type SelectInfo)
{
	if(NewSelection.IsValid())
	{
		UE_LOG(LogTemp, Warning, TEXT("Type Selected: %s"), **NewSelection);
		// Define type options
		UEnum* ControlTypeRef = StaticEnum<EControlType>();
		ChosenType = MakeShared<EControlType>(static_cast<EControlType>(ControlTypeRef->GetValueByNameString(*NewSelection)));
	}
}

FString SEnvironmentPropertyConfigWidget::GetCurrentObjectPath() const
{
	// Return the current object path as a string
	return ChosenActor != nullptr ? ChosenActor->GetPathName() : FString();
}

TSharedRef<SWidget> SEnvironmentPropertyConfigWidget::GenerateDropdownItem(TSharedPtr<FString> InItem)
{
	return SNew(STextBlock)
		.Text(FText::FromString(*InItem));
}

// Gets the currently selected property name for actor property
FText SEnvironmentPropertyConfigWidget::GetPropertyInputHint() const
{
	if (ChosenProperty != nullptr)
	{
		return FText::FromString(*ChosenProperty);
	}
	return FText::FromString("Choose actor property... ");
}

// Gets the currently selected property name for property type
FText SEnvironmentPropertyConfigWidget::GetTypeInputHint() const
{
	if (ChosenType.IsValid())
	{
		// Define type options
		UEnum* ControlTypeRef = StaticEnum<EControlType>();
		//Return text
		return FText::FromString(ControlTypeRef->GetDisplayNameTextByValue(*ChosenType).ToString());
	}
	return FText::FromString("Choose control type... ");
}