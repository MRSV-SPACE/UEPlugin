#pragma once

#include "ControlValue.h"

#include "MRSVControlsComponent.generated.h"

UENUM(BlueprintType, DisplayName="Available Controls")
enum EAvailableControls : uint8
{
	CONTROL1
};

/**
 * Delegate for environment played callback
 */
DECLARE_DYNAMIC_DELEGATE(FMRSVEnvironmentPlayedCallback);

/**
 * Delegate for environment stopped callback
 */
DECLARE_DYNAMIC_DELEGATE(FMRSVEnvironmentStoppedCallback);

/**
 * A delegate with a string value as a parameter. This is the type for the
 * ValueChanged callback delegate of a control.
 */
DECLARE_DYNAMIC_DELEGATE_OneParam(FMRSVControlValueCallback, const UControlValue*, ControlValue);

/**
 * Actor Component for Artist to implement events or functions to interact with MRSV*Space.
 * 
 * @author Florian Gubler
 */
UCLASS(ClassGroup = ("MRSV*Space"), meta = (BlueprintSpawnableComponent))
class MRSVSPACECOMPONENTS_API UMRSVControlsComponent : public UActorComponent
{
	GENERATED_UCLASS_BODY()
public:

	/**
	 * Binds a callback delegate to be triggered when a control value changes
	 *
	 * @param ControlName The name of the control to bind the delegate to
	 * @param CallbackDelegate The delegate object to bind to this control
	 */
	UFUNCTION(BlueprintCallable, Category = "MRSV*Space", meta=(ToolTip="Binds a callback to a certain control. The callback is executed, when the control value changes. To use, the control must configured as 'Blueprint Implementation'"))
	void BindControlValueChangedCallback(	const FString& ControlName,
									const FMRSVControlValueCallback& CallbackDelegate);

	/**
	 * Binds a callback delegate to be triggered when the environment is started
	 * 
	 * @param CallbackDelegate The delegate object to bind to this control
	 */
	UFUNCTION(BlueprintCallable, Category = "MRSV*Space", meta=(ToolTip="Binds a callback delegate to be triggered when the environment is played"))
	void BindEnvironmentPlayedCallback(const FMRSVEnvironmentPlayedCallback& CallbackDelegate);

	/**
	 * Binds a callback delegate to be triggered when the environment is stopped
	 *
	 * @param CallbackDelegate The delegate object to bind to this control
	 */
	UFUNCTION(BlueprintCallable, Category = "MRSV*Space", meta=(ToolTip="Binds a callback delegate to be triggered when the environment is stopped"))
	void BindEnvironmentStoppedCallback(const FMRSVEnvironmentStoppedCallback& CallbackDelegate);

	/**
	 * Executes the delegate callback of the given control with the given value.
	 *
	 * @param ControlName The name of the control to execute the callback for
	 * @param ControlValue The new value to pass to the callback delegate
	 */
	static void ControlValueChanged(const FString& ControlName, const UControlValue* ControlValue);

	static void EnvironmentPlayed();

	static void EnvironmentStopped();

	static void SetAvailableControls(TArray<FName> NameList);
	
	static void AddAvailableControl(FName Name);

	static void RemoveAvailableControl(FName Name);
	
private:
	
	/**
	 * The map storing the registered callbacks with the corresponding control name. 
	 */
	static TMap<FString, FMRSVControlValueCallback> ControlCallbackMap;

	/**
	 * The list of callbacks to execute when the environment is played
	 */
	static TArray<FMRSVEnvironmentPlayedCallback> EnvironmentPlayedCallbacks;

	/**
	 * The list of callbacks to execute when the environment is played
	 */
	static TArray<FMRSVEnvironmentStoppedCallback> EnvironmentStoppedCallbacks;
};
