#include "EnvironmentForm/PreviewImageListWidget.h"
#include "Misc/FileHelper.h"
#include "HAL/PlatformFilemanager.h"
#include "Misc/Paths.h"
#include "Modules/ModuleManager.h"

void SPreviewImageListWidget::Construct(const FArguments& InArgs)
{
	//Get Params
	PreviewImagesList = InArgs._PreviewImagesList;

	//Build view
	ChildSlot[
		SAssignNew(ListBox, SVerticalBox)
		+ SVerticalBox::Slot()
		.AutoHeight()
		[
			SAssignNew(PreviewImagePath, STextBlock)
		]
		+ SVerticalBox::Slot()
		.AutoHeight()
		.Padding(0.0f, 5.0f)
		.VAlign(VAlign_Center)
		.HAlign(HAlign_Left)
		[
			SNew(SButton)
			.Text(FText::FromString("Take a new image"))
			.OnClicked_Lambda([this]()
			{
				TakeNewPreviewImage();
				return FReply::Unhandled();
			})
		]
	];

	//Checks if an image already exist in the list 
	if (PreviewImagesList->Num() > 0)
	{
		//Set the path to last item of the list 
		PreviewImagePath->SetText(FText::FromString(PreviewImagesList->Last()));		
	}
}
/*
 * Recreates the preview image  
 */
FReply SPreviewImageListWidget::TakeNewPreviewImage()
{
	//Empty the list since we can only have 1 item for now
	PreviewImagesList->Empty();
	FString ScreenshotPath = TakeScreenshot();
	UpdatePreviewImageText(ScreenshotPath);
		
	return FReply::Handled();
}

void SPreviewImageListWidget::UpdatePreviewImageText(FString ScreenshotPath)
{
	// Makes the path relative to the project folder instead of the full path from the user computer
	FString ProjectDir = FPaths::ProjectDir();  // Folder of the unreal project
	ImagePath = ScreenshotPath;                
	
	//Remove the part before the project folder in the path
	ImagePath.RemoveFromStart(ProjectDir);

	// Verify there's no '/' at the start or the end
	if (ImagePath.StartsWith(TEXT("/")))
	{
		ImagePath.RemoveFromStart(TEXT("/"));
	}
	
	//Create a version with the project name for the TextBlock and one without for the JSON file
	FString ProjectName = FPaths::GetBaseFilename(FPaths::GetProjectFilePath());
	FString fullName = ProjectName + "/" + ImagePath;
	
	// Change the path in the TextBlock and adds it to the list
	PreviewImagePath->SetText(FText::FromString(fullName));
	PreviewImagesList->Add(ImagePath);
}

/**
 * Takes a screenshot
 * @return the path of the preview image that was created 
 */
FString SPreviewImageListWidget::TakeScreenshot()
{
	//Define the folder of where to save the images
	FString ScreenshotDir = FPaths::ProjectSavedDir() / TEXT("MRSV/PreviewImages/");
	IPlatformFile& PlatformFile = FPlatformFileManager::Get().GetPlatformFile();
	
	// Verify if the folder exists or creates it
	if (!PlatformFile.DirectoryExists(*ScreenshotDir))
	{
		PlatformFile.CreateDirectoryTree(*ScreenshotDir);
	}

	//Define the name path (with the name) of the preview image
	FString ScreenshotPath = ScreenshotDir / TEXT("DefaultPreviewImage.jpg");
	
	// Take the screenshot
	FScreenshotRequest::RequestScreenshot(ScreenshotPath, false, false);
	
	// Checks that the rendring was updated
	extern ENGINE_API void FlushRenderingCommands();

	return ScreenshotPath;
}