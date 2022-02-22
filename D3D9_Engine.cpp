#include "D3D9_Engine.h"

D3D9::D3D9(int width, int height, HWND hWnd)
{
	this->SCREEN_WIDTH = width;
	this->SCREEN_HEIGHT = height;
	this->hWnd = hWnd;
}

void D3D9::InitD3D()
{
	/*
	 * Setting up D3D9
	 */
	d3d = Direct3DCreate9(D3D_SDK_VERSION);

	D3DPRESENT_PARAMETERS d3dpp;

	ZeroMemory(&d3dpp, sizeof(d3dpp));
	d3dpp.Windowed = TRUE;
	d3dpp.SwapEffect = D3DSWAPEFFECT_DISCARD;
	d3dpp.hDeviceWindow = hWnd;
	d3dpp.BackBufferFormat = D3DFMT_X8R8G8B8;
	d3dpp.BackBufferWidth = SCREEN_WIDTH;
	d3dpp.BackBufferHeight = SCREEN_HEIGHT;
	d3dpp.EnableAutoDepthStencil = TRUE;
	d3dpp.AutoDepthStencilFormat = D3DFMT_D16;

	d3d->CreateDevice(
		D3DADAPTER_DEFAULT,
		D3DDEVTYPE_HAL,
		hWnd,
		D3DCREATE_SOFTWARE_VERTEXPROCESSING,
		&d3dpp,
		&d3ddevice
	);

	// Calling InitGraphics function
	InitGraphics();

	/*
	 * RenderStates
	 */
	d3ddevice->SetRenderState(D3DRS_LIGHTING, FALSE);
	d3ddevice->SetRenderState(D3DRS_CULLMODE, D3DCULL_NONE);
	d3ddevice->SetRenderState(D3DRS_ZENABLE, TRUE);
}


void D3D9::InitGraphics()
{
	CUSTOMVERTEX vertices[] =
	{
		{ -3.0f, 3.0f, -3.0f, D3DCOLOR_XRGB(0, 0, 255), },
		{ 3.0f, 3.0f, -3.0f, D3DCOLOR_XRGB(0, 255, 0), },
		{ -3.0f, -3.0f, -3.0f, D3DCOLOR_XRGB(255, 0, 0), },
		{ 3.0f, -3.0f, -3.0f, D3DCOLOR_XRGB(0, 255, 255), },
		{ -3.0f, 3.0f, 3.0f, D3DCOLOR_XRGB(0, 0, 255), },
		{ 3.0f, 3.0f, 3.0f, D3DCOLOR_XRGB(255, 0, 0), },
		{ -3.0f, -3.0f, 3.0f, D3DCOLOR_XRGB(0, 255, 0), },
		{ 3.0f, -3.0f, 3.0f, D3DCOLOR_XRGB(0, 255, 255), },
	};

	d3ddevice->CreateVertexBuffer(
		8 * sizeof(CUSTOMVERTEX),
		0,
		CUSTOMFVF,
		D3DPOOL_MANAGED,
		&v_buffer,
		NULL);

	VOID* pVoid;

	v_buffer->Lock(0, 0, (void**)&pVoid, 0);
	memcpy(pVoid, vertices, sizeof(vertices));
	v_buffer->Unlock();

	short indices[] =
	{
		0, 1, 2,    // Side 1
		2, 1, 3,

		4, 0, 6,    // Side 2
		6, 0, 2,

		7, 5, 6,    // Side 3
		6, 5, 4,

		3, 1, 7,    // Side 4
		7, 1, 5,

		4, 5, 0,    // Side 5
		0, 5, 1,

		3, 7, 2,    // Side 6
		2, 7, 6,
	};

	d3ddevice->CreateIndexBuffer(
		36 * sizeof(short),
		0,
		D3DFMT_INDEX16,
		D3DPOOL_MANAGED,
		&i_buffer,
		NULL);

	i_buffer->Lock(0, 0, (void**)&pVoid, 0);
	memcpy(pVoid, indices, sizeof(indices));
	i_buffer->Unlock();
}

void D3D9::RenderFrame()
{
	d3ddevice->Clear(0, NULL, D3DCLEAR_TARGET, D3DCOLOR_XRGB(36, 31, 83), 1.0f, 0);
	d3ddevice->Clear(0, NULL, D3DCLEAR_ZBUFFER, D3DCOLOR_XRGB(0, 0, 0), 1.0f, 0);

	/*
	 * Render Begin
	 */
	d3ddevice->BeginScene();
	d3ddevice->SetFVF(CUSTOMFVF);

	/*
	 * D3D9 Camera
	 */
	D3DXMATRIX matView;

	D3DXMatrixLookAtLH(&matView,
		&D3DXVECTOR3(0.0f, 10.0f, 20.0f),
		&D3DXVECTOR3(0.0f, 0.0f, 0.0f),
		&D3DXVECTOR3(0.0f, 1.0f, 0.0f));

	d3ddevice->SetTransform(D3DTS_VIEW, &matView);

	/*
	 * D3D9 movement
	 */
	D3DXMATRIX matProjection;
	D3DXMatrixPerspectiveFovLH(&matProjection,
		D3DXToRadian(45),
		(FLOAT)SCREEN_WIDTH / (FLOAT)SCREEN_HEIGHT,
		1.0f,
		100.0f);

	d3ddevice->SetTransform(D3DTS_PROJECTION, &matProjection);

	// Speed declaration. 
	static float index = 0.0f;

	// How fast?
	index += 0.001f;

	/*
	 * Cube movement
	 */
	D3DXMATRIX matRotateY;
	D3DXMatrixRotationY(&matRotateY, index);
	d3ddevice->SetTransform(D3DTS_WORLD, &(matRotateY));
	d3ddevice->SetTransform(D3DTS_PROJECTION, &matProjection);

	/*
	 * Drawing cube
	 */
	d3ddevice->SetStreamSource(0, v_buffer, 0, sizeof(CUSTOMVERTEX));
	d3ddevice->SetIndices(i_buffer);
	d3ddevice->DrawIndexedPrimitive(D3DPT_TRIANGLELIST, 0, 0, 8, 0, 12);

	/*
	 * Ending scene
	 */
	d3ddevice->EndScene();
	d3ddevice->Present(NULL, NULL, NULL, NULL);
}

void D3D9::CleanD3D()
{
	/*
	 * Cleaning up...
	 */
	v_buffer->Release();
	i_buffer->Release();
	d3ddevice->Release();
	d3d->Release();
}