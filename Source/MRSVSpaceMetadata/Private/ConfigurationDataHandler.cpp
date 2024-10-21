#include "ConfigurationDataHandler.h"

#include "JsonObjectConverter.h"
#include "ModuleDescriptor.h"

void ConfigurationDataHandler::Load()
{
	//Only load if metadata file already exists
	if(IFileManager::Get().FileExists(*StoragePath))
	{
		//Create JsonObject and JsonString
		TSharedPtr<FJsonObject> JsonObject;
		FString JsonString;
		//Load File to Json String
		FFileHelper::LoadFileToString(JsonString, *StoragePath);
		if (
			//Deserialize Json to JsonObject
			FJsonSerializer::Deserialize(TJsonReaderFactory<>::Create(JsonString), JsonObject)
			//Check JsonObject is valid after deserialization
			&& JsonObject.IsValid()
			//Load JsonObject to Environment UStruct
			&& FJsonObjectConverter::JsonObjectToUStruct(JsonObject.ToSharedRef(), EnvironmentData.Get(), 0, 0))
		{
			//After successful deserialization manually load option fields as Ustruct doesn't support recursion objects
			auto ControlListLoad = JsonObject->GetArrayField(FString("controls"));
			for(int i = 0; i < ControlListLoad.Num(); i++)
			{
				//Loop options
				for(auto OptionLoad : ControlListLoad[i]->AsObject()->GetObjectField(FString("details"))->GetArrayField(FString("options")))
				{
					//Create and load option
					FControl* Control = new FControl();
					if(FJsonObjectConverter::JsonObjectToUStruct(OptionLoad->AsObject().ToSharedRef(), Control, 0, 0))
					{
						//On succesfull load, add option to option array
						EnvironmentData->Controls[i].Details.Controls.Add(*Control);
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

void ConfigurationDataHandler::Save() const
{
	//Create the JsonObject
	TSharedPtr<FJsonObject> JsonObject = MakeShareable(new FJsonObject());
	//Create the Json Output String
	FString JsonString;
	// Convert the struct to JSON
	if (FJsonObjectConverter::UStructToJsonObject(FEnvironment::StaticStruct(), EnvironmentData.Get(), JsonObject.ToSharedRef(), 0, 0))
	{
		// Loop all controls available
		for(int i = 0; i < EnvironmentData->Controls.Num(); i++)
		{
			//Create Json Array
			TArray<TSharedPtr<FJsonValue>> ControlArr;
			//Loop all options of control
			for (auto Control : EnvironmentData->Controls[i].Details.Controls)
			{
				//Create the Json Option Object
				TSharedPtr<FJsonObject> OptionJsonObj = MakeShareable(new FJsonObject());
				//Load Struct to Json Object
				FJsonObjectConverter::UStructToJsonObject(FControl::StaticStruct(), &Control, OptionJsonObj.ToSharedRef(), 0, 0);
				//Add Option Json Object to Array
				ControlArr.Add(MakeShareable(new FJsonValueObject(OptionJsonObj)));
			}
			JsonObject->GetArrayField(FString("controls"))[i]->AsObject()->GetObjectField(FString("details"))->SetArrayField("controls", ControlArr);
		}
		// Convert the JSON object to a string
		FJsonSerializer::Serialize(JsonObject.ToSharedRef(), TJsonWriterFactory<>::Create(&JsonString));
		// Save the Json string to the metadata file
		FFileHelper::SaveStringToFile(JsonString, *StoragePath);
	}
}

TSharedPtr<FEnvironment> ConfigurationDataHandler::GetEnvironment() const
{
	return EnvironmentData;
}

ConfigurationDataHandler::ConfigurationDataHandler(FString InPath)
{
	// Store path
	StoragePath = InPath;
	// Load meta on initialization
	Load();
}