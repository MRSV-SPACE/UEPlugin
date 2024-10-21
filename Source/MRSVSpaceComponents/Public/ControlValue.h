#pragma once

#include "ControlValue.generated.h"

/**
 * Blueprint Wrapper for the FJsonValue
 */
UCLASS(BlueprintType, Blueprintable)
class MRSVCOMPONENTS_API UControlValue : public UObject
{
	GENERATED_UCLASS_BODY()

public:
	UControlValue(TSharedPtr<FJsonValue> InValue);
	
	/** Returns true if this value is a 'null' */
	UFUNCTION(BlueprintCallable, Category = "MRSV*Space")
	bool IsNull() const;

	/** Returns this value as a double, throwing an error if this is not an Json Number
	 * Attn.!! float used instead of double to make the function blueprintable! */
	UFUNCTION(BlueprintCallable, Category = "MRSV*Space")
	float AsNumber() const;

	/** Returns this value as a string, throwing an error if this is not an Json String */
	UFUNCTION(BlueprintCallable, Category = "MRSV*Space")
	FString AsString() const;

	/** Returns this value as a boolean, throwing an error if this is not an Json Bool */
	UFUNCTION(BlueprintCallable, Category = "MRSV*Space")
	bool AsBool() const;

private:
	/** Internal JSON data */
	TSharedPtr<FJsonValue> JsonVal;
};