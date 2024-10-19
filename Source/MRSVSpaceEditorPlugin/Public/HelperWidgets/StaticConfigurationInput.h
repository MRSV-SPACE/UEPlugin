#pragma once

/**
 * A component for a simple input with a label. 
 */
class SStaticConfigurationInput : public SCompoundWidget
{
public:
	SLATE_BEGIN_ARGS(SStaticConfigurationInput) {}
		// Defines the text for the label
		SLATE_ARGUMENT(FText, LabelText)
		// Defines a slot for the input content
        SLATE_DEFAULT_SLOT(FArguments, Content)
	SLATE_END_ARGS()

	void Construct(const FArguments& InArgs);

private:
	/**
	 * The default label font
	 */
	FSlateFontInfo LabelFont = FCoreStyle::GetDefaultFontStyle("Regular", 10);
};
