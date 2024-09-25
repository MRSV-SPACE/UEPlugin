#pragma once
#include "SSearchableComboBox.h"

// Workaround as Unreal cant handle TMap as a SLATE_ARGUMENT
template<typename T>
struct FDropdownItem
{
	T Key;
	FString Name;

	FDropdownItem() {}
	FDropdownItem(const T& InKey, const FString& InName)
		: Key(InKey), Name(InName) {}
};

template<typename T>
class SSearchableDropdownWidget : public SCompoundWidget
{
	DECLARE_DELEGATE_OneParam(
		FOnSelectionChanged,
		/** param: The newly selected value */
		TSharedPtr<T>
	)
	
	SLATE_BEGIN_ARGS(SSearchableDropdownWidget)
		: _HintText("Select...") {}
		
		SLATE_ARGUMENT(TArray<TSharedPtr<FDropdownItem<T>>>*, OptionsSource)
		SLATE_ARGUMENT(FString, HintText)
		SLATE_ARGUMENT(TSharedPtr<FDropdownItem<T>>, SelectedOption)

		SLATE_EVENT( FOnSelectionChanged, OnSelectionChanged )
	SLATE_END_ARGS()
	
	void Construct(const FArguments& InArgs)
	{
		//Store params
		this->OnSelectionChanged = InArgs._OnSelectionChanged;
		this->HintText = InArgs._HintText;
		this->OptionsSource = InArgs._OptionsSource;
		//Construct view
		ChildSlot
		[
			SAssignNew(ComboBox, SSearchableComboBox)
			.HasDownArrow(true)
			.IsEnabled_Lambda([this]()
			{
				return this->IsEnabled();
			})
			.OptionsSource(&AvailableItems) // Source for the dropdown
			.OnGenerateWidget(this, &SSearchableDropdownWidget::GenerateDropdownItem)
			.OnSelectionChanged(this, &SSearchableDropdownWidget::OnSelectionChanged_Internal) // Handle selection
			[
				SNew(STextBlock)
				.Text(this, &SSearchableDropdownWidget::GetInputHint) // Display the current selection
			]
		];
		//Refresh Options
		this->RefreshOptions();
		//Set selected item from args
		ComboBox->SetSelectedItem(
			InArgs._SelectedOption ? MakeShared<FString>(InArgs._SelectedOption->Name).ToSharedPtr() : nullptr,
			ESelectInfo::Type::Direct);
	}

	void RefreshOptions()
	{
		AvailableItems.Empty();
		ReverseLookupMap.Empty();
		ChosenItem.Reset();
		for (TSharedPtr<FDropdownItem<T>> Option : *this->OptionsSource)
		{
			//Create shared ptr to name
			TSharedPtr<FString> Name = MakeShared<FString>(Option->Name);
			AvailableItems.Add(Name);
			ReverseLookupMap.Add(Name, MakeShared<T>(Option->Key));
		}
		ComboBox->RefreshOptions();
	}
	
private:
	FOnSelectionChanged OnSelectionChanged;
	TSharedPtr<FString> ChosenItem;
	FString HintText;
	TArray<TSharedPtr<FDropdownItem<T>>>* OptionsSource = nullptr;
	TSharedPtr<SSearchableComboBox> ComboBox;
	TArray<TSharedPtr<FString>> AvailableItems;
	TMap<TSharedPtr<FString>, TSharedPtr<T>> ReverseLookupMap;
	
	TSharedRef<SWidget> GenerateDropdownItem(TSharedPtr<FString> InItem)
	{
		return SNew(STextBlock)
			.Text(FText::FromString(*InItem));
	}

	void OnSelectionChanged_Internal(TSharedPtr<FString> ProposedSelection, ESelectInfo::Type SelectInfo)
	{
		//Store chosen item
		ChosenItem = ProposedSelection;
		//Not execute OnSelectionChange if it is directly from code
		if(SelectInfo != ESelectInfo::Type::Direct)
		{
			UE_LOG(LogTemp, Display, TEXT("Change to %s"), *UEnum::GetValueAsString(SelectInfo));
			if(!OnSelectionChanged.ExecuteIfBound(*ReverseLookupMap.Find(ProposedSelection)))
			{
				UE_LOG(LogTemp, Warning, TEXT("No OnSelectionChange Event bound to Searchable Dropdown"));
			}
		}
	}

	FText GetInputHint() const
	{
		if (ChosenItem != nullptr)
		{
			return FText::FromString(*ChosenItem);
		}
		return FText::FromString(HintText);
	}
};
