#include "Vector3D.h"
#include "Matrix44.h"

Matrix44 Vector3D::GetMatrix()
{
	Matrix44 Ret = Matrix44::GetIdentity();
	Ret[3][0] = X;
	Ret[3][1] = Y;
	Ret[3][2] = Z;
	return Ret;
}