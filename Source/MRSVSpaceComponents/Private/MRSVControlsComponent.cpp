#include "MRSVControlsComponent.h"

UMRSVControlsComponent::UMRSVControlsComponent(FObjectInitializer const&)
{
	// Set enums empty
	UEnum* EnumType = StaticEnum<EAvailableControls>();
	TArray<TPair<FName, int64>> EmptyList = TArray<TPair<FName, int64>>();
	EnumType->SetEnums(EmptyList, UEnum::ECppForm::Regular);
}

// Define the static properties
TMap<FString, FMRSVControlValueCallback> UMRSVControlsComponent::ControlCallbackMap;
TArray<FMRSVEnvironmentPlayedCallback> UMRSVControlsComponent::EnvironmentPlayedCallbacks;
TArray<FMRSVEnvironmentStoppedCallback> UMRSVControlsComponent::EnvironmentStoppedCallbacks;

void UMRSVControlsComponent::BindControlValueChangedCallback(
	const FString& ControlName,
	const FMRSVControlValueCallback& CallbackDelegate)
{
	// Log binding control callback
	UE_LOG(LogTemp, Display, TEXT("Binding control %s to callback"), *ControlName);
	// Add control callback to map
	ControlCallbackMap.Add(ControlName, CallbackDelegate);
}

void UMRSVControlsComponent::BindEnvironmentPlayedCallback(const FMRSVEnvironmentPlayedCallback& CallbackDelegate)
{
	// Add delegate to played callback list
	EnvironmentPlayedCallbacks.Add(CallbackDelegate);
}

void UMRSVControlsComponent::BindEnvironmentStoppedCallback(const FMRSVEnvironmentStoppedCallback& CallbackDelegate)
{
	// Add delegate to stopped callback list
	EnvironmentStoppedCallbacks.Add(CallbackDelegate);
}

void UMRSVControlsComponent::ControlValueChanged(const FString& ControlName, const UControlValue* ControlValue)
{
	UE_LOG(LogTemp, Display, TEXT("Control %s changed"), *ControlName);
	// Find the control callback from the callback map
	FMRSVControlValueCallback* Callback = ControlCallbackMap.Find(ControlName);
	// Check ff found the callback and delegate is bound
	if (Callback != nullptr && Callback->IsBound())
	{
		// Execute callback with given value
		Callback->Execute(ControlValue);
	}
	else
	{
		// Log warn that no implementation was found for given control name
		UE_LOG(LogTemp, Warning, TEXT("No implementation for control with name %s found"), *ControlName);
	}
}

void UMRSVControlsComponent::EnvironmentPlayed()
{
	// Loop registered callbacks and execute if bound
	for(FMRSVEnvironmentPlayedCallback Callback : EnvironmentPlayedCallbacks)
	{
		if(!Callback.ExecuteIfBound())
		{
			UE_LOG(LogTemp, Warning, TEXT("No implementation for environment played callback"));
		}
	}
}

void UMRSVControlsComponent::EnvironmentStopped()
{
	// Loop registered callbacks and execute if bound
	for(FMRSVEnvironmentStoppedCallback Callback : EnvironmentStoppedCallbacks)
	{
		if(!Callback.ExecuteIfBound())
		{
			UE_LOG(LogTemp, Warning, TEXT("No implementation for environment stopped callback"));
		}
	}
}

void UMRSVControlsComponent::SetAvailableControls(TArray<FName> NameList)
{
	UEnum* EnumType = StaticEnum<EAvailableControls>();
	TArray<TPair<FName, int64>> ControlEnums = TArray<TPair<FName, int64>>();
	uint counter = 0;
	for(auto Name : NameList)
	{
		ControlEnums.Add(TPair<FName, int64>(Name, counter));
		counter++;
	}
	EnumType->SetEnums(ControlEnums, UEnum::ECppForm::Regular);
}

void UMRSVControlsComponent::AddAvailableControl(FName Name)
{
	UEnum* EnumType = StaticEnum<EAvailableControls>();
	TArray<TPair<FName, int64>> ControlEnums = TArray<TPair<FName, int64>>();
	for (int32 i = 0; i < EnumType->NumEnums(); i++)
	{
		ControlEnums.Add(TPair<FName, int64>(EnumType->GetNameStringByIndex(i), i));
	}
	ControlEnums.Add(TPair<FName, int64>(Name, EnumType->NumEnums()));
	EnumType->SetEnums(ControlEnums, UEnum::ECppForm::Regular);
}

void UMRSVControlsComponent::RemoveAvailableControl(FName Name)
{
	UEnum* EnumType = StaticEnum<EAvailableControls>();
	TArray<TPair<FName, int64>> ControlEnums = TArray<TPair<FName, int64>>();
	for (int32 i = 0; i < EnumType->NumEnums(); i++)
	{
		if(!EnumType->GetNameByIndex(i).IsEqual(Name))
		{
			ControlEnums.Add(TPair<FName, int64>(EnumType->GetNameStringByIndex(i), i));
		}
	}
	EnumType->SetEnums(ControlEnums, UEnum::ECppForm::Regular);
}
