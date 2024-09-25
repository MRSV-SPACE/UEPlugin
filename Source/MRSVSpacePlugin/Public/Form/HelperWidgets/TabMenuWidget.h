#pragma once

class STabMenuWidget : public SCompoundWidget
{
public:
	class FSlot : public TBasicLayoutWidgetSlot<FSlot>
	{
		SLATE_SLOT_BEGIN_ARGS(FSlot, TBasicLayoutWidgetSlot<FSlot>)
			SLATE_ARGUMENT(FName, Title)
		SLATE_END_ARGS()
		
		void Construct(const FChildren& SlotOwner, FSlotArguments&& InArgs)
		{
			TBasicLayoutWidgetSlot::Construct(SlotOwner, MoveTemp(InArgs));
		}
	};

	static FSlot::FSlotArguments Slot()
	{
		return FSlot::FSlotArguments(MakeUnique<FSlot>());
	}

	SLATE_BEGIN_ARGS( STabMenuWidget ) {}
		SLATE_SLOT_ARGUMENT(STabMenuWidget::FSlot, Slots)
	SLATE_END_ARGS()
	
	void Construct( const FArguments& InArgs );
private:
	struct FButtonClickData
	{
		TSharedRef<SWidget> Content;
		TSharedPtr<SButton> Button;
		FName Title;

		FButtonClickData(TSharedRef<SWidget> InContent, TSharedPtr<SButton> InButton, FName InTitle)
		: Content(InContent), Button(InButton), Title(InTitle)
		{}
	};
	
	TSharedPtr<SHorizontalBox> TabHeader;
	TArray<TSharedPtr<SButton>> ButtonList;
	TSharedPtr<SBox> ActiveTabBox;

	void SetButtonColor(TSharedPtr<SButton> Button, bool IsActive);
};

