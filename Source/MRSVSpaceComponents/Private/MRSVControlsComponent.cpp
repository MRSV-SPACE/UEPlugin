#include "MRSVControlsComponent.h"

UMRSVControlsComponent::UMRSVControlsComponent(FObjectInitializer const&)
{
	//Set enums empty
	UEnum* EnumType = StaticEnum<EAvailableControls>();
	TArray<TPair<FName, int64>> EmptyList = TArray<TPair<FName, int64>>();
	EnumType->SetEnums(EmptyList, UEnum::ECppForm::Regular);
}

TMap<FString, FMRSVControlValueCallback> UMRSVControlsComponent::ControlCallbackMap;

void UMRSVControlsComponent::BindControlValueChanged(const FString& ControlName,
                                                     const FMRSVControlValueCallback& CallbackDelegate)
{
	UE_LOG(LogTemp, Display, TEXT("Binding control %s to callback"), *ControlName);
	ControlCallbackMap.Add(ControlName, CallbackDelegate);
}

void UMRSVControlsComponent::ExecuteControlValueChanged(const FString& ControlName, const FString& ControlValue)
{
	FMRSVControlValueCallback* Callback = ControlCallbackMap.Find(ControlName);
	if (Callback != nullptr && Callback->IsBound())
	{
		Callback->Execute(ControlValue);
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("No implementation for control with name %s found"), *ControlName);
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
