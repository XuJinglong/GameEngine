#include "Vector4D.h"
#include "Matrix44.h"

Vector4D Vector4D::operator* (Matrix44 M)
{
	return Vector4D(
		X * M[0][0] + Y * M[1][0] + Z * M[2][0] + W * M[3][0],
		X * M[0][1] + Y * M[1][1] + Z * M[2][1] + W * M[3][1],
		X * M[0][2] + Y * M[1][2] + Z * M[2][2] + W * M[3][2],
		X * M[0][3] + Y * M[1][3] + Z * M[2][3] + W * M[3][3]
	);
}

Vector4D& Vector4D::operator=(const Vector3D& v3)
{
	X = v3.X;
	Y = v3.Y;
	Z = v3.Z;
	W = 1;
	return *this;
}
