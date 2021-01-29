#include "WinApp.h"
#include "Render/ForwardRenderer.h"
#include "RHI/DX11/RHIDX11.h"
#include "Scene/World.h"
#include "Scene/InputManager.h"
#include "Scene/Camera.h"

extern Camera* DefaultCamera = nullptr;
extern IRHI* g_pRHI = nullptr;

LRESULT CALLBACK WinProc(HWND HWnd, UINT UMsg, WPARAM WParam, LPARAM LParam);
bool bCanExit = false;

void WinApp::Launch()
{
    Init();
	while (!bCanExit)
	{
		Tick();
	}
	Exit();
}

void WinApp::Init()
{
    //get instance
    HINSTANCE HInstance = (HINSTANCE)GetModuleHandle(nullptr);

    //create WndClass
    WNDCLASSEX WndClass;
    WndClass.cbSize = sizeof(WndClass);
    WndClass.cbClsExtra = 0;
    WndClass.cbWndExtra = 0;
    WndClass.hbrBackground = (HBRUSH)GetStockObject(WHITE_BRUSH);
    WndClass.hCursor = LoadCursor(nullptr, IDC_ARROW);
    WndClass.hIcon = LoadIcon(nullptr, IDI_APPLICATION);
    WndClass.hIconSm = nullptr;
    WndClass.hInstance = HInstance;
    WndClass.lpfnWndProc = WinProc;
    WndClass.lpszClassName = TEXT("MainWClass");
    WndClass.lpszMenuName = nullptr;
    WndClass.style = CS_VREDRAW | CS_HREDRAW;

    //register WndClass
    RegisterClassEx(&WndClass);

    //create a window
    HWnd = CreateWindowEx(
        0,
        TEXT("MainWClass"),
        TEXT("GameEngine"),
        WS_OVERLAPPEDWINDOW,
        0,
        0,
        800,
        600,
        nullptr,
        nullptr,
        HInstance,
        nullptr
    );

    //show window
    ShowWindow(HWnd, SW_NORMAL);

    //update window
    UpdateWindow(HWnd);

    //render
	g_pRHI = new FRHIDX11(HWnd);

	//init world
	FArray<FString> maps{ "Content/Test_Map.json" };
	GWorld->Build(maps);

	//create default camera
	DefaultCamera = new Camera();

	Renderer = new ForwardRender();
	Renderer->PreRender();
}

void WinApp::Tick()
{
	//game logic
	DefaultCamera->Update();
	GWorld->Tick();
	//render
	Renderer->Render();

	//dispatch message
	MSG Msg;
	while (PeekMessage(&Msg, nullptr, 0, 0, PM_NOREMOVE))
	{
		if (GetMessage(&Msg, nullptr, 0, 0))
		{
			TranslateMessage(&Msg);
			DispatchMessage(&Msg);
		}
	}
}

void WinApp::Exit()
{
	delete Renderer;
	delete RHI;
}


LRESULT CALLBACK WinProc(HWND HWnd, UINT UMsg, WPARAM WParam, LPARAM LParam)
{
	auto InputMng = InputManager::GetInstance();
	switch (UMsg)
	{
	case WM_KEYUP:
	{
		if (WParam == VK_ESCAPE)
		{
			bCanExit = true;
		}
		else if (WParam == 'O')
		{

		}
		else if (WParam == 'P')
		{
		}
		else if (WParam == 'W')
		{
			InputMng->FrontDelta = 0;
		}
		else if (WParam == 'S')
		{
			InputMng->FrontDelta = 0;
		}
		else if (WParam == 'A')
		{
			InputMng->RightDelta = 0;
		}
		else if (WParam == 'D')
		{
			InputMng->RightDelta = 0;
		}
		else if (WParam == 'Q')
		{
			InputMng->UpDelta = 0;
		}
		else if (WParam == 'E')
		{
			InputMng->UpDelta = 0;
		}
		break;
	}
	case WM_KEYDOWN:
	{
		if (WParam == 'W')
		{
			InputMng->FrontDelta = 0.1;
		}
		else if (WParam == 'S')
		{
			InputMng->FrontDelta = -0.1;
		}
		else if (WParam == 'A')
		{
			InputMng->RightDelta = -0.1;
		}
		else if (WParam == 'D')
		{
			InputMng->RightDelta = 0.1;
		}
		else if (WParam == 'Q')
		{
			InputMng->UpDelta = 0.1;
		}
		else if (WParam == 'E')
		{
			InputMng->UpDelta = -0.1;
		}
		break;
	}
	case WM_MOUSEMOVE:
	{
		if (WParam == MK_LBUTTON)
		{
			float XPos = LOWORD(LParam);
			float YPos = HIWORD(LParam);
			if (InputMng->bStartTouchMove)
			{
				InputMng->DeltaMove = Vector2D(XPos - InputMng->LastMousePos.X, YPos - InputMng->LastMousePos.Y);
			}
			else
			{
				InputMng->bStartTouchMove = true;
			}
			InputMng->LastMousePos = Vector2D(XPos, YPos);
		}
		else
		{
			InputMng->bStartTouchMove = false;
			InputMng->LastMousePos = Vector2D();
			InputMng->DeltaMove = Vector2D();
		}
		break;
	}
	}
	return DefWindowProc(HWnd, UMsg, WParam, LParam);
}