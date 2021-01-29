#pragma once
#include "EngineCommon.h"
#include "Application/IApplication.h"
#include <Windows.h>

class WinApp : public IApplication
{
public:
	WinApp() {}
	~WinApp() {}

	virtual void Launch() override;
	virtual void Init() override;
	virtual void Tick() override;
	virtual void Exit() override;

	virtual IRenderer* GetRenderer() override {return Renderer;}
	virtual IRHI* GetRHI() override { return RHI; }

private:
	HWND HWnd = nullptr;
	IRenderer* Renderer = nullptr;
	IRHI* RHI = nullptr;
};

