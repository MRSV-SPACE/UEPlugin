#include "Form/HelperWidgets/TabMenuWidget.h"

void STabMenuWidget::Construct(const FArguments& InArgs)
{
	ChildSlot
	[
		SNew(SVerticalBox)
		+SVerticalBox::Slot()
		.AutoHeight()
		[
			SAssignNew(TabHeader, SHorizontalBox)
		]
		+SVerticalBox::Slot()
		.Padding(5.0f)
		[
			SAssignNew(ActiveTabBox, SBox)
		]
	];
	for (const FSlot::FSlotArguments& Arg : InArgs._Slots)
	{
		//Get content
		TSharedRef<SWidget> Content = Arg.GetAttachedWidget().ToSharedRef();
		//Create button
		TSharedRef<SButton> Button = SNew(SButton)
			[
				SNew(SHorizontalBox)
				+ SHorizontalBox::Slot()
				.HAlign(HAlign_Center)
				.VAlign(VAlign_Center)
				.FillWidth(1)
				[
					SNew(STextBlock)
					.Text(FText::FromName(Arg._Title))
				]
			];
		FButtonClickData Data = FButtonClickData(Content, Button, Arg._Title);
		Button->SetOnClicked(FOnClicked::CreateLambda([this, Data]() -> FReply
		{
			ActiveTabBox->SetContent(Data.Content);
			for(TSharedPtr<SButton> Button : ButtonList)
			{
				SetButtonColor(Button, false);
			}
			SetButtonColor(Data.Button, true);
			return FReply::Handled();
		}));
		//Add to tab headers
		TabHeader->AddSlot()
		[
			Button
		];
		//Store button ref
		ButtonList.Add(Button);
	}
	ActiveTabBox->SetContent(InArgs._Slots[0].GetAttachedWidget().ToSharedRef());
}

void STabMenuWidget::SetButtonColor(TSharedPtr<SButton> Button, bool IsActive)
{
	if (IsActive)
	{
		Button->SetEnabled(false);
	} else
	{
		Button->SetEnabled(true);
	}
}

