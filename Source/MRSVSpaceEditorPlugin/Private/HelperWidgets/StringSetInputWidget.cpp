#include  "HelperWidgets/StringSetInputWidget.h"
#include "EnvironmentForm/ChoiceSelectionWidget.h"
#include "EnvironmentForm/TagWidget.h"

void SStringSetInputWidget::Construct(const FArguments& InArgs)
{
	//Get params
	StringInputList = InArgs._StringList;
	IsDefault =  InArgs._IsDefault;
	DefaultOptionsList = InArgs._DefaultOptionsList;
	
	//Define rounded border for tag container 
	FSlateBrush* RoundedBrushContainer = new FSlateBrush();
	RoundedBrushContainer->TintColor = FLinearColor(0.3f, 0.3f, 0.3f, 0.1f);
	RoundedBrushContainer->DrawAs = ESlateBrushDrawType::Type::RoundedBox;
	RoundedBrushContainer->OutlineSettings.CornerRadii = FVector4(8.0f, 8.0f, 8.0f, 8.0f);
	RoundedBrushContainer->OutlineSettings.RoundingType = ESlateBrushRoundingType::FixedRadius;
	
	//Struct of the widget
	ChildSlot
	[
		SNew(SHorizontalBox)
		//Tags management section
		+ SHorizontalBox::Slot()
		.VAlign(VAlign_Center)
		.FillWidth(1)
		[
			SNew(SVerticalBox)
			+SVerticalBox::Slot()
			.Padding(0.0f,0.0f,0.0f,5.0f)
			.AutoHeight()
			[
				//Tag container border
				SAssignNew(BorderContainer, SBorder)
			   .Padding(5.0f)
			   .BorderImage(RoundedBrushContainer)
				/*
				 * Function to display the container only when there's at least 1 tag in the list
				 */
			   .Visibility_Lambda([this]() { return Tags.Num() > 0 ? EVisibility::Visible : EVisibility::Collapsed; })
			   [
				   //Tag container
				   SAssignNew(TagsContainer, SWrapBox)
				   .Orientation(EOrientation::Orient_Horizontal)
				   .UseAllottedWidth(true)
			   ]
			]
			+SVerticalBox::Slot()
			[
				//Editable text to add a new value
				SAssignNew(EditableTextWidget, SEditableText)
				.HintText(FText::FromString("Add value..."))
				.Visibility(IsDefault ? EVisibility::Collapsed : EVisibility::Visible)
				.Font(DefaultFont)
				// Enter Key Management
				/*
				 * when Enter is pressed :
				 * - Check if the text isn't empty
				 * - Add the tag through the AddTag function
				 * - Empty the text so it's clear for the next entry
				 * - TODO: Refocus on the SEditableText so the user can directly type the next tag 
				 */
				.OnTextCommitted_Lambda([this](const FText& Text, ETextCommit::Type CommitType)
				{
					if (CommitType == ETextCommit::OnEnter && !Text.IsEmpty())
					{
						AddTag(Text.ToString());
						EditableTextWidget->SetText(FText::GetEmpty());
					}
				})
			]
		]
		//Margin for the container to only go to half of the window
		//(otherwise when adding tag it can go all the way to the right side)
		//Adjust FillWidth number to leave more or less space for the tags
		+SHorizontalBox::Slot()
		.FillWidth(1)
	];

OnConstructCompleted();
}

/*
 * Once the widget is fully constructed, this function verify if some tags already exists
 * If yes, they're all added visually to the container
 */
void SStringSetInputWidget::OnConstructCompleted()
{
	/*
	 *Temporary array to contain the values.
	 *(since we use the AddTag function, it will add the items to the String list.
	 *using a temp array prevent issue caused by modifying the list while using it to loop)
	 */
	TArray<FString>* tempItems = nullptr;

	//If the TagsContainer is valid, checked if we're dealing with default controls or not
	if (TagsContainer.IsValid())
	{
		if (!IsDefault)
		{
			tempItems = StringInputList;
			//Loop through the list to add each tag to the container 
			for (FString existingTag : *tempItems)
			{
				AddTag(existingTag);
			}
		}
		else
		{
			//Use the default options list to add all the possible options
			tempItems = DefaultOptionsList;
			for (FString existingTag : *tempItems)
			{
				//checks if the current choice exist in the StringInputList.
				//If True -> the choice needs to be checked
				//If False -> the choice needs to be unchecked
				bool isChecked = false;
				StringInputList->Contains(existingTag) ? isChecked = true : isChecked = false;
				AddChoiceSelection(existingTag, isChecked);
			}
		}
	}
}

/*
 * Add a tag visually and in the list (first in the list and then in the container)
 * const FString& Text :value of the tag
 */
void SStringSetInputWidget::AddTag(const FString& Text)
{
	//Checks if the container is valid
	if (TagsContainer.IsValid())
	{
		//Normalizing the text 
		FString NormalizedTag = Text.ToLower();

		/*
		 *Checks if the normalized string exists already or not
		 *(prevent having same tag twice in the list while authorising user to use upper case)
		 *Compare in lower case and stock values the same way the user wrote it
		 *Example : Test = teSt = TEST = test / but only one of those be use (first one enter)
		*/
		if (!NormalizeTags.Contains(NormalizedTag))
		{
			//Add the tag to the different lists
			Tags.Add(*Text);
			NormalizeTags.Add(NormalizedTag);
			*StringInputList = Tags;
			
			//Add the tag visually
			TagsContainer->AddSlot()
			.Padding(FMargin(5.0f))
			[
				SNew(STagWidget)
				.TagName(Text)
				//Remove button management
				.OnRemove_Lambda([this, Text](TSharedRef<SWidget> TagWidget)
				{
					//Remove from all the lists
					Tags.Remove(Text);
					NormalizeTags.Remove(Text.ToLower());
					*StringInputList = Tags;
					//Remove from the container
					TagsContainer->RemoveSlot(TagWidget);
				})
			];
		}
	}
}

/* Add a choice to the selection
 *
 */
void SStringSetInputWidget::AddChoiceSelection(const FString& Text, bool isChecked)
{
	
	//Checks if the container is valid
	if (TagsContainer.IsValid())
	{
		//The choice still needs to be in the tags list since the visibility is based on the tags length
		Tags.Add(Text);
		//Add the tag visually
		TagsContainer->AddSlot()
		.Padding(FMargin(5.0f))
		[
			SNew(SChoiceSelectionWidget)
			.ChoiceName(Text)
			//Use of the StringInputList so we can have a list with all the possible choices and one with the checked ones
			.ChoiceList(StringInputList)
			.IsChecked(isChecked)
		];
	}
}