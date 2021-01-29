#pragma once
#include "EngineCommon.h"

class Map
{
public:
	Map(FString mapFile);
	~Map() {}

public:
	void Build(class World* world);
private:
	FString m_MatFile;
};

