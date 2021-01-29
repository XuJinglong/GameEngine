#include "Map.h"
#include "Scene/PointLight.h"
#include "Scene/DirectionLight.h"
#include "World.h"
#include "Actor.h"
#include "Third/nlohmann_json/json.hpp"
#include "FileSystem/FileUtils.h"

using json = nlohmann::json;

#define JSON2VECTOR3(array) Vector3D(array[0], array[1], array[2])

Map::Map(FString mapFile)
{
	m_MatFile = mapFile;
}

void Map::Build(World* world)
{
	std::fstream mapFile(m_MatFile);
	if (!mapFile)
	{
		return;
	}

	json mapConfig = json::parse(mapFile);
	if (mapConfig.is_discarded())
	{
		return;
	}

	//create actors
	auto jsonActors = mapConfig["actors"];
	for (json::iterator it = jsonActors.begin(); it != jsonActors.end(); ++it)
	{
		FString name = it.key();
		auto actorJson = it.value();
		FString meshName = "";
		if (actorJson["mesh"].is_string())
		{
			meshName = actorJson["mesh"];
		}

		FString materialName = "";
		if (actorJson["material"].is_string())
		{
			materialName = actorJson["material"];
		}
		
		Actor* pActor = new Actor(name, meshName, materialName);
		pActor->SetLocation(JSON2VECTOR3(actorJson["position"]));
		world->m_Actors.push_back(pActor);

		if (actorJson["rotation"].is_array()) 
		{
			pActor->SetRotation(JSON2VECTOR3(actorJson["rotation"]));
		}
	}

	//create lights
	auto jsonDLight = mapConfig["direction_light"];
	auto dLightDir = JSON2VECTOR3(jsonDLight["direction"]);
	auto dLightColor = JSON2VECTOR3(jsonDLight["color"]);
	DirectionLight* pDLight = new DirectionLight(dLightDir, dLightColor);
	world->m_pDirectionLight = pDLight;

	auto jsonPLights = mapConfig["point_lights"];
	for (json::iterator it = jsonPLights.begin(); it != jsonPLights.end(); ++it)
	{
		FString name = it.key();
		auto lightJson = it.value();
		auto position = JSON2VECTOR3(lightJson["position"]);
		auto color = JSON2VECTOR3(lightJson["color"]);
		PointLight* pPlight = new PointLight(position, color);
		world->m_PointLights.push_back(pPlight);
	}
}
