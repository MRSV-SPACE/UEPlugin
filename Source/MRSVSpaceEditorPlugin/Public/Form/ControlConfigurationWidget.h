#pragma once
#include "ControlListFormWidget.h"
#include "DynamicControlDetailsForm.h"
#include "HelperWidgets/SearchableDropdownWidget.h"

class SControlConfigurationWidget : public SCompoundWidget
{
public:
	SLATE_BEGIN_ARGS(SControlConfigurationWidget) {}
		SLATE_ARGUMENT(TSharedPtr<SScrollBox>, ParentBox)
		SLATE_ARGUMENT(int, ControlDataIndex)
		SLATE_ARGUMENT(TSharedPtr<TArray<FControl>>, ControlList)
	SLATE_END_ARGS()

	void Construct(const FArguments& InArgs);

private:
	TSharedPtr<SScrollBox> ParentBox; // Container for the widget
	TSharedPtr<SDynamicControlDetailsForm> DetailsForm;
	TWeakObjectPtr<AActor> ChosenActor; // Current actor chosen in the picker
	TArray<TSharedPtr<FDropdownItem<EControlType>>> TypeOptions;
	TSharedPtr<SSearchableDropdownWidget<FString>> ActorPropertyInput;
	TArray<TSharedPtr<FDropdownItem<FString>>> ActorProperties;
	TSharedPtr<FString> ChosenProperty; // The chosen property

	TUniquePtr<int> ControlDataIndex;
	TSharedPtr<TArray<FControl>> ControlList;

	FReply OnRemoveClicked();
	void OnActorSelected(const FAssetData& AssetData);
	void OnActorPropertySelected(TSharedPtr<FString> NewSelection);
	void OnTypeSelected(TSharedPtr<EControlType> NewSelection) const;
	FString GetCurrentObjectPath() const;
	FControl* GetControlData() const;
};
