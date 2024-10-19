#include "MRSVSpaceEditorPluginStyle.h"
#include "Styling/SlateStyleRegistry.h"
#include "Framework/Application/SlateApplication.h"
#include "Slate/SlateGameResources.h"
#include "Interfaces/IPluginManager.h"
#include "Styling/SlateStyleMacros.h"

// Defines the root content dir
#define RootToContentDir Style->RootToContentDir

// Define the static style instance attribute
TSharedPtr<FSlateStyleSet> FMRSVSpaceEditorPluginStyle::StyleInstance = nullptr;

void FMRSVSpaceEditorPluginStyle::Initialize()
{
	// Check if the style instance is not valid
	if (!StyleInstance.IsValid())
	{
		// Create the new style instance
		StyleInstance = Create();
		// Register the style in the slate style registry
		FSlateStyleRegistry::RegisterSlateStyle(*StyleInstance);
	}
}

void FMRSVSpaceEditorPluginStyle::Shutdown()
{
	// Unregister the slate style from the registry
	FSlateStyleRegistry::UnRegisterSlateStyle(*StyleInstance);
	// Ensures that no other pointers exist to the slate style instance
	ensure(StyleInstance.IsUnique());
	// Reset the style instance pointer
	StyleInstance.Reset();
}

FName FMRSVSpaceEditorPluginStyle::GetStyleSetName()
{
	// Define and return static name for the slate style
	static FName StyleSetName(TEXT("MRSVSpacePluginStyle"));
	return StyleSetName;
}

TSharedRef<FSlateStyleSet> FMRSVSpaceEditorPluginStyle::Create()
{
	// Create the new slate style set with the given name
	TSharedRef<FSlateStyleSet> Style = MakeShareable(new FSlateStyleSet("MRSVSpacePluginStyle"));
	// Define the content root as the resource forlder in the plugin
	Style->SetContentRoot(IPluginManager::Get().FindPlugin("MRSVSpacePlugin")->GetBaseDir() / TEXT("Resources"));
	// Define the property OpenPluginWindow with the Logo Image
	Style->Set("MRSVSpacePlugin.OpenPluginWindow", new IMAGE_BRUSH_SVG(TEXT("Logo"), FVector2D(21.0f, 21.0f)));
	// Return the new style instance
	return Style;
}

void FMRSVSpaceEditorPluginStyle::ReloadTextures()
{
	//Reload textures if slate app is initialied
	if (FSlateApplication::IsInitialized())
	{
		FSlateApplication::Get().GetRenderer()->ReloadTextureResources();
	}
}

const ISlateStyle& FMRSVSpaceEditorPluginStyle::Get()
{
	return *StyleInstance;
}
