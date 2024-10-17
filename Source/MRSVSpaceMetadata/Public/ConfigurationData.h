#pragma once

#include "CoreMinimal.h"
#include "ConfigurationData.generated.h"

USTRUCT()
struct FEnvironmentVersion
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere)
	FString Tag;
	
	UPROPERTY(EditAnywhere)
	FString Revision;

	UPROPERTY(EditAnywhere)
	FDateTime CreatedAt;

	FEnvironmentVersion(const FString& InTag, const FString& InRevision, const FDateTime& InCreatedAt)
		: Tag(InTag), Revision(InRevision), CreatedAt(InCreatedAt) {}

	FEnvironmentVersion(const FString& InTag, const FString& InRevision)
		: Tag(InTag), Revision(InRevision), CreatedAt(FDateTime::UtcNow()) {}

	FEnvironmentVersion()
		: CreatedAt(FDateTime::UtcNow()) {}
};

USTRUCT()
struct FPreset
{
	GENERATED_BODY()
	
	UPROPERTY(EditAnywhere)
	FString Name;

	UPROPERTY(EditAnywhere)
	FString Id;

	bool operator==(const FPreset& Other) const;

	FPreset(const FString& InId, const FString& InName)
		: Name(InName), Id(InId) {}

	FPreset(const FString& InName)
		: Name(InName), Id(FGuid::NewGuid().ToString(EGuidFormats::DigitsWithHyphensLower)) {}

	FPreset()
		: Id(FGuid::NewGuid().ToString(EGuidFormats::DigitsWithHyphensLower)) {}
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

UENUM()
enum EControlActionType {
	ACTOR,
	BLUEPRINT,
};

USTRUCT()
struct FControlAction
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere)
	TEnumAsByte<EControlActionType> Type;

	UPROPERTY(EditAnywhere)
	FString Actor;
	
	UPROPERTY(EditAnywhere)
	FString Property;

	FControlAction(const EControlActionType& InType, const FString& InActor, const FString& InProperty)
		: Type(InType), Actor(InActor), Property(InProperty) {}

	FControlAction(const EControlActionType& InType)
		: Type(InType) {}

	FControlAction()
		: Type() {}
};

// Workaround to use self reflection without a pointer. Pointers would leed to much overhead in code
// as all the forms are working with direct references to the metadata object to make the save and loading as easy as possible
typedef struct FControl FControl;

USTRUCT()
struct FControlDetails
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere)
	double CurrentValue;

	UPROPERTY(EditAnywhere)
	double Min;

	UPROPERTY(EditAnywhere)
	double Max;

	// Never expose as UPROPERTY, as Unreal can't handle pointer fields in USTRUCT
	// Needs to be converted manually to JSON
	TArray<FControl> Options;

	UPROPERTY(EditAnywhere)
	FString Selected;

	UPROPERTY(EditAnywhere)
	bool Toggled;

	// Constructor for sliders
	FControlDetails(const double& InCurrentValue, const double& InMin, const double& InMax)
		: CurrentValue(InCurrentValue), Min(InMin), Max(InMax), Toggled() {}

	// Constructor for Expanded Options
	FControlDetails(const TArray<FControl>& InOptions, const FString& InSelected)
		: CurrentValue(), Min(), Max(), Options(InOptions), Selected(InSelected), Toggled() {}

	// Constructor for Joystick
	FControlDetails(const double& InCurrentValue)
		: CurrentValue(InCurrentValue), Min(), Max(), Toggled() {}

	// Constructor for Toggle Button
	FControlDetails(const bool InToggled)
		: CurrentValue(), Min(), Max(), Toggled(InToggled) {}

	FControlDetails()
		: CurrentValue(), Min(), Max(), Toggled() {}
};

USTRUCT()
struct FControl
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere)
	FString Name;

	UPROPERTY(EditAnywhere)
	TEnumAsByte<EControlType> Type = TEnumAsByte<EControlType>();

	UPROPERTY(EditAnywhere)
	FControlAction Action;

	UPROPERTY(EditAnywhere)
	FControlDetails Details;

	UPROPERTY(EditAnywhere)
	FString Id;

	bool operator==(const FControl& Other) const;

	FControl(const FString &InName, const EControlType &InType, const FControlAction &InAction, const FControlDetails &InDetails, const FString &InId)
		: Name(InName), Type(InType), Action(InAction), Details(InDetails), Id(InId) {}

	FControl(const FString &InName, const EControlType &InType, const FControlAction &InAction, const FControlDetails &InDetails)
		: Name(InName), Type(InType), Action(InAction), Details(InDetails), Id(FGuid::NewGuid().ToString(EGuidFormats::DigitsWithHyphensLower)) {}

	FControl()
		: Action(FControlAction()), Details(FControlDetails()), Id(FGuid::NewGuid().ToString(EGuidFormats::DigitsWithHyphensLower)) {}
};

USTRUCT()
struct FEnvironment
{
	GENERATED_BODY()
	
	UPROPERTY(EditAnywhere)
	FString Name;
	
	UPROPERTY(EditAnywhere)
	FEnvironmentVersion Version;

	UPROPERTY(EditAnywhere)
	TArray<FString> Previews;

	UPROPERTY(EditAnywhere)
	TArray<FPreset> Presets;

	UPROPERTY(EditAnywhere)
	TArray<FControl> Controls;

	UPROPERTY(EditAnywhere)
	FString Id;

	FEnvironment(const FString &InName, const FEnvironmentVersion &InVersion, const TArray<FString> &InPreviews, const TArray<FPreset> &InPresets, const TArray<FControl> &InControls, const FString& Id)
		: Name(InName), Version(InVersion), Previews(InPreviews), Presets(InPresets), Controls(InControls), Id(FGuid::NewGuid().ToString(EGuidFormats::DigitsWithHyphensLower)) {}

	FEnvironment()
		: Name(""), Version(FEnvironmentVersion()), Previews(TArray<FString>()), Presets(TArray<FPreset>()), Controls(TArray<FControl>()), Id(FGuid::NewGuid().ToString(EGuidFormats::DigitsWithHyphensLower)) {}
};

uint32 GetTypeHash(const FPreset& Preset);
uint32 GetTypeHash(const FControl& Control);