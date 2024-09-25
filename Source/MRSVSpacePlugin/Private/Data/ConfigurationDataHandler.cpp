#include "Data/ConfigurationDataHandler.h"

#include "JsonObjectConverter.h"

void ConfigurationDataHandler::Load()
{
	//Only load if metadata file already exists
	if(IFileManager::Get().FileExists(*GetMetaFilePath()))
	{
		//Create JsonObject and JsonString
		TSharedPtr<FJsonObject> JsonObject;
		FString JsonString;
		//Load File to Json String
		FFileHelper::LoadFileToString(JsonString, *GetMetaFilePath());
		if (
			//Deserialize Json to JsonObject
			FJsonSerializer::Deserialize(TJsonReaderFactory<>::Create(JsonString), JsonObject)
			//Check JsonObject is valid after deserialization
			&& JsonObject.IsValid()
			//Load JsonObject to Environment UStruct
			&& FJsonObjectConverter::JsonObjectToUStruct(JsonObject.ToSharedRef(), EnvironmentData, 0, 0))
		{
			//After successful deserialization manually load option fields as Ustruct doesn't support recursion objects
			auto ControlListLoad = JsonObject->GetArrayField(FString("controls"));
			for(int i = 0; i < ControlListLoad.Num(); i++)
			{
				//Get corresponding control in metadata
				FControl Control = EnvironmentData->Controls[i];
				//Loop options
				for(auto OptionLoad : ControlListLoad[i]->AsObject()->GetObjectField(FString("details"))->GetArrayField(FString("options")))
				{
					//Create and load option
					FControl* Option = new FControl();
					if(FJsonObjectConverter::JsonObjectToUStruct(OptionLoad->AsObject().ToSharedRef(), Option, 0, 0))
					{
						//On succesfull load, add option to option array
						Control.Details.Options.Add(Option);
					} else
					{
						UE_LOG(LogTemp, Error, TEXT("Loading option to as struct failed"));
					}
				}
			}
		}
		else
		{
			UE_LOG(LogTemp, Error, TEXT("Loading metadata JSON failed"));
		}
	} 
}

void ConfigurationDataHandler::Save()
{
	//Create the JsonObject
	TSharedPtr<FJsonObject> JsonObject = MakeShareable(new FJsonObject());
	//Create the Json Output String
	FString JsonString;
	// Convert the struct to JSON
	if (FJsonObjectConverter::UStructToJsonObject(FEnvironment::StaticStruct(), EnvironmentData, JsonObject.ToSharedRef(), 0, 0))
	{
		// Loop all controls available
		for(int i = 0; i < EnvironmentData->Controls.Num(); i++)
		{
			//Create Json Array
			TArray<TSharedPtr<FJsonValue>> OptionsArr;
			//Loop all options of control
			for (auto Option : EnvironmentData->Controls[i].Details.Options)
			{
				//Create the Json Option Object
				TSharedPtr<FJsonObject> OptionJsonObj = MakeShareable(new FJsonObject());
				//Load Struct to Json Object
				FJsonObjectConverter::UStructToJsonObject(FControl::StaticStruct(), Option, OptionJsonObj.ToSharedRef(), 0, 0);
				//Add Option Json Object to Array
				OptionsArr.Add(MakeShareable(new FJsonValueObject(OptionJsonObj)));
			}
			JsonObject->GetArrayField(FString("controls"))[i]->AsObject()->GetObjectField(FString("details"))->SetArrayField("options", OptionsArr);
		}
		// Convert the JSON object to a string
		FJsonSerializer::Serialize(JsonObject.ToSharedRef(), TJsonWriterFactory<>::Create(&JsonString));
		// Save the Json string to the metadata file
		FFileHelper::SaveStringToFile(JsonString, *GetMetaFilePath());
	}
}

FEnvironment* ConfigurationDataHandler::GetEnvironment()
{
	return EnvironmentData;
}

void ConfigurationDataHandler::SetEnvironment(FEnvironment InEnvironment)
{
	EnvironmentData = &InEnvironment;
}

FString ConfigurationDataHandler::GetMetaFilePath()
{
	return FPaths::Combine(FPaths::ProjectSavedDir(), TEXT("MRSV/metadata.json"));
}
