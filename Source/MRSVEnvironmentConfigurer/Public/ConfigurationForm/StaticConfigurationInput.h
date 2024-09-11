#pragma once

class SStaticConfigurationInput : public SCompoundWidget
{
public:
	SLATE_BEGIN_ARGS(SStaticConfigurationInput) {}
		SLATE_ARGUMENT(FText, LabelText)
	SLATE_END_ARGS()

	void Construct(const FArguments& InArgs);

private:
	TSharedPtr<SWidget> InputWidget;

	FText LabelText;

	//Label font
	FSlateFontInfo LabelFont = FCoreStyle::GetDefaultFontStyle("Regular", 10);
};
