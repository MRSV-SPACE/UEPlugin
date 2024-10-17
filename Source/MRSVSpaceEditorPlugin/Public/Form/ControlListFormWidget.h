#pragma once
#include "ConfigurationData.h"

class SControlListFormWidget : public SCompoundWidget
{
public:
	SLATE_BEGIN_ARGS(SControlListFormWidget) {}
		SLATE_ARGUMENT(TSharedPtr<TArray<FControl>>, InitialControlList)
	SLATE_END_ARGS()

	void Construct(const FArguments& InArgs);
	
	static TSharedRef<SControlListFormWidget> ShowAsPopup(TSharedPtr<TArray<FControl>> InitalList, FText PopupTitle, FVector2D PopUpSize = FVector2D(800, 400));
private:
	TSharedPtr<SScrollBox> PropertyConfigFormContainer;
	TSharedPtr<TArray<FControl>> InternalControlList;
	
	FSlateFontInfo DefaultFont = FCoreStyle::GetDefaultFontStyle("Regular", 10);
	FReply AddPropertyConfigurationForm() const;
};
