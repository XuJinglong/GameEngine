#include "ForwardRenderer.h"
#include "Scene/World.h"
#include "Render/MeshDrawCommand.h"
#include "Scene/Actor.h"
#include "RHI/IRHI.h"
#include "Scene/Camera.h"
#include "Scene/SkySphere.h"

extern Camera* DefaultCamera;
extern IRHI* g_pRHI;

void ForwardRender::PreRender()
{
	//设置灯光constbuffer
	g_pRHI->PSSetConstantBuffers(1, 1, GWorld->m_pWorldConstBuffer);
	g_pRHI->VSSetConstantBuffers(1, 1, GWorld->m_pWorldConstBuffer);

	//设置相机constbuffer
	g_pRHI->PSSetConstantBuffers(0, 1, DefaultCamera->m_pMVPConstBuffer);
	g_pRHI->VSSetConstantBuffers(0, 1, DefaultCamera->m_pMVPConstBuffer);
}

void ForwardRender::Render()
{
	//clear view
	g_pRHI->ClearView();

	//update world constbuffer
	g_pRHI->UpdateConstBuffer(&GWorld->m_WorldBufferInfo, sizeof(GWorld->m_WorldBufferInfo), GWorld->m_pWorldConstBuffer);

	g_pRHI->SetDepthStencilState(GWorld->m_pDepthStencileState);
	FArray<MeshDrawCommand*> commandList;
	for (auto actor : GWorld->m_Actors) 
	{
		commandList.push_back(actor->GetMeshDrawCommand());
	}
	for (auto cmd : commandList) 
	{
		cmd->Execute();
	}

	GWorld->m_pSky->Draw();
	
	g_pRHI->FlushView();
}
