#pragma once
#include "Data/ConfigurationData.h"
#include "Data/ConfigurationDataHandler.h"
#include "HelperWidgets/SearchableDropdownWidget.h"

class SPropertyConfigurationWidget : public SCompoundWidget
{
public:
	SLATE_BEGIN_ARGS(SPropertyConfigurationWidget) {}
		SLATE_ARGUMENT(TSharedPtr<SScrollBox>, ParentBox)
		SLATE_ARGUMENT(TSharedPtr<FControl>, ControlDataRef)
		SLATE_ARGUMENT(TSharedPtr<ConfigurationDataHandler>, DataHandler)
	SLATE_END_ARGS()

	void Construct(const FArguments& InArgs);

private:
	TSharedPtr<SScrollBox> ParentBox; // Container for the widget
	TWeakObjectPtr<AActor> ChosenActor; // Current actor chosen in the picker
	TArray<TSharedPtr<FDropdownItem<EControlType>>> TypeOptions;
	TSharedPtr<SSearchableDropdownWidget<FString>> ActorPropertyInput;
	TArray<TSharedPtr<FDropdownItem<FString>>> ActorProperties;
	TSharedPtr<FString> ChosenProperty; // The chosen property

	TSharedPtr<FControl> ControlDataRef;
	TSharedPtr<ConfigurationDataHandler> DataHandler;

	FReply OnRemoveClicked();
	void OnActorSelected(const FAssetData& AssetData);
	void OnActorPropertySelected(TSharedPtr<FString> NewSelection);
	void OnTypeSelected(TSharedPtr<EControlType> NewSelection);
	FString GetCurrentObjectPath() const;
};
