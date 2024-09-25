#include "Form/PropertyConfigurationWidget.h"

#include "Data/ConfigurationData.h"
#include "PropertyCustomizationHelpers.h"
#include "Form/HelperWidgets/SearchableDropdownWidget.h"
#include "Form/HelperWidgets/StaticConfigurationInput.h"
#include "Form/HelperWidgets/TabMenuWidget.h"

void SPropertyConfigurationWidget::Construct(const FArguments& InArgs)
{
	//Store params
	ParentBox = InArgs._ParentBox;
	ControlDataRef = InArgs._ControlDataRef;
	DataHandler = InArgs._DataHandler;
	//Set the actor from control metadata
	if (*ControlDataRef->Action.Actor != nullptr)
	{
		ChosenActor = Cast<AActor>(StaticLoadObject(AActor::StaticClass(), nullptr, *ControlDataRef->Action.Actor));
	}
	// Define type options from Enum
	UEnum* ControlTypeRef = StaticEnum<EControlType>();
	for (uint8 i = 0; i < ControlTypeRef->NumEnums() - 1; ++i)
	{
		TypeOptions.Add(MakeShareable(new FDropdownItem(
			static_cast<EControlType>(ControlTypeRef->GetValueByIndex(i)),
			ControlTypeRef->GetDisplayNameTextByIndex(i).ToString())));
	}
	//Construct basic view
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
						.Text(FText::FromString(ControlDataRef->Name))
						.OnTextChanged_Lambda([this](const FText& NewText)
						{
							ControlDataRef->Name = NewText.ToString();
						})
					]
				]
				+SHorizontalBox::Slot()
				.AutoWidth()
				.Padding(5.0f, 0.0f)
				[
					SNew(SButton)
					.Text(FText::FromString("x"))
					.ButtonColorAndOpacity(FLinearColor::Red)
					.OnClicked(this, &SPropertyConfigurationWidget::OnRemoveClicked)
				]
			]
			+SVerticalBox::Slot()
			.AutoHeight()
			[
				SNew(SStaticConfigurationInput)
				.LabelText(FText::FromString("Type"))
				[
					SNew(SSearchableDropdownWidget<EControlType>)
					.HintText("Choose control type... ")
					.OptionsSource(&TypeOptions) // Source for the dropdown
					.SelectedOption(ControlDataRef->Type ? MakeShareable(new FDropdownItem(ControlDataRef->Type.GetValue(), ControlTypeRef->GetDisplayNameTextByIndex(ControlDataRef->Type.GetValue()).ToString())) : nullptr)
					.OnSelectionChanged(this, &SPropertyConfigurationWidget::OnTypeSelected) // Handle selection
				]
			]
			+SVerticalBox::Slot()
			.AutoHeight()
			[
				SNew(STabMenuWidget)
				+ STabMenuWidget::Slot()
				.Title("Single Actor")
				[
					SNew(SStaticConfigurationInput)
					.LabelText(FText::FromString("Updatable Actor"))
					[
						SNew(SVerticalBox)
						+SVerticalBox::Slot()
						.VAlign(VAlign_Top)
						.Padding(0.0f, 0.0f, 0.0f, 5.0f)
						.AutoHeight()
						[
							SNew(SObjectPropertyEntryBox)
							.DisplayBrowse(true)
							.DisplayThumbnail(true)
							.EnableContentPicker(true)
							//Allow only actors
							.AllowedClass(AActor::StaticClass())
							.OnObjectChanged(this, &SPropertyConfigurationWidget::OnActorSelected)
							.ObjectPath(this, &SPropertyConfigurationWidget::GetCurrentObjectPath)
						]
						+SVerticalBox::Slot()
						.VAlign(VAlign_Top)
						.AutoHeight()
						[
							SAssignNew(ActorPropertyInput, SSearchableDropdownWidget<FString>)
							.HintText("Choose actor property... ")
							.IsEnabled_Lambda([this]()
							{
								//Only enable if chosen actor is not null
								return ChosenActor != nullptr;
							})
							.OptionsSource(&ActorProperties) // Source for the dropdown
							.SelectedOption(MakeShareable(new FDropdownItem(ControlDataRef->Action.Property, ControlDataRef->Action.Property)))
							.OnSelectionChanged(this, &SPropertyConfigurationWidget::OnActorPropertySelected) // Handle selection
						]
					]
				]
				+ STabMenuWidget::Slot()
				.Title("Dynamic Blueprint Function")
				[
					SNew(SBorder)
					.BorderBackgroundColor(FLinearColor(0.9f, 0.25f, 0.2f, 0.7f))
					.Padding(5.0f)
					[
						SNew(STextBlock)
						.Text(FText::FromString("Use a blueprint function"))
					]					
				]
			]
		]
	];
}

FReply SPropertyConfigurationWidget::OnRemoveClicked()
{
	if (ParentBox.IsValid())
	{
		//Remove slot from UI
		ParentBox->RemoveSlot(AsShared());
		//Remove from metadata
		DataHandler->GetEnvironment()->Controls.Remove(*ControlDataRef);
	}
	return FReply::Handled();
}

void SPropertyConfigurationWidget::OnActorSelected(const FAssetData& AssetData)
{
	// Handle the selected object here
	ChosenActor = Cast<AActor>(AssetData.GetAsset());
	//Store in metadata
	ControlDataRef->Action.Actor = ChosenActor->GetPathName();
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
				ActorProperties.Add(MakeShareable(new FDropdownItem(Property->GetName(), Property->GetDisplayNameText().ToString())));
			}
		}
		// Enable property input
		ActorPropertyInput->SetEnabled(true);
	} else
	{
		// Disable property input
		ActorPropertyInput->SetEnabled(false);
	}
	//Refresh Options
	ActorPropertyInput->RefreshOptions();
}

void SPropertyConfigurationWidget::OnActorPropertySelected(TSharedPtr<FString> NewSelection)
{
	if(NewSelection.IsValid())
	{
		ControlDataRef->Action.Property = *NewSelection;
		ChosenProperty = NewSelection;
	}
}

void SPropertyConfigurationWidget::OnTypeSelected(TSharedPtr<EControlType> NewSelection)
{
	if(NewSelection.IsValid())
	{
		ControlDataRef->Type = *NewSelection;
	}
}

FString SPropertyConfigurationWidget::GetCurrentObjectPath() const
{
	// Return the current object path as a string
	return ChosenActor != nullptr ? ChosenActor->GetPathName() : FString();
}