#pragma once

#include "CoreMinimal.h"
#include "Styling/SlateStyle.h"

/**
 * Style object for the editor plugin registration
 */
class FMRSVSpaceEditorPluginStyle
{
public:

	/**
	 * Initializes the style definition
	 */
	static void Initialize();

	/**
	 * Clears up unused data and shutdowns style
	 */
	static void Shutdown();

	/**
	 * Reloads textures used by slate renderer
	 */
	static void ReloadTextures();

	/**
	 * Get the style
	 *
	 * @return The Slate style set for the Shooter game
	 */
	static const ISlateStyle& Get();

	/**
	 * Get the style name
	 *
	 * @return The slate style name
	 */
	static FName GetStyleSetName();

private:

	/**
	 * Creates the MRSV plugin style
	 *
	 * @return The Slate style set of the new style object
	 */
	static TSharedRef< FSlateStyleSet > Create();

	/**
	 * The Style instance
	 */
	static TSharedPtr< FSlateStyleSet > StyleInstance;
};