#pragma once

/**
 * The Control Configuration Widget to configure an environment control
 */
class SChoiceSelectionWidget : public SCompoundWidget
{
public:
	SLATE_BEGIN_ARGS(SChoiceSelectionWidget){}
		//Value of the tag
		SLATE_ARGUMENT(FString,ChoiceName);
		//Pointer to the list of possible choices
		SLATE_ARGUMENT(TArray<FString>*, ChoiceList);
		//Is the choice is checked 
		SLATE_ARGUMENT(bool, IsChecked);
	SLATE_END_ARGS()
	
	/**
	 * Construct the widget
	 * @param InArgs const FArguments& 
	 */
	void Construct(const FArguments& InArgs);

private:
	/**
	 * Name/value of the tag
	 */
	FString ChoiceName;

	/**
	 * The list of the possible choices
	 */
	TArray<FString>* ChoiceList = nullptr;

	/**
	 * Pointer to the container of all the choices
	 */
	TSharedPtr<SHorizontalBox> ChoiceContainer;

	/***
	 * The checked status of the choice (True = Checked ; False = Unchecked)
	 */
	bool CheckStatus = false;

	/***
	 * Function to execute when the checkbox state changes
	 * Update the list the of checked value (list given in StringSetInputWidget)
	 *
	 * @param NewState New state of the checkbox 
	 */
	void OnStateChange(ECheckBoxState NewState);
};
