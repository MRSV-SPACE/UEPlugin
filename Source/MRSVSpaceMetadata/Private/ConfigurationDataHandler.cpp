#include "ConfigurationDataHandler.h"
#include "JsonObjectConverter.h"
#include "MetaDataUtils.h"

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
			EnvironmentData->Controls = MetaDataUtils::ManuallyLoadControls(JsonObject);

			auto PresetListLoad = JsonObject->GetArrayField(FString("presets"));
			for(int i = 0; i < PresetListLoad.Num(); i++)
			{
				//Loop values
				for(auto ValuesLoad : PresetListLoad[i]->AsObject()->GetObjectField(FString("values"))->Values)
				{
					EnvironmentData->Presets[i].Values.Add(ValuesLoad);
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
		
		// Loop all controls available
        for(int i = 0; i < EnvironmentData->Presets.Num(); i++)
        {
        	//Create the Json Option Object
        	TSharedPtr<FJsonObject> PresetValuesMapObject = MakeShared<FJsonObject>();
        	//Loop all options of control
        	for (TMap<FString,TSharedPtr<FJsonValue>>::TIterator It(EnvironmentData->Presets[i].Values); It; ++It)
        	{
        		PresetValuesMapObject->SetField(It.Key(), It.Value());
        	}
        	JsonObject->GetArrayField(FString("presets"))[i]->AsObject()->SetObjectField(FString("values"), PresetValuesMapObject);
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
