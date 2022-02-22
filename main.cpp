#include <Windows.h>
#include <windowsx.h>
#include <d3dx9.h>
#include <d3d9.h>

#pragma comment (lib, "d3d9.lib")
#pragma comment (lib, "d3dx9.lib")

#include "D3D9_Engine.h"

/*
 * SCREEN Utilities
 */
#define SCREEN_WIDTH 1280
#define SCREEN_HEIGHT 720

/*
 * WindowProc
 */
LRESULT CALLBACK WindowProc(HWND hWnd,
	UINT message,
	WPARAM wParam,
	LPARAM lParam);

/*
 * Main Initialization
 */
int WINAPI WinMain(HINSTANCE hInstance,
	HINSTANCE hPrevInstance,
	LPSTR lpCmdLine,
	int nCmdShow)
{
	HWND hWnd;
	WNDCLASSEX wc;

	ZeroMemory(&wc, sizeof(WNDCLASSEX));

	wc.cbSize = sizeof(WNDCLASSEX);
	wc.style = CS_HREDRAW | CS_VREDRAW;
	wc.lpfnWndProc = WindowProc;
	wc.hInstance = hInstance;
	wc.hCursor = LoadCursor(NULL, IDC_ARROW);
	// wc.hbrBackground = (HBRUSH)COLOR_WINDOW;
	wc.lpszClassName = L"D3D9";

	RegisterClassEx(&wc);

	hWnd = CreateWindowEx(
		NULL,
		L"D3D9",
		L"D3D9",
		WS_OVERLAPPEDWINDOW,
		330,
		200,
		SCREEN_WIDTH,
		SCREEN_HEIGHT,
		NULL,
		NULL,
		hInstance,
		NULL
	);

	ShowWindow(hWnd, nCmdShow);

	// Initializing D3D
	D3D9 D3DWindow(1280, 720, hWnd);

	D3DWindow.InitD3D();

	MSG msg;

	while (TRUE)
	{
		while (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE))
		{
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}

		if (msg.message == WM_QUIT)
		{
			break;
		}

		// Rendering D3D Frame
		D3DWindow.RenderFrame();
	}

	// Cleaning up D3D
	D3DWindow.CleanD3D();

	return msg.wParam;
}

LRESULT CALLBACK WindowProc(HWND hWnd,
	UINT message,
	WPARAM wParam,
	LPARAM lParam)
{
	switch (message)
	{
	case WM_DESTROY:
		PostQuitMessage(0);
		return 0;
		break;
	}

	return DefWindowProc(hWnd, message, wParam, lParam);
}