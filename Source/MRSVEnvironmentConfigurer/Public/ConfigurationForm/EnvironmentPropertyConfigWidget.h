#pragma once

class SEnvironmentPropertyConfigWidget : public SCompoundWidget
{
public:
	SLATE_BEGIN_ARGS(SEnvironmentPropertyConfigWidget) {}
		SLATE_ARGUMENT(TSharedPtr<SScrollBox>, ParentBox)
	SLATE_END_ARGS()

	void Construct(const FArguments& InArgs);

private:
	TSharedPtr<SScrollBox> ParentBox; // Container for the widget
	TWeakObjectPtr<AActor> ChosenActor; // Current actor chosen in the picker
	TArray<TSharedPtr<FString>> ActorProperties;
	TSharedPtr<FString> ChosenProperty; // The chosen property

	FReply OnRemoveClicked();
	void OnActorSelected(const FAssetData& AssetData);
	void OnActorPropertySelected(TSharedPtr<FString> NewSelection, ESelectInfo::Type SelectInfo);
	FString GetCurrentObjectPath() const;
	TSharedRef<SWidget> GeneratePropertyDropdownItem(TSharedPtr<FString> InItem);
	FText GetPropertyInputHint() const;
};
