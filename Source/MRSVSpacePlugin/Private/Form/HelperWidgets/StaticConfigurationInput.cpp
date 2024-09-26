
#include "Form/HelperWidgets/StaticConfigurationInput.h"

void SStaticConfigurationInput::Construct(const FArguments& InArgs)
{
	ChildSlot
	.Padding(0.0f, 0.0f, 0.0f, 5.0f)
	[
		SNew( SHorizontalBox )
		+ SHorizontalBox::Slot()
		.VAlign(VAlign_Top)
		.HAlign(HAlign_Left)
		.FillWidth(1)
		[
			SNew( STextBlock )
			.Text( InArgs._LabelText )
			.Font( LabelFont )
		]
		+ SHorizontalBox::Slot()
		.VAlign(VAlign_Top)
		.FillWidth(5)
		[
			InArgs._Content.Widget
		]
	];
}
