#pragma once
#include "ConfigurationData.h"
#include "JsonObjectConverter.h"

class MetaDataUtils
{
public:
	static TArray<FControl> ManuallyLoadControls(TSharedPtr<FJsonObject> OuterObject)
	{
		TArray<FControl> Controls;
		//After successful deserialization manually load option fields as Ustruct doesn't support recursion objects
		auto ControlListLoad = OuterObject->GetArrayField(FString("controls"));
		for(int i = 0; i < ControlListLoad.Num(); i++)
		{
			//Create and load option
			FControl* Control = new FControl();
			if(FJsonObjectConverter::JsonObjectToUStruct(ControlListLoad[i]->AsObject().ToSharedRef(), Control, 0, 0))
			{
				for(auto OptionLoad : ControlListLoad[i]->AsObject()->GetObjectField(FString("details"))->GetArrayField(FString("controls")))
				{
					Control->Details.Controls = ManuallyLoadControls(OptionLoad->AsObject());
				}
				//On succesfull load, add option to option array
				Controls.Add(*Control);
			}
			else
			{
				UE_LOG(LogTemp, Error, TEXT("Loading option to as struct failed"));
			}
		
		}
		return Controls;
	}

	
};
