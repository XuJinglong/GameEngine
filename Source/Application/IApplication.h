#pragma once
#include "Render/IRenderer.h"
#include "RHI/IRHI.h"

class IApplication
{
public:
	virtual void Launch() = 0;
	virtual void Init() = 0;
	virtual void Tick() = 0;
	virtual void Exit() = 0;

	virtual IRHI* GetRHI() = 0;
	virtual IRenderer* GetRenderer() = 0;
};

