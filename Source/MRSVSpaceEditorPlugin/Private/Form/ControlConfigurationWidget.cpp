#include "Form/ControlConfigurationWidget.h"

#include "PropertyCustomizationHelpers.h"
#include "Form/DynamicControlDetailsForm.h"
#include "Form/HelperWidgets/SearchableDropdownWidget.h"
#include "Form/HelperWidgets/StaticConfigurationInput.h"
#include "Form/HelperWidgets/InputTabMenuWidget.h"

void SControlConfigurationWidget::Construct(const FArguments& InArgs)
{
	//Store params
	ParentBox = InArgs._ParentBox;
	ControlDataIndex = MakeUnique<int>(InArgs._ControlDataIndex);
	ControlList = InArgs._ControlList;
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
	if (!GetControlData()->Action.Actor.IsEmpty())
	{
		ChosenActor = Cast<AActor>(StaticLoadObject(AActor::StaticClass(), nullptr, *GetControlData()->Action.Actor));
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
					.Text(FText::FromString(GetControlData()->Name))
					.OnTextChanged_Lambda([this](const FText& NewText)
					{
						GetControlData()->Name = NewText.ToString();
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
					.SelectedOption(GetControlData()->Type ? MakeShareable(new FDropdownItem(GetControlData()->Type.GetValue(), ControlTypeRef->GetDisplayNameTextByIndex(GetControlData()->Type.GetValue()).ToString())) : nullptr)
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
						return &GetControlData()->Details;
					})
					.InitalType(GetControlData()->Type ? new EControlType(GetControlData()->Type.GetValue()) : nullptr)
				]
			]
			+SVerticalBox::Slot()
			.AutoHeight()
			[
				SNew(SInputTabMenuWidget<EControlActionType>)
				.SelectedTab(GetControlData()->Action.Type)
				.OnSelectionChanged_Lambda([this](EControlActionType SelectedType)
				{
					GetControlData()->Action.Type = SelectedType;
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
							.ObjectPath(this, &SControlConfigurationWidget::GetCurrentObjectPath)
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
							.SelectedOption(!GetControlData()->Action.Property.IsEmpty() ? MakeShareable(new FDropdownItem(GetControlData()->Action.Property, GetControlData()->Action.Property)) : nullptr)
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
			]
		]
	];
}

FReply SControlConfigurationWidget::OnRemoveClicked()
{
	if (ParentBox.IsValid())
	{
		//Remove from metadata and don't shrink array to not mix up array indexes
		ControlList->RemoveAt(*ControlDataIndex, EAllowShrinking::No);
		//Remove slot from UI
		ParentBox->RemoveSlot(AsShared());
	}
	return FReply::Handled();
}

void SControlConfigurationWidget::OnActorSelected(const FAssetData& AssetData)
{
	// Handle the selected object here
	ChosenActor = Cast<AActor>(AssetData.GetAsset());
	//Store in metadata
	GetControlData()->Action.Actor = ChosenActor->GetPathName();
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

void SControlConfigurationWidget::OnActorPropertySelected(TSharedPtr<FString> NewSelection)
{
	if(NewSelection.IsValid())
	{
		GetControlData()->Action.Property = *NewSelection;
		ChosenProperty = NewSelection;
	}
}

void SControlConfigurationWidget::OnTypeSelected(TSharedPtr<EControlType> NewSelection) const
{
	if(NewSelection.IsValid())
	{
		//Store selection
		GetControlData()->Type = *NewSelection;
		//Update Dynamic Details Form
		DetailsForm->SetType(&*NewSelection);
	}
}

FString SControlConfigurationWidget::GetCurrentObjectPath() const
{
	// Return the current object path as a string
	return ChosenActor != nullptr ? ChosenActor->GetPathName() : FString();
}

FControl* SControlConfigurationWidget::GetControlData() const
{
	return &(*ControlList)[*ControlDataIndex];
}
