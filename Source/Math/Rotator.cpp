#include "Rotator.h"
#include "Matrix44.h"

//for world to camera
Matrix44 Rotator::GetMatrix()
{
	//Roll:Z axis
	Matrix44 RollMat = Matrix44::GetIdentity();
	RollMat[0][0] = cos(TO_RADIAN(Roll));
	RollMat[0][1] = -sin(TO_RADIAN(Roll));
	RollMat[1][0] = sin(TO_RADIAN(Roll));
	RollMat[1][1] = cos(TO_RADIAN(Roll));
	//Yaw:Y axis
	Matrix44 YawMat = Matrix44::GetIdentity();
	YawMat[0][0] = cos(TO_RADIAN(Yaw));
	YawMat[0][2] = sin(TO_RADIAN(Yaw));
	YawMat[2][0] = -sin(TO_RADIAN(Yaw));
	YawMat[2][2] = cos(TO_RADIAN(Yaw));
	//Pitch:X axis
	Matrix44 PitchMat = Matrix44::GetIdentity();
	PitchMat[1][1] = cos(TO_RADIAN(Pitch));
	PitchMat[1][2] = -sin(TO_RADIAN(Pitch));
	PitchMat[2][1] = sin(TO_RADIAN(Pitch));
	PitchMat[2][2] = cos(TO_RADIAN(Pitch));

	return RollMat * PitchMat * YawMat;
}

//for camera to world
Matrix44 Rotator::GetInverseMatrix()
{
	//Roll:Z axis
	Matrix44 RollMat = Matrix44::GetIdentity();
	RollMat[0][0] = cos(TO_RADIAN(-Roll));
	RollMat[0][1] = -sin(TO_RADIAN(-Roll));
	RollMat[1][0] = sin(TO_RADIAN(-Roll));
	RollMat[1][1] = cos(TO_RADIAN(-Roll));
	//Yaw:Y axis
	Matrix44 YawMat = Matrix44::GetIdentity();
	YawMat[0][0] = cos(TO_RADIAN(-Yaw));
	YawMat[0][2] = sin(TO_RADIAN(-Yaw));
	YawMat[2][0] = -sin(TO_RADIAN(-Yaw));
	YawMat[2][2] = cos(TO_RADIAN(-Yaw));
	//Pitch:X axis
	Matrix44 PitchMat = Matrix44::GetIdentity();
	PitchMat[1][1] = cos(TO_RADIAN(-Pitch));
	PitchMat[1][2] = -sin(TO_RADIAN(-Pitch));
	PitchMat[2][1] = sin(TO_RADIAN(-Pitch));
	PitchMat[2][2] = cos(TO_RADIAN(-Pitch));

	return  YawMat * PitchMat * RollMat;
}