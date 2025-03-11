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
		SLATE_ARGUMENT(TArray<FControl>*, ControlList)
		// The list of default controls
		SLATE_ARGUMENT(TArray<FControl>*, DefaultControls)
	SLATE_END_ARGS()

	void Construct(const FArguments& InArgs);

	/**
	 * Showing the form as a new popup window
	 *
	 * @param InitalList The initial list of controls
	 * @param DefaultControlList The list of default controls
	 * @param PopupTitle The title of the popup window
	 * @param PopUpSize The size of the popup window
	 * @return The created control list form widget
	 */
	static TSharedRef<SControlListFormWidget> ShowAsPopup(
		TArray<FControl>* InitalList,
		TArray<FControl>* DefaultControlList,
		FText PopupTitle,
		FVector2D PopUpSize = FVector2D(800, 400));
	
	/**
 * Showing the form as a new popup window
 * (use for when needing to open the control list form without the default controls like for a control of type Container)
 * @param InitalList The initial list of controls
 * @param PopupTitle The title of the popup window
 * @param PopUpSize The size of the popup window
 * @return The created control list form widget
 */
	static TSharedRef<SControlListFormWidget> ShowAsPopup(
		TArray<FControl>* InitalList,
		FText PopupTitle,
		FVector2D PopUpSize = FVector2D(800, 400));

	/**
	 * Function to reload the form 
	 * Same as if the user closed and reopened the form
	 */
	void ReloadForm() const;
private:
	/**
	 * The container widget for the control form widgets
	 */
	TSharedPtr<SScrollBox> PropertyConfigFormContainer;

	/**
	 * The internal list of controls
	 */
	TArray<FControl>* ControlList = nullptr;
	/**
	 * The list of default controls
	 */
	TArray<FControl>* DefaultControlsList = nullptr;
	/**
	 * The amount of default controls
	 */
	uint64 	numberOfDefault = 0;
	
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

	/**
	 * Function to add every existing control to the form
	 */
	void AddExistingControls() const;

};
