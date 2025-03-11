#pragma once
#include "Widgets/Layout/SWrapBox.h"
#include "ConfigurationData.h"

class STagWidget;
/**
 * To allow a user to input multiple strings. 
 */
class SStringSetInputWidget : public SCompoundWidget
{
public:
	SLATE_BEGIN_ARGS(SStringSetInputWidget){}
		//List that will contain the strings 
		SLATE_ARGUMENT(TArray<FString>*, StringList);
		//Is this widget for a default control
		SLATE_ARGUMENT(bool, IsDefault);
		//The list of default options (can be left null)
		SLATE_ARGUMENT(TArray<FString>*, DefaultOptionsList);
	SLATE_END_ARGS()

	void Construct(const FArguments& InArgs);

	/*
	 *Function called when the whole widget has been constructed
	 * This needs to be done so the Strings stored in the EnvironmentDate can be displayed when reopen
	 */
	void OnConstructCompleted();
	
	/**
	 * Add a tag visually and in the list (first in the list and then in the container)
	 *
	 * @param Text const FString& : Text value of the tag
	 *
	 */
	void AddTag(const FString& Text);
	/**
	 * Add the choice to the selection
	 * 
	 * @param Text Name of the choice / Text to display for the choice
	 * @param isChecked Should the checkbox of the choice be checked or not
	 */
	void AddChoiceSelection(const FString& Text, bool isChecked);

	/**
	 * Remove a tag visually and in the list (first from the list and then the container)
	 * 
	 * @param TagWidget TSharedPtr<SWidget>* : Pointer value of the widget to remove
	*/
	FReply RemoveTag(TSharedPtr<SWidget>* TagWidget);
    
private:
	/**
	 * List of all the strings linked to the pointer of the given list
	 * Example : List of tags in the EnvironmentDate or List of options in the ControlDetails 
	 */
	TArray<FString>* StringInputList = nullptr;

	/**
	 * List of all the tags
	 */
	TArray<FString> Tags;

	/**
	 * List of all the tags
	 */
	TArray<FString> NormalizeTags;

	/**
	 * Is the control default
	 */
	bool IsDefault = false;

	/**
	 * Pointer to the list of default options
	 */
	TArray<FString>* DefaultOptionsList = nullptr;
	
	/**
	 * The default label font
	 */
	FSlateFontInfo LabelFont = FCoreStyle::GetDefaultFontStyle("Regular", 10);

	/**
	 * The default font style
	 */
	FSlateFontInfo DefaultFont = FCoreStyle::GetDefaultFontStyle("Regular", 10);
	
	/**
	 * Editable text to add a new value in the tag list
	 */
	TSharedPtr<SEditableText> EditableTextWidget;
	
	/**
	 * Container of all the tags
	 */
	TSharedPtr<SWrapBox> TagsContainer;

	/**
	 * Border of the 'TagsContainer' wrap
	 */
	TSharedPtr<SBorder> BorderContainer;
	
};
