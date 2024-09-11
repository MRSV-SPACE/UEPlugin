#pragma once

class SConfigurationFormWidget : public SCompoundWidget
{
public:
	SLATE_BEGIN_ARGS(SConfigurationFormWidget) {}
	SLATE_END_ARGS()

	void Construct(const FArguments& InArgs);

	FReply AddPropertyConfigurationForm() const;
	FReply SaveConfiguration() const;
private:
	TSharedPtr<SScrollBox> PropertyConfigFormContainer;
};
