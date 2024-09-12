#include "ConfigurationForm/StaticConfigurationInput.h"

void SStaticConfigurationInput::Construct(const FArguments& InArgs)
{
	LabelText = InArgs._LabelText;
	ChildSlot
	.Padding(0.0f, 0.0f, 0.0f, 10.0f)
	[
		SNew( SHorizontalBox )
		+ SHorizontalBox::Slot()
		.VAlign(VAlign_Center)
		.FillWidth(1)
		[
			SNew( STextBlock )
			.Text( LabelText )
			.Font( LabelFont )
		]
		+ SHorizontalBox::Slot()
		.VAlign(VAlign_Center)
		.FillWidth(5)
		.Padding(5.0f, 0.0f)
		[
			InArgs._Content.Widget
		]
	];
}
