#pragma once

#include "DynamicControlDetailsForm.h"
#include "HelperWidgets/SearchableDropdownWidget.h"

DECLARE_DELEGATE_OneParam(FOnRemoveCallback, TSharedRef<SWidget>)

/**
 * The Control Configuration Widget to configure an environment control
 */
class SControlConfigurationWidget : public SCompoundWidget
{
public:
	SLATE_BEGIN_ARGS(SControlConfigurationWidget) {}
		// Reference to the control object
		SLATE_ARGUMENT(FControl*, ControlData)
		
		// Callback delegate for removing the control
		SLATE_EVENT(FOnRemoveCallback, OnRemove)
	SLATE_END_ARGS()

	void Construct(const FArguments& InArgs);

private:

	/**
	 * The pointer to the control data object
	 */
	FControl* ControlData = nullptr;
	
	/**
	 * The callback delegate to remove the current control
	 */
	FOnRemoveCallback RemoveCallback;

	/**
	 * The form the the control details
	 */
	TSharedPtr<SDynamicControlDetailsForm> DetailsForm;

	/**
	 * The reference to the chosen actor
	 */
	TWeakObjectPtr<AActor> ChosenActor;

	/**
	 * The list of type options
	 */
	TArray<TSharedPtr<SSearchableDropdownWidget<EControlType>::FDropdownItem>> TypeOptions;

	/**
	 * The input widget for the actor property
	 */
	TSharedPtr<SSearchableDropdownWidget<FString>> ActorPropertyInput;

	/**
	 * The item list of actor properties
	 */
	TArray<TSharedPtr<SSearchableDropdownWidget<FString>::FDropdownItem>> ActorProperties;

	/**
	 * The currently chosen actor property
	 */
	TSharedPtr<FString> ChosenProperty;

	/**
	 * Function to handle when the remove button is clicked
	 *
	 * @return Reply for handled events
	 */
	FReply OnRemoveClicked();

	/**
	 * Called when an actor is selected
	 *
	 * @param AssetData The Asset selected
	 */
	void OnActorSelected(const FAssetData& AssetData);

	/**
	 * Called when the actor property is selected
	 *
	 * @param NewSelection The name of the property selected
	 */
	void OnActorPropertySelected(TSharedPtr<FString> NewSelection);

	/**
	 * Called when the control type is selected
	 *
	 * @param NewSelection The control type selected
	 */
	void OnTypeSelected(TSharedPtr<EControlType> NewSelection) const;

	/**
	 * Evaluates the current control actor path name
	 *
	 * @return  The pathname of the chosen control actor or an empty string, of none is selected
	 */
	FString GetActorPathName() const;
};
