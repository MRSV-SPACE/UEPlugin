#include "EnvironmentForm/TagWidget.h"

#include "HelperWidgets/StringSetInputWidget.h"

void STagWidget::Construct(const FArguments& InArgs)
{
	//Define rounded border for tag widget
	FSlateBrush* RoundedBrushTag = new FSlateBrush();
	RoundedBrushTag->TintColor = FLinearColor(0.3f, 0.3f, 0.3f, 0.3f);
	RoundedBrushTag->DrawAs = ESlateBrushDrawType::Type::RoundedBox;
	RoundedBrushTag->OutlineSettings.CornerRadii = FVector4(8.0f, 8.0f, 8.0f, 8.0f);
	RoundedBrushTag->OutlineSettings.RoundingType = ESlateBrushRoundingType::FixedRadius;
	
	//Get the Tag name from the Args
	TagName = InArgs._TagName;  

	//Get the RemoveCallback from the Args
	RemoveCallback = InArgs._OnRemove;

	//Struct of the TagWidget
	ChildSlot
	[
		SNew(SBorder)
		.BorderImage(RoundedBrushTag)
		.Padding(5.0f, 3.0f)
		[
			SNew(SHorizontalBox)
			+ SHorizontalBox::Slot()
			.AutoWidth()
			.VAlign(VAlign_Center)
			.Padding(0.0f, 0.0f, 4.0f, 0)
			[
				//Textblock with the text value of the tag
				SNew(STextBlock)
				.Text(FText::FromString(TagName))
			]
			+ SHorizontalBox::Slot()
			.VAlign(VAlign_Center)
			.AutoWidth()
			[
				// Remove tag button wrapped in SBox to control size
				SNew(SBox)
				.WidthOverride(32.0f)  // Smallest size possible 32.0f (for visual reason)
				.HeightOverride(20.0f) // Smallest size possible 20.0f (for visual reason)
				[
					//Remove button
					SNew(SButton)
					.Text(FText::FromString("x"))
					.ButtonColorAndOpacity(FLinearColor::Red)
					.TextStyle(FCoreStyle::Get(), "NormalText")
					.ForegroundColor(FLinearColor::White) 
					.ContentPadding(FMargin(0)) 
					.OnClicked(this, &STagWidget::OnRemoveClicked)
				]
			]
		]
		
	];
}

/**
 * Remove the Widget when the 'x' button is clicked
 *
 * @return Reply for handled events
 */
FReply STagWidget::OnRemoveClicked()
{
	if (RemoveCallback.ExecuteIfBound(AsShared()))
	{
		UE_LOG(LogTemp, Display, TEXT("TagWidget successfully removed"));
		return FReply::Handled();
	}
	UE_LOG(LogTemp, Warning, TEXT("No callback registred for removing tag"))
	return FReply::Unhandled();
}
