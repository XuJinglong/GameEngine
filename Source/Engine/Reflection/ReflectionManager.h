#pragma once
#include "EngineCommon.h"
#include "ReflectionClass.h"
class ReflectionManager
{
private:
	ReflectionManager() {};

public:
	~ReflectionManager() 
	{
		for (auto& pair : m_reflectionData) 
		{
			delete pair.second;
		}
	}
public:
	ReflectionManager GetSington() 
	{
		static ReflectionManager sington;
		return sington;
	}

private:
	CMap<FString, ReflectionClass*> m_reflectionData;
};

