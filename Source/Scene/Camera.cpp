#include "Camera.h"
extern IRHI* g_pRHI;
Camera::Camera()
{
	MVPData.World = Matrix44::GetIdentity();
	MVPData.View = GetCameraInverseMatrix();
	MVPData.Proj = GetPerspectiveMatrix();
	MVPData.WorldForNormal = Matrix44::GetIdentity();
	MVPData.CamPos = Location;
	g_pRHI->CreateConstBuffer(&MVPData, sizeof(MVPData), &m_pMVPConstBuffer);
}
