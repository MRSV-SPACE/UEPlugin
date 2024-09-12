#pragma once

struct FVersion
{
	FString* Tag;
	FString* Revision;
	FString* CreatedAt;

	FVersion(FString* InTag, FString* InRevision, FString* InCreatedAt)
		: Tag(InTag), Revision(InRevision), CreatedAt(InCreatedAt)
	{}
};

struct FPreset
{
	FString* Name;

	FPreset(FString* InName)
		: Name(InName)
	{}
};

UENUM()
enum EControlType : uint8 {
	SLIDER_FREE_HORIZ UMETA(DisplayName = "Free Horizontal Slider"),
	SLIDER_FREE_VERT UMETA(DisplayName = "Free Vertical Slider"),
	SLIDER_NOTCH_HORIZ UMETA(DisplayName = "Notch Horizontal Slider"),
	SLIDER_NOTCH_VERT UMETA(DisplayName = "Notch Vertical Slider"),
	JOYSTICK UMETA(DisplayName = "Joystick"),
	TOGGLE_BTN UMETA(DisplayName = "Toggle Button"),
	TOGGLE_BTN_EXP_OPT UMETA(DisplayName = "Dropdown"),
};

struct FControl
{
	FString* Key;
	FString* Name;
	EControlType* Type;
	long int CurrentValue;
	long int Min;
	long int Max;
	TArray<TSharedPtr<FControl>> Options;

	FControl(FString* InKey, FString* InName, EControlType* InType, long InCurrentValue, long InMin, long InMax, TArray<TSharedPtr<FControl>> InOptions)
		: Key(InKey), Name(InName), Type(InType), CurrentValue(InCurrentValue), Min(InMin), Max(InMax), Options(InOptions)
	{}
};

struct FEnvironment
{
	FString* Name;
	TSharedPtr<FVersion> Version;
	FString* Artist;
	TArray<TSharedPtr<FString>> Preview;
	TArray<TSharedPtr<FPreset>> Presets;
	TArray<TSharedPtr<FControl>> Controls;

	FEnvironment(FString* InName, TSharedPtr<FVersion> InVersion, FString* InArtist, TArray<TSharedPtr<FControl>> InControls)
		: Name(InName), Version(InVersion), Artist(InArtist), Controls(InControls)
	{}
};