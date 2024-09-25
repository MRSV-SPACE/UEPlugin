// Copyright Epic Games, Inc. All Rights Reserved.

#include "MRSVSpacePluginStyle.h"
#include "Styling/SlateStyleRegistry.h"
#include "Framework/Application/SlateApplication.h"
#include "Slate/SlateGameResources.h"
#include "Interfaces/IPluginManager.h"
#include "Styling/SlateStyleMacros.h"

#define RootToContentDir Style->RootToContentDir

TSharedPtr<FSlateStyleSet> FMRSVSpacePluginStyle::StyleInstance = nullptr;

void FMRSVSpacePluginStyle::Initialize()
{
	if (!StyleInstance.IsValid())
	{
		StyleInstance = Create();
		FSlateStyleRegistry::RegisterSlateStyle(*StyleInstance);
	}
}

void FMRSVSpacePluginStyle::Shutdown()
{
	FSlateStyleRegistry::UnRegisterSlateStyle(*StyleInstance);
	ensure(StyleInstance.IsUnique());
	StyleInstance.Reset();
}

FName FMRSVSpacePluginStyle::GetStyleSetName()
{
	static FName StyleSetName(TEXT("MRSVSpacePluginStyle"));
	return StyleSetName;
}

TSharedRef< FSlateStyleSet > FMRSVSpacePluginStyle::Create()
{
	TSharedRef< FSlateStyleSet > Style = MakeShareable(new FSlateStyleSet("MRSVSpacePluginStyle"));
	Style->SetContentRoot(IPluginManager::Get().FindPlugin("MRSVSpacePlugin")->GetBaseDir() / TEXT("Resources"));

	Style->Set("MRSVSpacePlugin.OpenPluginWindow", new IMAGE_BRUSH_SVG(TEXT("Logo"), FVector2D(21.0f, 21.0f)));

	return Style;
}

void FMRSVSpacePluginStyle::ReloadTextures()
{
	if (FSlateApplication::IsInitialized())
	{
		FSlateApplication::Get().GetRenderer()->ReloadTextureResources();
	}
}

const ISlateStyle& FMRSVSpacePluginStyle::Get()
{
	return *StyleInstance;
}
