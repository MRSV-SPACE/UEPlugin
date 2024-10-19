#pragma once

#include "SSearchableComboBox.h"

/**
 * A dropdown widget which is searchable and can contain any type of
 * items (Using SSearchableComboBox but allowing other types of items than strings.)
 *
 * @tparam T The type of the value each dropdown item contains
 */
template<typename T>
class SSearchableDropdownWidget : public SCompoundWidget
{
public:
	/**
	 * Struct defining the item in the dropdown consisting of a key with a
	 * dynamic type and a name as a string, which is shown in the dropdown list
	 *
	 * @tparam T The type of the value each item contains
	 */
	struct FDropdownItem
	{
		/**
		 * The key of the dropdown item
		 */
		T Key;

		/**
		 * A name of the dropdown item
		 */
		FString Name;

		FDropdownItem() {}
	
		FDropdownItem(const T& InKey, const FString& InName)
			: Key(InKey), Name(InName) {}
	};
	
	/**
	 * Defining the delegate for the selection changed event on the widget
	 */
	DECLARE_DELEGATE_OneParam(FOnSelectionChanged, TSharedPtr<T>)
	
	SLATE_BEGIN_ARGS(SSearchableDropdownWidget)
		// Default value for the hint text
		: _HintText("Select...") {}

		// Pointer to the options source
		SLATE_ARGUMENT(TArray<TSharedPtr<FDropdownItem>>*, OptionsSource)
		// The hint text, which is shown when no item is selected
		SLATE_ARGUMENT(FString, HintText)
		// Sets the selected option item
		SLATE_ARGUMENT(TSharedPtr<FDropdownItem>, SelectedOption)

		// Event for selection change in the dropdown
		SLATE_EVENT( FOnSelectionChanged, OnSelectionChanged )
	SLATE_END_ARGS()
	
	void Construct(const FArguments& InArgs)
	{
		// Store params
		this->OnSelectionChanged = InArgs._OnSelectionChanged;
		this->HintText = InArgs._HintText;
		this->OptionsSource = InArgs._OptionsSource;
		// Construct view
		ChildSlot
		[
			// Create the searchable combo box
			SAssignNew(ComboBox, SSearchableComboBox)
			.ContentPadding(2.0f)
			.IsEnabled_Lambda([this]()
			{
				return this->IsEnabled();
			})
			// Source for the dropdown
			.OptionsSource(&AvailableItems)
			.OnGenerateWidget_Static(GenerateDropdownItem)
			// Handle selection change event
			.OnSelectionChanged(this, &SSearchableDropdownWidget::OnSelectionChanged_Internal)
			[
				SNew(STextBlock)
				// Display the current selection or the input hint
				.Text(this, &SSearchableDropdownWidget::GetInputHint)
			]
		];
		// Refresh Options
		this->RefreshOptions();
		// Set selected item from args
		ComboBox->SetSelectedItem(
			InArgs._SelectedOption ? MakeShared<FString>(InArgs._SelectedOption->Name).ToSharedPtr() : nullptr,
			ESelectInfo::Type::Direct);
	}

	/**
	 * Refreshes the options from the option source
	 */
	void RefreshOptions()
	{
		// Reset the item list, lookup map and chosen item
		AvailableItems.Empty();
		ReverseLookupMap.Empty();
		ChosenItem.Reset();
		// Loop items in the option source
		for (TSharedPtr<FDropdownItem> Option : *this->OptionsSource)
		{
			//Create shared ptr to name
			TSharedPtr<FString> Name = MakeShared<FString>(Option->Name);
			// Add the string item list
			AvailableItems.Add(Name);
			// Add to reverse lookup map
			ReverseLookupMap.Add(Name, MakeShared<T>(Option->Key));
		}
		ComboBox->RefreshOptions();
	}
	
private:
	
	/**
	 * The event to trigger when the selection changes
	 */
	FOnSelectionChanged OnSelectionChanged;

	/**
	 * The name of the chosen item
	 */
	TSharedPtr<FString> ChosenItem;

	/**
	 * The hint text to show when no item is selected
	 */
	FString HintText;

	/**
	 * The pointer to the source of the options
	 */
	TArray<TSharedPtr<FDropdownItem>>* OptionsSource = nullptr;

	/**
	 * The underlying combox box widget
	 */
	TSharedPtr<SSearchableComboBox> ComboBox;

	/**
	 * The list of available item names to pass to the underlying combo box
	 */
	TArray<TSharedPtr<FString>> AvailableItems;

	/**
	 * The reverse lookup map to lookup from the chosen name to the item
	 */
	TMap<TSharedPtr<FString>, TSharedPtr<T>> ReverseLookupMap;

	/**
	 * Generates a dropdown item from the string
	 *
	 * @param InItem The item (name) to generate a dropdown item for
	 * @return A reference to the widget to add to the dropdown list
	 */
	static TSharedRef<SWidget> GenerateDropdownItem(TSharedPtr<FString> InItem)
	{
		// Generate simple text block for given name
		return SNew(STextBlock)
			.Text(FText::FromString(*InItem));
	}

	/**
	 * Function to handle the selection change internally. Is triggered when the underlying selection changes and
	 * triggers the on selection changed event on the current widget.
	 *
	 * @param ProposedSelection The name of the selected item
	 * @param SelectInfo The type of the selection
	 */
	void OnSelectionChanged_Internal(TSharedPtr<FString> ProposedSelection, ESelectInfo::Type SelectInfo)
	{
		//Store chosen item
		ChosenItem = ProposedSelection;
		//Not execute OnSelectionChange if it is directly from code
		if(SelectInfo != ESelectInfo::Type::Direct)
		{
			// Execute the selection changed event and pass the item from the lookup map as parameter
			if(!OnSelectionChanged.ExecuteIfBound(*ReverseLookupMap.Find(ProposedSelection)))
			{
				// If event not bound, log warning
				UE_LOG(LogTemp, Warning, TEXT("No OnSelectionChange Event bound to Searchable Dropdown"));
			}
		}
	}

	/**
	 * Evaluates the input hint for the dropdown widget.
	 * (Item name if one is selected, else default input hint)
	 *
	 * @return The input hint for the dropdown widget
	 */
	FText GetInputHint() const
	{
		// If an item is currently selected
		if (ChosenItem != nullptr)
		{
			// Return name of currently chosen item
			return FText::FromString(*ChosenItem);
		}
		// Else return default hint text
		return FText::FromString(HintText);
	}
};
