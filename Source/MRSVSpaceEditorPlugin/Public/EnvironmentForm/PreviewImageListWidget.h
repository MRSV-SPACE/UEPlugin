#pragma once
#include "IImageWrapper.h"

class SPreviewImageListWidget : public SCompoundWidget
{
public:
	SLATE_BEGIN_ARGS(SPreviewImageListWidget) {}
	//Pointer to the list of preview images path
	SLATE_ARGUMENT(TArray<FString>*, PreviewImagesList);
	SLATE_END_ARGS()

	void Construct(const FArguments& InArgs);

private:
	/**
	 * The list of preview images
	 */
	TArray<FString>* PreviewImagesList = nullptr;

	/**
	 * Pointer to the container of preview's paths
	 */
	TSharedPtr<SVerticalBox> ListBox = nullptr;

	/**
	 * Path the default image 
	 */
	FString ImagePath;

	/**
	 * Pointer to the TextBlock of the default image path
	 */
	TSharedPtr<STextBlock> PreviewImagePath = nullptr;

	/**
	 * Function of the button "Take a new image"
	 * Changes the current default preview image
	 * @return The reply if the event is handled (Used for handled events)
	 */
	FReply TakeNewPreviewImage();

	/**
	 * Update the path of the default image in the TextBlock PreviewImagePath
	 * @param ScreenshotPath The current path of the screenshot
	 */
	void UpdatePreviewImageText(FString ScreenshotPath);

	/**
	 * Takes a screenshot based of the editor
	 * @return The path of the taken screenshot
	 */
	FString TakeScreenshot();
};
