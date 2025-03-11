#pragma once

DECLARE_DELEGATE_OneParam(FOnRemoveCallback, TSharedRef<SWidget>)

/**
 * The Control Configuration Widget to configure an environment control
 */
class STagWidget : public SCompoundWidget
{
public:
	SLATE_BEGIN_ARGS(STagWidget){}
		//Value of the tag
		SLATE_ARGUMENT(FString,TagName);
		// Callback delegate for removing the control
		SLATE_EVENT(FOnRemoveCallback, OnRemove)
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
	FString TagName;
	/**
	 * The callback delegate to remove the current control
	 */
	FOnRemoveCallback RemoveCallback;
	/**
	 * Function to handle when the remove button is clicked
	 *
	 * @return Reply for handled events
	 */
	FReply OnRemoveClicked();
};
