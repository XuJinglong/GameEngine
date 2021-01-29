#pragma once
#include "StaticMeshComponent.h"

class Actor
{
public:
	Actor(FString name, FString meshName = "", FString materialName = "")
	{
		m_name = name;
		Mesh = new StaticMeshComponent(meshName, materialName);
	}
	~Actor()
	{
		delete Mesh;
	}

public:
	void SetLocation(Vector3D location);
	void SetRotation(Vector3D rotation);

	class MeshDrawCommand* GetMeshDrawCommand();
public:
	StaticMeshComponent* Mesh = nullptr;
	Transform Trans;
	FString m_name = "";
public:
	Rotator m_Rotation{ 0,0,0 };
	Vector3D m_Location{ 0, 0, 0 };
	Matrix44 GetWorldMatrix()
	{
		auto Ret = Matrix44::GetIdentity();
		Ret = m_Rotation.GetMatrix() * m_Location.GetMatrix();
		return Ret;
	}
};

