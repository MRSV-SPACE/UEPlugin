#pragma once

#include "CoreMinimal.h"
#include "ConfigurationData.generated.h"

/**
 * Struct for the version of an environment
 */
USTRUCT()
struct FEnvironmentVersion
{
	GENERATED_BODY()

	/**
	 * The tag (e.g. version name) of the version
	 */
	UPROPERTY(EditAnywhere)
	FString Tag;

	/**
	 * The description of the version (revision text)
	 */
	UPROPERTY(EditAnywhere)
	FString Revision;

	/**
	 * The date time when the version was released. 
	 */
	UPROPERTY(EditAnywhere)
	FDateTime CreatedAt;

	FEnvironmentVersion(const FString& InTag, const FString& InRevision, const FDateTime& InCreatedAt)
		: Tag(InTag), Revision(InRevision), CreatedAt(InCreatedAt) {}

	FEnvironmentVersion(const FString& InTag, const FString& InRevision)
		: Tag(InTag), Revision(InRevision), CreatedAt(FDateTime::UtcNow()) {}

	FEnvironmentVersion()
		: CreatedAt(FDateTime::UtcNow()) {}
};

/**
 * Struct for the preset of an environment
 */
USTRUCT()
struct FPreset
{
	GENERATED_BODY()

	/**
	 * The unique id of the preset
	 */
	UPROPERTY(EditAnywhere)
	FString Id;
	
	/**
	 * The name / title of the preset
	 */
	UPROPERTY(EditAnywhere)
	FString Name;

	/**
	 * The stored values of the preset
	 */
	UPROPERTY(EditAnywhere)
	TMap<FString, FString> Values;

	FPreset(const FString& InId, const FString& InName, const TMap<FString, FString>& InValues)
		: Id(InId), Name(InName), Values(InValues) {}

	FPreset(const FString& InName, const TMap<FString, FString>& InValues)
		: Id(FGuid::NewGuid().ToString(EGuidFormats::DigitsWithHyphensLower)), Name(InName), Values(InValues) {}

	FPreset()
		: Id(FGuid::NewGuid().ToString(EGuidFormats::DigitsWithHyphensLower)), Name("") {}

	/**
	 * Defines the compare operator (compares using the ID field)
	 */
	bool operator==(const FPreset& Other) const;
};

/**
 * Enum for the available control types with their DisplayName. 
 */
UENUM()
enum EControlType : uint8 {
	
	/* Defines a free horizontal slider */
	SLIDER_FREE_HORIZONTAL UMETA(DisplayName = "Free Horizontal Slider"),
	
	/* Defines a free vertical slider */
	SLIDER_FREE_VERTICAL UMETA(DisplayName = "Free Vertical Slider"),

	/* Defines a notched horizontal slider */
	SLIDER_NOTCH_HORIZONTAL UMETA(DisplayName = "Notch Horizontal Slider"),

	/* Defines a notched vertical slider */
	SLIDER_NOTCH_VERTICAL UMETA(DisplayName = "Notch Vertical Slider"),
	
	/* Defines a joystick input*/
	JOYSTICK UMETA(DisplayName = "Joystick"),

	/* Defines a on / off toggle button */
	TOGGLE UMETA(DisplayName = "Toggle Button"),

	/* Defines a container for other controls */
	CONTAINER UMETA(DisplayName = "Container"),

	/* Defines an option chooser (radio / dropdown like) */
	CHOICE UMETA(DisplayName = "Choice Selector")
};

/**
 * Enum for the available control action types. 
 */
UENUM()
enum EControlActionType {

	/* No Action defined (Only used for container control )*/
	NONE,
	
	/* Defines the action updating a specific property on an actor in scene */
	ACTOR,

	/* Defines the action is implemented in blueprint */
	BLUEPRINT
};

/**
 * Struct for a control action. The action defines how a value change of the control is executed
 * in Unreal Engine. 
 */
USTRUCT()
struct FControlAction
{
	GENERATED_BODY()

	/**
	 * The action type (as byte for JSON parsing)
	 */
	UPROPERTY(EditAnywhere)
	TEnumAsByte<EControlActionType> Type;

	/**
	 * The actor (PathName of actor) for the action type "actor"
	 */
	UPROPERTY(EditAnywhere)
	FString Actor;

	/**
	 * The property name for the action type "actor"
	 */
	UPROPERTY(EditAnywhere)
	FString Property;

	FControlAction(const EControlActionType& InType, const FString& InActor, const FString& InProperty)
		: Type(InType), Actor(InActor), Property(InProperty) {}

	FControlAction(const EControlActionType& InType)
		: Type(InType) {}

	FControlAction()
		// Set actor as default action type
		: Type(ACTOR) {}
};

// Workaround to use self reflection without a pointer. Pointers would leed to much overhead in code
// as all the forms are working with direct references to the metadata object to make the save and loading as easy as possible
typedef struct FControl FControl;

/**
 * Struct for the control details (basically all fields are optional, defined
 * properties change with control type)
 */
USTRUCT()
struct FControlDetails
{
	GENERATED_BODY()

	/**
	 * The minimum value of the control (used for slider controls)
	 */
	UPROPERTY(EditAnywhere)
	double Min;

	/**
	 * The maximum value of the control (used for slider controls)
	 */
	UPROPERTY(EditAnywhere)
	double Max;

	/**
	 * A list of options (used for radio control)
	 */
	UPROPERTY(EditAnywhere)
	TArray<FString> Options;

	/**
	 * A list of controls (used for container control)
	 *
	 * Important: Never expose as UPROPERTY, as Unreal can't handle pointer fields in USTRUCT
	 * Note: Needs to be converted manually to JSON as isn't part of the USTRUCT
	 */
	TArray<FControl> Controls;

	/**
	 * Constructor for slider type controls
	 *
	 * @param InMin The minimum value of the slider
	 * @param InMax The maximum value of the slider
	 */
	FControlDetails(const double& InMin, const double& InMax)
		: Min(InMin), Max(InMax) {}

	/**
	 * Constructor for container type control
	 *
	 * @param InControls The list of controls the control contains
	 */
	FControlDetails(const TArray<FControl>& InControls)
		: Min(), Max(), Controls(InControls) {}

	/**
	 * Constructor for radio type control
	 *
	 * @param InOptions The options available in the radio / dropdown input
	 */
	FControlDetails(const TArray<FString>& InOptions)
		: Min(), Max(), Options(InOptions) {}

	/**
	 * Constructor for joystick type control and default constructor.
	 * Default set min 0 and max 100
	 */
	FControlDetails()
		: Min(0), Max(100) {}
};

/**
 * Struct for a control of an environment
 */
USTRUCT()
struct FControl
{
	GENERATED_BODY()

	/**
     * The unique ID of the control
     */
	UPROPERTY(EditAnywhere)
	FString Id;

	/**
	 * The name of the environment
	 */
	UPROPERTY(EditAnywhere)
	FString Name;

	/**
	 * The type of the control (as byte for JSON parsing)
	 */
	UPROPERTY(EditAnywhere)
	TEnumAsByte<EControlType> Type = TEnumAsByte<EControlType>();

	/**
	 * The action definition of the control
	 */
	UPROPERTY(EditAnywhere)
	FControlAction Action;

	/**
	 * The details of the control
	 */
	UPROPERTY(EditAnywhere)
	FControlDetails Details;

	FControl(const FString &InName, const EControlType &InType, const FControlAction &InAction, const FControlDetails &InDetails, const FString &InId)
		: Id(InId), Name(InName), Type(InType), Action(InAction), Details(InDetails) {}

	FControl(const FString &InName, const EControlType &InType, const FControlAction &InAction, const FControlDetails &InDetails)
		: Id(FGuid::NewGuid().ToString(EGuidFormats::DigitsWithHyphensLower)), Name(InName), Type(InType), Action(InAction), Details(InDetails) {}

	FControl()
		: Id(FGuid::NewGuid().ToString(EGuidFormats::DigitsWithHyphensLower)), Name(""), Action(FControlAction()), Details(FControlDetails()) {}

	/**
	 * Defines the compare operator (compares using the ID field)
	 */
	bool operator==(const FControl& Other) const;
};

/**
 * Struct for an environment definition
 */
USTRUCT()
struct FEnvironment
{
	GENERATED_BODY()

	/**
	 * The unique ID of the environment
	 */
	UPROPERTY(EditAnywhere)
	FString Id;

	/**
	 * The name of the environment
	 */
	UPROPERTY(EditAnywhere)
	FString Name;

	/**
	 * The current environment version
	 */
	UPROPERTY(EditAnywhere)
	FEnvironmentVersion Version;

	/**
	 * The list of preview file paths for this environment
	 */
	UPROPERTY(EditAnywhere)
	TArray<FString> Previews;

	/**
	 * The list of artist defined presets of this environment
	 */
	UPROPERTY(EditAnywhere)
	TArray<FPreset> Presets;

	/**
	 * The list of controls, this environment exposes
	 */
	UPROPERTY(EditAnywhere)
	TArray<FControl> Controls;

	FEnvironment(const FString &InName, const FEnvironmentVersion &InVersion, const TArray<FString> &InPreviews, const TArray<FPreset> &InPresets, const TArray<FControl> &InControls, const FString& Id)
		: Id(FGuid::NewGuid().ToString(EGuidFormats::DigitsWithHyphensLower)), Name(InName), Version(InVersion), Previews(InPreviews), Presets(InPresets), Controls(InControls) {}

	FEnvironment()
		: Id(FGuid::NewGuid().ToString(EGuidFormats::DigitsWithHyphensLower)), Name(""), Version(FEnvironmentVersion()), Previews(TArray<FString>()), Presets(TArray<FPreset>()), Controls(TArray<FControl>()) {}

	/**
	 * Defines the compare operator (compares using the ID field)
	 */
	bool operator==(const FEnvironment& Other) const;
};

/**
 * Type Hash functions for unreal 
 */
uint32 GetTypeHash(const FPreset& Preset);
uint32 GetTypeHash(const FControl& Control);
uint32 GetTypeHash(const FEnvironment& Environment);