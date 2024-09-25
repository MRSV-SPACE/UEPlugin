#pragma once

#include "MRSVControlsComponent.generated.h"

UENUM(BlueprintType, DisplayName="Available Controls")
enum EAvailableControls : uint8
{
	CONTROL1
};

/** Please add a class description */
UCLASS(ClassGroup = ("MRSV*Space"), BlueprintType, meta = (BlueprintSpawnableComponent))
class UMRSVControlsComponent : public UActorComponent
{
	GENERATED_UCLASS_BODY()
public:

	/** Please add a variable description */
	DECLARE_DYNAMIC_MULTICAST_DELEGATE(FEnvironment_Played);
	UPROPERTY(BlueprintAssignable, EditDefaultsOnly, Category="MRSV*Space")
	FEnvironment_Played Environment_Played;

	/** Please add a variable description */
	DECLARE_DYNAMIC_MULTICAST_DELEGATE(FEnvironment_Stopped);
	UPROPERTY(BlueprintAssignable, EditDefaultsOnly, Category="MRSV*Space")
	FEnvironment_Stopped Environment_Stopped;

	/** Please add a variable description */
	DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnValueChanged, EAvailableControls, Control, FString, Value);
	UPROPERTY(BlueprintAssignable, EditDefaultsOnly, Category="MRSV*Space")
	FOnValueChanged OnValueChanged;
	
	UFUNCTION(BlueprintCallable, Category="MRSV*Space")
	void MyTestFunction();

	static void SetAvailableControls(TArray<FName> NameList);
	
	static void AddAvailableControl(FName Name);

	static void RemoveAvailableControl(FName Name);
};

