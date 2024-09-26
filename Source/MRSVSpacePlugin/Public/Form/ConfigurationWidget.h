#pragma once
#include "Data/ConfigurationDataHandler.h"

class SConfigurationWidget : public SCompoundWidget
{
public:
	SLATE_BEGIN_ARGS(SConfigurationWidget) {}
		SLATE_ARGUMENT(TSharedPtr<ConfigurationDataHandler>, DataHandler)
	SLATE_END_ARGS()

	void Construct(const FArguments& InArgs);
private:
	TSharedPtr<ConfigurationDataHandler> DataHandler;

	FSlateFontInfo DefaultFont = FCoreStyle::GetDefaultFontStyle("Regular", 10);
};
