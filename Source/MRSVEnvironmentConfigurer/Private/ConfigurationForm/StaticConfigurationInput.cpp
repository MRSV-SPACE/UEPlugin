#include "ConfigurationForm/StaticConfigurationInput.h"

void SStaticConfigurationInput::Construct(const FArguments& InArgs)
{
	LabelText = InArgs._LabelText;
	ChildSlot
	[
		SNew(SHorizontalBox)
		+SHorizontalBox::Slot()
		.MaxWidth(300.0f)
		.Padding(2.0f)
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
			.FillWidth(1)
			.Padding(5.0f, 0.0f)
			[
				SNew( SBorder )
				.Padding(5.0f)
				.BorderBackgroundColor(FColor::White)
				[
					SAssignNew( InputWidget, SEditableText )
					.HintText(LabelText)
				]
			]
		]
	];
}
