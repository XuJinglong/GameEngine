#pragma once
#include "EngineCommon.h"
#include "InputManager.h"
#include "RHI/IRHI.h"

class Camera 
{
public:
	Camera();
	~Camera() {}

	void Update() 
	{
		auto InputMng = InputManager::GetInstance();
		Location.Z += InputMng->FrontDelta * ControlMoveSpeed;
		Location.X += InputMng->RightDelta * ControlMoveSpeed;
		Location.Y += InputMng->UpDelta * ControlMoveSpeed;

		Rotation.Pitch += InputMng->DeltaMove.Y * ControlRotSpeed;
		Rotation.Yaw += InputMng->DeltaMove.X * ControlRotSpeed;

		MVPData.View = GetCameraInverseMatrix();
		MVPData.Proj = GetPerspectiveMatrix();
	}

	Matrix44 GetPerspectiveMatrix()
	{
		Matrix44 Ret = Matrix44::GetIdentity();
		Ret[0][0] = 1 / (WidthHeightRatio * tan(TO_RADIAN(FOV / 2)));
		Ret[1][1] = 1.f / tan(TO_RADIAN(FOV / 2));
		Ret[2][2] = FarPlane / (FarPlane - NearPlane);
		Ret[2][3] = 1;
		Ret[3][2] = -FarPlane * NearPlane  / (FarPlane - NearPlane);
		Ret[3][3] = 0;
		return Ret;
	}

	Matrix44 GetCameraMatrix() 
	{
		Matrix44 Ret = Matrix44::GetIdentity();
		Ret[3][0] = Location.X;
		Ret[3][1] = Location.Y;
		Ret[3][2] = Location.Z;

		Ret =  Rotation.GetMatrix() * Ret;
		return Ret;
	}

	Matrix44 GetCameraInverseMatrix() 
	{
		Matrix44 Rot = Rotation.GetInverseMatrix();
		Matrix44 Trans = Matrix44::GetIdentity();
		Trans[3][0] = -Location.X;
		Trans[3][1] = -Location.Y;
		Trans[3][2] = -Location.Z;
		return  Trans * Rot;
	}

	Matrix44 GetSkyBoxCameraViewMatrix()
	{
		return Rotation.GetInverseMatrix();
	}

public:
	struct MVP
	{
		Matrix44 World;
		Matrix44 View;
		Matrix44 Proj;
		Matrix44 WorldForNormal;
		Vector4D CamPos;
	};

	float FOV = 90.f;//Height
	float ViewDistance = 1.f;
	float WidthHeightRatio = 1.33f;
	float NearPlane = 10.f;
	float FarPlane = 3000.f;


	Vector3D Location{0, 0, -100};
	Rotator Rotation{0, 0, 0};

	float ControlMoveSpeed = 1.f;
	float ControlRotSpeed = 0.02f;

	MVP MVPData;
	RHIResourceRef* m_pMVPConstBuffer = nullptr;
};