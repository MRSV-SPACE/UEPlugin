#include "ControlValue.h"
#include "Misc/Base64.h"

UControlValue::UControlValue(TSharedPtr<FJsonValue> InValue)
{
	JsonVal = InValue;
}

UControlValue::UControlValue(const FObjectInitializer& PCIP)
	: Super(PCIP)
{ }

bool UControlValue::IsNull() const
{
	if (!JsonVal.IsValid())
	{
		return JsonVal->IsNull();
	}
	UE_LOG(LogTemp, Warning, TEXT("JsonVal is not set"));
	return true;
}

float UControlValue::AsNumber() const
{
	if (JsonVal.IsValid())
	{
		return JsonVal->AsNumber();
		
	}
	UE_LOG(LogTemp, Warning, TEXT("JsonVal is not set"));
	return 0.f;
}

FString UControlValue::AsString() const
{
	if (JsonVal.IsValid())
	{
		return JsonVal->AsString();
	}
	UE_LOG(LogTemp, Warning, TEXT("JsonVal is not set"));
	return FString();
}

bool UControlValue::AsBool() const
{
	if (JsonVal.IsValid())
	{
		return JsonVal->AsBool();
		
	}
	UE_LOG(LogTemp, Warning, TEXT("JsonVal is not set"));
	return false;
}
