#pragma once

/**
 * The main widget for the environment configuration form
 */
class SEnvironmentConfigurationWidget : public SCompoundWidget
{
public:
	SLATE_BEGIN_ARGS(SEnvironmentConfigurationWidget) {}
	SLATE_END_ARGS()

	void Construct(const FArguments& InArgs);
private:
	/**
	 * The default font style
	 */
	FSlateFontInfo DefaultFont = FCoreStyle::GetDefaultFontStyle("Regular", 10);
};
