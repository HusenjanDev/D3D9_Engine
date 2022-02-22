#pragma once
#include <Windows.h>
#include <windowsx.h>
#include <d3dx9.h>
#include <d3d9.h>

#define CUSTOMFVF (D3DFVF_XYZ | D3DFVF_DIFFUSE)

class D3D9
{
public:
	D3D9(int width, int height, HWND hWnd);
	
	/*
	 * D3D9 functions
	 */
	void InitD3D();
	void RenderFrame();
	void InitGraphics();
	void CleanD3D();

private:

	/*
	 * Window utilities
	 */
	FLOAT SCREEN_WIDTH = 0;
	FLOAT SCREEN_HEIGHT = 0;

	HWND hWnd;

	/*
	 * D3D9 utilities
	 */
	LPDIRECT3D9 d3d;
	LPDIRECT3DDEVICE9 d3ddevice;
	LPDIRECT3DVERTEXBUFFER9 v_buffer;
	LPDIRECT3DINDEXBUFFER9 i_buffer;

	/*
	 * D3D9 Vertex
	 */
	struct CUSTOMVERTEX { 
		FLOAT X, Y, Z; 
		DWORD COLOR;
	};

};