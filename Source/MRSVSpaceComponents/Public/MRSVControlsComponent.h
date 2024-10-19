#pragma once

#include "MRSVControlsComponent.generated.h"

UENUM(BlueprintType, DisplayName="Available Controls")
enum EAvailableControls : uint8
{
	CONTROL1
};

/**
 * A delegate with no params. Used for OnPlayed and OnStopped events.
 */
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FMRSVEventDelegate);

/**
 * A delegate with a string value as a parameter. This is the type for the
 * ValueChanged callback delegate of a control.
 */
DECLARE_DYNAMIC_DELEGATE_OneParam(FMRSVControlValueCallback, FString, ControlValue);

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
	 * Event received when environment play is started (Not implemented yet)
	 */
	UPROPERTY(BlueprintAssignable, Category = "MRSV*Space", meta=(ToolTip="Is triggered, when the current environment is started"))
	FMRSVEventDelegate OnEnvironmentPlayed;

	/**
	 * Received when environment play is stopped (Not implemented yet)
	 */
	UPROPERTY(BlueprintAssignable, Category = "MRSV*Space", meta=(ToolTip="Is triggered, when the current environment is stopped"))
	FMRSVEventDelegate OnEnvironmentStopped;

	/**
	 * Received when environment play is stopped (Not implemented yet)
	 *
	 * @param ControlName The name of the control to bind the delegate to
	 * @param CallbackDelegate The delegate object to bind to this control
	 */
	UFUNCTION(BlueprintCallable, Category = "MRSV*Space", meta=(ToolTip="Binds a callback to a certain control. The callback is executed, when the control value changes. To use, the control must configured as 'Blueprint Implementation'"))
	void BindControlValueChanged(	const FString& ControlName,
									const FMRSVControlValueCallback& CallbackDelegate);

	/**
	 * Executes the delegate callback of the given control with the given value.
	 *
	 * @param ControlName The name of the control to execute the callback for
	 * @param ControlValue The new value to pass to the callback delegate
	 */
	static void ExecuteControlValueChanged(const FString& ControlName, const FString& ControlValue);

	static void SetAvailableControls(TArray<FName> NameList);
	
	static void AddAvailableControl(FName Name);

	static void RemoveAvailableControl(FName Name);
	
private:
	
	/**
	 * The map storing the registered callbacks with the corresponding control name. 
	 */
	static TMap<FString, FMRSVControlValueCallback> ControlCallbackMap;
};
