#pragma once

#include "ConfigurationData.h"

/**
 * The widget for the input form of a list of controls
 */
class SControlListFormWidget : public SCompoundWidget
{
public:
	SLATE_BEGIN_ARGS(SControlListFormWidget) {}
		// The initial list of controls
		SLATE_ARGUMENT(TSharedPtr<TArray<FControl>>, ControlList)
	SLATE_END_ARGS()

	void Construct(const FArguments& InArgs);

	/**
	 * Showing the form as a new popup window
	 *
	 * @param InitalList The initial list of controls
	 * @param PopupTitle The title of the popup window
	 * @param PopUpSize The size of the popup window
	 * @return The created control list form widget
	 */
	static TSharedRef<SControlListFormWidget> ShowAsPopup(
		TSharedPtr<TArray<FControl>> InitalList,
		FText PopupTitle,
		FVector2D PopUpSize = FVector2D(800, 400));
private:
	/**
	 * The container widget for the control form widgets
	 */
	TSharedPtr<SScrollBox> PropertyConfigFormContainer;

	/**
	 * The internal list of controls
	 */
	TSharedPtr<TArray<FControl>> ControlList;

	/**
	 * The default font size
	 */
	FSlateFontInfo DefaultFont = FCoreStyle::GetDefaultFontStyle("Regular", 10);

	/**
	 * Function for adding a property configuration form to the list
	 *
	 * @return The reply if the event is handled (Used for handled events)
	 */
	FReply AddPropertyConfigurationForm() const;
};
