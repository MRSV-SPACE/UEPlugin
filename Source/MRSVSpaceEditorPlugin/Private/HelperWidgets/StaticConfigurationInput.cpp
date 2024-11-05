
#include "HelperWidgets/StaticConfigurationInput.h"

void SStaticConfigurationInput::Construct(const FArguments& InArgs)
{
	ChildSlot
	.Padding(0.0f, 0.0f, 0.0f, 5.0f)
	[
		// Adding a horizontal box
		SNew( SHorizontalBox )
		// Adding a slot for the label
		+ SHorizontalBox::Slot()
		.VAlign(VAlign_Top)
		.HAlign(HAlign_Left)
		// Fill width as 1
		.FillWidth(1)
		[
			SNew( STextBlock )
			.Text( InArgs._LabelText )
			.Font( LabelFont )
		]
		// Adding a slot for the input content
		+ SHorizontalBox::Slot()
		.VAlign(VAlign_Top)
		// Fill width as 5
		.FillWidth(5)
		[
			InArgs._Content.Widget
		]
	];
}
