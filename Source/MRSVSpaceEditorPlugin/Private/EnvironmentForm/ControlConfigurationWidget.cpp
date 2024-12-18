#include "EnvironmentForm/ControlConfigurationWidget.h"

#include "PropertyCustomizationHelpers.h"
#include "HelperWidgets/InputTabMenuWidget.h"
#include "HelperWidgets/StaticConfigurationInput.h"

void SControlConfigurationWidget::Construct(const FArguments& InArgs)
{
	//Store params
	ControlData = InArgs._ControlData;
	RemoveCallback = InArgs._OnRemove;
	
	//Define color for blueprint function background
	FSlateBrush* BPFuncBackgroundBrush = new FSlateBrush();
	BPFuncBackgroundBrush->TintColor = FLinearColor(0.8f, 0.0f, 0.0f, 0.3f);
	
	//Define rounded border for widget
	FSlateBrush* RoundedBrush = new FSlateBrush();
	RoundedBrush->TintColor = FLinearColor(0.3f, 0.3f, 0.3f, 0.1f);
	RoundedBrush->DrawAs = ESlateBrushDrawType::Type::RoundedBox;
	RoundedBrush->OutlineSettings.CornerRadii = FVector4(8.0f, 8.0f, 8.0f, 8.0f);
	RoundedBrush->OutlineSettings.RoundingType = ESlateBrushRoundingType::FixedRadius;
	
	//Set the actor from control metadata
	if (!ControlData->Action.Actor.IsEmpty())
	{
		ChosenActor = Cast<AActor>(StaticLoadObject(AActor::StaticClass(), nullptr, *ControlData->Action.Actor));
	}
	// Define type options from Enum
	UEnum* ControlTypeRef = StaticEnum<EControlType>();
	for (uint8 i = 0; i < ControlTypeRef->NumEnums() - 1; ++i)
	{
		TypeOptions.Add(MakeShareable(new SSearchableDropdownWidget<EControlType>::FDropdownItem(
			static_cast<EControlType>(ControlTypeRef->GetValueByIndex(i)),
			ControlTypeRef->GetDisplayNameTextByIndex(i).ToString())));
	}
	//Construct basic view
	ChildSlot
	[
		SNew(SBorder)
		.BorderImage(RoundedBrush)
		.Padding(10.0f)
		[
			SNew(SVerticalBox)
			+SVerticalBox::Slot()
			.Padding(0.0f, 0.0f, 0.0f, 5.0f)
			[
				SNew(SHorizontalBox)
				+SHorizontalBox::Slot()
				[
					SNew( SEditableText )
					.HintText(FText::FromString("Type Property Name..."))
					.Text(FText::FromString(ControlData->Name))
					.OnTextChanged_Lambda([this](const FText& NewText)
					{
						ControlData->Name = NewText.ToString();
					})
				]
				+SHorizontalBox::Slot()
				.AutoWidth()
				.Padding(5.0f, 0.0f)
				[
					SNew(SButton)
					.Text(FText::FromString("x"))
					.ButtonColorAndOpacity(FLinearColor::Red)
					.OnClicked(this, &SControlConfigurationWidget::OnRemoveClicked)
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
					.SelectedOption(ControlData->Type ? MakeShareable(new SSearchableDropdownWidget<EControlType>::FDropdownItem(ControlData->Type.GetValue(), ControlTypeRef->GetDisplayNameTextByIndex(ControlData->Type.GetValue()).ToString())) : nullptr)
					.OnSelectionChanged(this, &SControlConfigurationWidget::OnTypeSelected) // Handle selection
				]
			]
			+SVerticalBox::Slot()
			.AutoHeight()
			[
				SNew(SStaticConfigurationInput)
				.LabelText(FText::FromString("Details"))
				[
					SAssignNew(DetailsForm, SDynamicControlDetailsForm)
					.ControlDetails_Lambda([this]()
					{
						return &ControlData->Details;
					})
					.InitalType(ControlData->Type ? new EControlType(ControlData->Type.GetValue()) : nullptr)
				]
			]
			+SVerticalBox::Slot()
			.AutoHeight()
			[
				SNew(SInputTabMenuWidget<EControlActionType>)
				.SelectedTab(ControlData->Action.Type)
				.OnSelectionChanged_Lambda([this](EControlActionType SelectedType)
				{
					ControlData->Action.Type = SelectedType;
				})
				+ SInputTabMenuWidget<EControlActionType>::Slot()
				.Title("Actor")
				.Value(ACTOR)
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
							.OnObjectChanged(this, &SControlConfigurationWidget::OnActorSelected)
							.ObjectPath(this, &SControlConfigurationWidget::GetActorPathName)
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
							.SelectedOption(!ControlData->Action.Property.IsEmpty() ? MakeShareable(new SSearchableDropdownWidget<FString>::FDropdownItem(ControlData->Action.Property, ControlData->Action.Property)) : nullptr)
							.OnSelectionChanged(this, &SControlConfigurationWidget::OnActorPropertySelected) // Handle selection
						]
					]
				]
				+ SInputTabMenuWidget<EControlActionType>::Slot()
				.Title("Blueprint Implementation")
				.Value(BLUEPRINT)
				[
					SNew(SOverlay)
					+ SOverlay::Slot()
					[
						SNew(SImage)
						.Image(BPFuncBackgroundBrush)
					]
					+SOverlay::Slot()
					.Padding(5.0f)
					[
						SNew(STextBlock)
						.Text(FText::FromString("Explanation on how to implement MRSV Blueprint Events (TODO)"))
					]					
				]
				+ SInputTabMenuWidget<EControlActionType>::Slot()
				.Title("No Action")
				.Value(NONE)
				[
					SNew(STextBlock)
					.Text(FText::FromString("No Action defined"))
				]
			]
		]
	];
}

FReply SControlConfigurationWidget::OnRemoveClicked()
{
	if (RemoveCallback.ExecuteIfBound(AsShared()))
	{
		return FReply::Handled();
	}
	UE_LOG(LogTemp, Warning, TEXT("No callback registred for removing control"))
	return FReply::Unhandled();
}

void SControlConfigurationWidget::OnActorSelected(const FAssetData& AssetData)
{
	// Handle the selected object here
	ChosenActor = Cast<AActor>(AssetData.GetAsset());
	//Store in metadata
	ControlData->Action.Actor = ChosenActor->GetPathName();
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
				// TODO store a better reference to the property (ID?)
				ActorProperties.Add(MakeShareable(new SSearchableDropdownWidget<FString>::FDropdownItem(Property->GetName(), Property->GetDisplayNameText().ToString())));
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

void SControlConfigurationWidget::OnActorPropertySelected(TSharedPtr<FString> NewSelection)
{
	if(NewSelection.IsValid())
	{
		ControlData->Action.Property = *NewSelection;
		ChosenProperty = NewSelection;
	}
}

void SControlConfigurationWidget::OnTypeSelected(TSharedPtr<EControlType> NewSelection) const
{
	if(NewSelection.IsValid())
	{
		//Store selection
		ControlData->Type = *NewSelection;
		//Update Dynamic Details Form
		DetailsForm->SetType(&*NewSelection);
	}
}

FString SControlConfigurationWidget::GetActorPathName() const
{
	// Return the current object path as a string
	return ChosenActor != nullptr ? ChosenActor->GetPathName() : FString();
}
