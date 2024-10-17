#pragma once

#define TAB_CONTENT_COLOR FLinearColor(0.3f, 0.3f, 0.3f, 0.2f)

class FMyTabButtonStyle : public FButtonStyle
{
	FLinearColor DarkGray = FLinearColor(0.3f, 0.3f, 0.3f, 0.1f);
	FLinearColor DarkerGray = FLinearColor(0.3f, 0.3f, 0.3f, 0.2f);
public:
	FMyTabButtonStyle(bool IsActive)
	{
		if(IsActive)
		{
			Normal.TintColor = TAB_CONTENT_COLOR;
			Hovered.TintColor = TAB_CONTENT_COLOR;
			Pressed.TintColor = TAB_CONTENT_COLOR;
		} else
		{
			Normal.TintColor = DarkGray;
			Hovered.TintColor = DarkerGray;
			Pressed.TintColor = DarkerGray;
		}
	}
};

// A Tab Menu that can be used as an Input
template<typename T>
class SInputTabMenuWidget : public SCompoundWidget
{
public:
	class FSlot : public TBasicLayoutWidgetSlot<FSlot>
	{
		SLATE_SLOT_BEGIN_ARGS(FSlot, TBasicLayoutWidgetSlot<FSlot>)
			SLATE_ARGUMENT(FName, Title)
			SLATE_ARGUMENT(T, Value)
		SLATE_END_ARGS()
		
		void Construct(const FChildren& SlotOwner, FSlotArguments&& InArgs)
		{
			TBasicLayoutWidgetSlot<FSlot>::Construct(SlotOwner, MoveTemp(InArgs));
		}
	};

	static typename FSlot::FSlotArguments Slot()
	{
		return typename FSlot::FSlotArguments(MakeUnique<FSlot>());
	}

	DECLARE_DELEGATE_OneParam(FOnSelectionChanged, T)

	SLATE_BEGIN_ARGS( SInputTabMenuWidget ) {}
		SLATE_SLOT_ARGUMENT(SInputTabMenuWidget::FSlot, Slots)

		SLATE_ARGUMENT(T, SelectedTab)
		SLATE_EVENT( FOnSelectionChanged, OnSelectionChanged )
	SLATE_END_ARGS()
	
	void Construct( const FArguments& InArgs )
	{
		//Store params
		this->OnSelectionChanged = InArgs._OnSelectionChanged;
		//Define brush for tab background
		FSlateBrush* BackgroundBrush = new FSlateBrush();
		BackgroundBrush->TintColor = TAB_CONTENT_COLOR;
		//Create basic view
		ChildSlot
		[
			SNew(SVerticalBox)
			+SVerticalBox::Slot()
			.AutoHeight()
			[
				SAssignNew(TabHeader, SHorizontalBox)
			]
			+SVerticalBox::Slot()
			[
				SNew(SOverlay)
				+ SOverlay::Slot()
				[
					SNew(SImage)
					.Image(BackgroundBrush) // No image, we will use a color
				]
				+ SOverlay::Slot()
				.Padding(5.0f)
				[
					SAssignNew(ActiveTabBox, SBox)
				]
			]
		];
		//Attach Slots
		for (const typename FSlot::FSlotArguments& Arg : InArgs._Slots)
		{
			//Get content
			TSharedRef<SWidget> Content = Arg.GetAttachedWidget().ToSharedRef();
			//Create button
			TSharedRef<SButton> Button = SNew(SButton)
				.ButtonStyle(&TabInActiveStyle)
				[
					SNew(SBox)
					.HAlign(HAlign_Center)
					.VAlign(VAlign_Center)
					.Padding(0.0f, 2.0f)
					[
						SNew(STextBlock)
						.Text(FText::FromName(Arg._Title))
					]
				];
			//If initially selected tab is currently handling slot
			if(InArgs._SelectedTab == Arg._Value)
			{
				//Set active tab style
				Button->SetButtonStyle(&TabActiveStyle);
				//Set current content to current content
				ActiveTabBox->SetContent(Content);
			}
			//Create Button click data
			FButtonClickData Data = FButtonClickData(Content, Button, Arg._Title, Arg._Value);
			//Declare on Click handling and pass Button click data
			Button->SetOnClicked(FOnClicked::CreateLambda([this, Data]() -> FReply
			{
				//Set Tab Content
				ActiveTabBox->SetContent(Data.Content);
				//Set InActive Tabs Button Style
				for(TSharedPtr<SButton> Button : ButtonList)
				{
					Button->SetButtonStyle(&TabInActiveStyle);
				}
				//Set Active Tab Button Style
				Data.Button->SetButtonStyle(&TabActiveStyle);
				//Call SelectionChangedEvent
				if(!OnSelectionChanged.ExecuteIfBound(Data.Value))
				{
					UE_LOG(LogTemp, Warning, TEXT("No OnSelectionChange Event bound to Input Tab Menu"));
				}
				//Return Handled Reply
				return FReply::Handled();
			}));
			//Add to tab headers
			TabHeader->AddSlot()
			[
				SNew(SBox)
				//Add padding to right if not last slot
				.Padding(0.0f, 0.0f, InArgs._Slots.Last()._Value == Arg._Value ? 0.0f : 7.0f, 0.0f)
				[
					Button
				]
			];
			//Store button ref
			ButtonList.Add(Button);
		}
	}
private:
	struct FButtonClickData
	{
		TSharedRef<SWidget> Content;
		TSharedPtr<SButton> Button;
		FName Title;
		T Value;

		FButtonClickData(TSharedRef<SWidget> InContent, TSharedPtr<SButton> InButton, const FName InTitle, const T InValue)
		: Content(InContent), Button(InButton), Title(InTitle), Value(InValue)
		{}
	};
	
	TSharedPtr<SHorizontalBox> TabHeader;
	TArray<TSharedPtr<SButton>> ButtonList;
	TSharedPtr<SBox> ActiveTabBox;
	FOnSelectionChanged OnSelectionChanged;
	FMyTabButtonStyle TabActiveStyle = FMyTabButtonStyle(true);
	FMyTabButtonStyle TabInActiveStyle = FMyTabButtonStyle(false);
};