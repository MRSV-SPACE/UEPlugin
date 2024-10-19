#pragma once

/**
 * The background color of the active tab content
 */
#define CONTENT_BACKGROUND_COLOR FLinearColor(0.3f, 0.3f, 0.3f, 0.2f)

/**
 * The button style for the tab buttons for both active and inactive states
 */
class FInputTabButtonStyle : public FButtonStyle
{

	/**
	 * The color for the tab button if active (Should be same as content background)
	 */
	FLinearColor ActiveTabButtonColor = CONTENT_BACKGROUND_COLOR;
		
	/**
	 * The color for the tab button if inactive
	 */
	FLinearColor InActiveTabButtonColor = FLinearColor(0.3f, 0.3f, 0.3f, 0.1f);

	/**
	 * The color for the tab button if inactive but interacting (hovering, pressing)
	 */
	FLinearColor InActiveTabButtonColorInteract = FLinearColor(0.3f, 0.3f, 0.3f, 0.2f);
	
public:

	/**
	 * Creates a new Input Tab Button Style.
	 *
	 * @param IsActive Defines if the button style should be for an active or inactive button
	 */
	FInputTabButtonStyle(bool IsActive)
	{
		if(IsActive)
		{
			Normal.TintColor = ActiveTabButtonColor;
			Hovered.TintColor = ActiveTabButtonColor;
			Pressed.TintColor = ActiveTabButtonColor;
		} else
		{
			Normal.TintColor = InActiveTabButtonColor;
			Hovered.TintColor = InActiveTabButtonColorInteract;
			Pressed.TintColor = InActiveTabButtonColorInteract;
		}
	}
};

/**
 * A widget for a tab menu, that also serves as an input.
 *
 * @tparam T The type of the value each tab represents
 */
template<typename T>
class SInputTabMenuWidget : public SCompoundWidget
{
public:
	
	/**
	 * The slot widget for each tab
	 */
	class FSlot : public TBasicLayoutWidgetSlot<FSlot>
	{
		SLATE_SLOT_BEGIN_ARGS(FSlot, TBasicLayoutWidgetSlot<FSlot>)
			// The title of the tab
			SLATE_ARGUMENT(FName, Title)
			// The underlying value of the tab
			SLATE_ARGUMENT(T, Value)
		SLATE_END_ARGS()
		
		void Construct(const FChildren& SlotOwner, FSlotArguments&& InArgs)
		{
			TBasicLayoutWidgetSlot<FSlot>::Construct(SlotOwner, MoveTemp(InArgs));
		}
	};

	/**
	 * A static function to create a new slot item for the tab menu
	 *
	 * @return The new slot item created
	 */
	static typename FSlot::FSlotArguments Slot()
	{
		return typename FSlot::FSlotArguments(MakeUnique<FSlot>());
	}

	/**
	 * The event delegate type for the selection change. Contains one parameter for the value selected
	 */
	DECLARE_DELEGATE_OneParam(FOnSelectionChanged, T)

	SLATE_BEGIN_ARGS( SInputTabMenuWidget ) {}
		// Argument for the slots to input
		SLATE_SLOT_ARGUMENT(SInputTabMenuWidget::FSlot, Slots)
		
		// The selected tab by default
		SLATE_ARGUMENT(T, SelectedTab)
		
		// The event triggered when the selection changes
		SLATE_EVENT( FOnSelectionChanged, OnSelectionChanged )
	SLATE_END_ARGS()
	
	void Construct( const FArguments& InArgs )
	{
		//Store params
		this->OnSelectionChanged = InArgs._OnSelectionChanged;
		//Define brush for tab background
		FSlateBrush* BackgroundBrush = new FSlateBrush();
		BackgroundBrush->TintColor = CONTENT_BACKGROUND_COLOR;
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
	/**
	 * Internal struct for the data passed to the tab button clicked event, containing all needed values
	 * to change the content, styles and trigger the selection changed event
	 */
	struct FButtonClickData
	{
		/**
		 * The content to show for this tab item
		 */
		TSharedRef<SWidget> Content;
		
		/**
		 * The button clicked itself
		 */
		TSharedPtr<SButton> Button;

		/**
		 * The title of the tab
		 */
		FName Title;

		/**
		 * The value of the tab item slot
		 */
		T Value;

		FButtonClickData(TSharedRef<SWidget> InContent, TSharedPtr<SButton> InButton, const FName InTitle, const T InValue)
		: Content(InContent), Button(InButton), Title(InTitle), Value(InValue)
		{}
	};

	/**
     * The widget container of the tab header (tab buttons)
     */
	TSharedPtr<SHorizontalBox> TabHeader;

	/**
	 * The list of tab buttons
	 */
	TArray<TSharedPtr<SButton>> ButtonList;

	/**
	 * The box containing the active tab content
	 */
	TSharedPtr<SBox> ActiveTabBox;

	/**
	 * The delegate for the selection changed event
	 */
	FOnSelectionChanged OnSelectionChanged;

	/**
	 * The style for active tab buttons
	 */
	FInputTabButtonStyle TabActiveStyle = FInputTabButtonStyle(true);

	/**
	 * The style for inactive tab buttons
	 */
	FInputTabButtonStyle TabInActiveStyle = FInputTabButtonStyle(false);
};