#pragma once

class SConfigurationWidget : public SCompoundWidget
{
public:
	SLATE_BEGIN_ARGS(SConfigurationWidget) {}
	SLATE_END_ARGS()

	void Construct(const FArguments& InArgs);
private:
	FSlateFontInfo DefaultFont = FCoreStyle::GetDefaultFontStyle("Regular", 10);
};
