#pragma once

#include "MRSVControlsComponent.generated.h"

UENUM(BlueprintType, DisplayName="Available Controls")
enum EAvailableControls : uint8
{
	CONTROL1
};

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FMRSVEventDelegate);
DECLARE_DYNAMIC_DELEGATE_OneParam(FMRSVControlValueCallback, FString, ControlValue);

UCLASS(ClassGroup = ("MRSV*Space"), meta = (BlueprintSpawnableComponent))
class MRSVSPACECOMPONENTS_API UMRSVControlsComponent : public UActorComponent
{
	GENERATED_UCLASS_BODY()
public:
	/** Received when environment play is started (Not implemented yet) */
	UPROPERTY(BlueprintAssignable, Category = "MRSV*Space")
	FMRSVEventDelegate OnEnvironmentPlayed;

	/** Received when environment play is stopped (Not implemented yet) */
	UPROPERTY(BlueprintAssignable, Category = "MRSV*Space")
	FMRSVEventDelegate OnEnvironmentStopped;
	
	UFUNCTION(BlueprintCallable, Category = "MRSV*Space")
	void BindControlValueChanged(	const FString& ControlName,
									const FMRSVControlValueCallback& CallbackDelegate);

	static void ExecuteControlValueChanged(const FString& ControlName, const FString& ControlValue);

	static void SetAvailableControls(TArray<FName> NameList);
	
	static void AddAvailableControl(FName Name);

	static void RemoveAvailableControl(FName Name);
	
private:
	static TMap<FString, FMRSVControlValueCallback> ControlCallbackMap;
};
