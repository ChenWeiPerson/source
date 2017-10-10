//--------------------------------------------------------------------------------------
// File: Tutorial01.cpp
//
// This application demonstrates creating a Direct3D 11 device
//
// Copyright (c) Microsoft Corporation. All rights reserved.
//--------------------------------------------------------------------------------------
#include <windows.h>
#include <d3d11.h>
#include <d3dx11.h>
#include "resource.h"


//--------------------------------------------------------------------------------------
// Global Variables
//--------------------------------------------------------------------------------------
HINSTANCE               g_hInst = NULL;
HWND                    g_hWnd = NULL;
D3D_DRIVER_TYPE         g_driverType = D3D_DRIVER_TYPE_NULL;
D3D_FEATURE_LEVEL       g_featureLevel = D3D_FEATURE_LEVEL_11_0;
ID3D11Device*           g_pd3dDevice = NULL;
ID3D11DeviceContext*    g_pImmediateContext = NULL;
IDXGISwapChain*         g_pSwapChain = NULL;
ID3D11RenderTargetView* g_pRenderTargetView = NULL;


//--------------------------------------------------------------------------------------
// Forward declarations
//--------------------------------------------------------------------------------------
HRESULT InitWindow( HINSTANCE hInstance, int nCmdShow );
HRESULT InitDevice();
void CleanupDevice();
LRESULT CALLBACK    WndProc( HWND, UINT, WPARAM, LPARAM );
void Render();


//--------------------------------------------------------------------------------------
// Entry point to the program. Initializes everything and goes into a message processing 
// loop. Idle time is used to render the scene.
//--------------------------------------------------------------------------------------
int WINAPI wWinMain( HINSTANCE hInstance, HINSTANCE hPrevInstance, LPWSTR lpCmdLine, int nCmdShow )
{
    UNREFERENCED_PARAMETER( hPrevInstance );
    UNREFERENCED_PARAMETER( lpCmdLine );

    if( FAILED( InitWindow( hInstance, nCmdShow ) ) )
        return 0;

    if( FAILED( InitDevice() ) )
    {
        CleanupDevice();
        return 0;
    }

    // Main message loop
    MSG msg = {0};
    while( WM_QUIT != msg.message )
    {
        if( PeekMessage( &msg, NULL, 0, 0, PM_REMOVE ) )
        {
            TranslateMessage( &msg );
            DispatchMessage( &msg );
        }
        else
        {
            Render();
        }
    }

    CleanupDevice();

    return ( int )msg.wParam;
}


//--------------------------------------------------------------------------------------
// Register class and create window
//--------------------------------------------------------------------------------------
HRESULT InitWindow( HINSTANCE hInstance, int nCmdShow )
{
    // Register class
    WNDCLASSEX wcex;
    wcex.cbSize = sizeof( WNDCLASSEX );
    wcex.style = CS_HREDRAW | CS_VREDRAW;
    wcex.lpfnWndProc = WndProc;
    wcex.cbClsExtra = 0;
    wcex.cbWndExtra = 0;
    wcex.hInstance = hInstance;
    wcex.hIcon = LoadIcon( hInstance, ( LPCTSTR )IDI_TUTORIAL1 );
    wcex.hCursor = LoadCursor( NULL, IDC_ARROW );
    wcex.hbrBackground = ( HBRUSH )( COLOR_WINDOW + 1 );
    wcex.lpszMenuName = NULL;
    wcex.lpszClassName = L"TutorialWindowClass";
    wcex.hIconSm = LoadIcon( wcex.hInstance, ( LPCTSTR )IDI_TUTORIAL1 );
    if( !RegisterClassEx( &wcex ) )
        return E_FAIL;

    // Create window
    g_hInst = hInstance;
    RECT rc = { 0, 0, 640, 480 };
    AdjustWindowRect( &rc, WS_OVERLAPPEDWINDOW, FALSE );
    g_hWnd = CreateWindow( L"TutorialWindowClass", L"Direct3D 11 Tutorial 1: Direct3D 11 Basics", WS_OVERLAPPEDWINDOW,
                           CW_USEDEFAULT, CW_USEDEFAULT, rc.right - rc.left, rc.bottom - rc.top, NULL, NULL, hInstance,
                           NULL );
    if( !g_hWnd )
        return E_FAIL;

    ShowWindow( g_hWnd, nCmdShow );

    return S_OK;
}


//--------------------------------------------------------------------------------------
// Called every time the application receives a message
//--------------------------------------------------------------------------------------
LRESULT CALLBACK WndProc( HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam )
{
    PAINTSTRUCT ps;
    HDC hdc;

    switch( message )
    {
        case WM_PAINT:
            hdc = BeginPaint( hWnd, &ps );
            EndPaint( hWnd, &ps );
            break;

        case WM_DESTROY:
            PostQuitMessage( 0 );
            break;

        default:
            return DefWindowProc( hWnd, message, wParam, lParam );
    }

    return 0;
}


//--------------------------------------------------------------------------------------
// Create Direct3D device and swap chain
//--------------------------------------------------------------------------------------
HRESULT InitDevice()
{
    HRESULT hr = S_OK;

    RECT rc;
    GetClientRect( g_hWnd, &rc );
    UINT width = rc.right - rc.left;
    UINT height = rc.bottom - rc.top;

    UINT createDeviceFlags = 0;
#ifdef _DEBUG
    createDeviceFlags |= D3D11_CREATE_DEVICE_DEBUG;
#endif

    D3D_DRIVER_TYPE driverTypes[] =
    {
        D3D_DRIVER_TYPE_HARDWARE,//Ӳ������
        D3D_DRIVER_TYPE_WARP,//WARP�����������һ�������ܵ������դ����������դ֧��9_1��10.1�Ĺ��ܼ���
        D3D_DRIVER_TYPE_REFERENCE,//�ο�����������ͨ�������֧�����е�Direct3D���ܡ�
    };
    UINT numDriverTypes = ARRAYSIZE( driverTypes );

    D3D_FEATURE_LEVEL featureLevels[] =
    {
        D3D_FEATURE_LEVEL_11_0,//D3D11Ӳ��������֧��SM5��tessellation��hull shader��domain shader��compute shader 5��BC6H/BC7������10_1�Ĺ���
        D3D_FEATURE_LEVEL_10_1,//D3D10.1Ӳ��������֧��SM4.1��cubemap array����չ��MSAA������10_0�Ĺ��ܣ���ѡ֧��compute shader 4.1
        D3D_FEATURE_LEVEL_10_0,//D3D10 Ӳ��������֧��SM4��geometry shader��stream output��alpha to coverage��MSAA����˫��stencil��texuture array��BC4/5������9_3�Ĺ��ܣ���ѡ֧��compute shader 4.0
    };
	UINT numFeatureLevels = ARRAYSIZE( featureLevels );


	//typedef struct DXGI_MODE_DESC {
	//	UINT                     Width;
	//	UINT                     Height;
	//	DXGI_RATIONAL            RefreshRate;//ˢ����
	//	DXGI_FORMAT              Format;//���ظ�ʽ
	//	DXGI_MODE_SCANLINE_ORDER ScanlineOrdering;//ɨ������
	//	DXGI_MODE_SCALING        Scaling;//��������
	//} DXGI_MODE_DESC, *LPDXGI_MODE_DESC;

	 //typedef struct DXGI_SAMPLE_DESC
	// {
	//     UINT Count;//ÿ���ض��ز�������
	//     UINT Quality;//ͼ�������ȼ����ȼ�Խ�ߣ�����Խ�ͣ���ѡ��Χ��0��ID3D10Device::CheckMultisampleQualityLevels
	// }DXGI_SAMPLE_DESC;

    DXGI_SWAP_CHAIN_DESC sd;
    ZeroMemory( &sd, sizeof( sd ) );
	//BufferDesc ��̨�Դ�ģʽ
    sd.BufferCount = 1;//�󻺳�����
    sd.BufferDesc.Width = width;
    sd.BufferDesc.Height = height;
    sd.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
    sd.BufferDesc.RefreshRate.Numerator = 60;
    sd.BufferDesc.RefreshRate.Denominator = 1;
    sd.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;//����ʹ���ߣ�ָ��ΪDXGI_USAGE_RENDER_TARGET_OUTPUT
    sd.OutputWindow = g_hWnd;//������ھ��
    sd.SampleDesc.Count = 1;//�������������ز���DXGI_SAMPLE_DESC���͵Ľṹ��
    sd.SampleDesc.Quality = 0;
    sd.Windowed = TRUE;//�Ƿ�Ϊ����ģʽ
	//swapeffect:����Ч����ָ��Ϊ DXGI_SWAP_EFFECT_DISCARD


    for( UINT driverTypeIndex = 0; driverTypeIndex < numDriverTypes; driverTypeIndex++ )
    {
        g_driverType = driverTypes[driverTypeIndex];
        hr = D3D11CreateDeviceAndSwapChain( 
			NULL,//����������һ���Կ���NULLΪĬ��
			g_driverType, //��������  D3D_DRIVER_TYPE_HARDWARE����ʱ��һ����������NULL
			NULL, //���ģ��
			createDeviceFlags, //Ϊ��ѡ������һ��ΪNULL��������ΪD3D11_CREATE_DEVICE_DEBUG��D3D11_CREATE_DEVICE_SINGLETHREADED��������һ��ǰ����Ҫ���ڵ���ʱ�ռ���Ϣ��������ȷ������ֻ�ڵ��߳�������ʱ����Ϊ��������������ܣ�
			featureLevels, //�����ȼ���ָ��
			numFeatureLevels,//�����ȼ���ָ���Ԫ������
			D3D11_SDK_VERSION,//�汾��
			&sd, //��������
			&g_pSwapChain, //����һ��IDXGISwapChain�����ָ���ַ�������������������Ⱦ
			&g_pd3dDevice, //�豸ָ��ĵ�ַ��ע���豸��ָ������
			&g_featureLevel, //�����صȼ��������˵�һ��֧�ֵ�Ԫ��
			&g_pImmediateContext //������
		);
        if( SUCCEEDED( hr ) )
            break;
    }
    if( FAILED( hr ) )
        return hr;

    // Create a render target view
    ID3D11Texture2D* pBackBuffer = NULL;
    hr = g_pSwapChain->GetBuffer( 0, __uuidof( ID3D11Texture2D ), ( LPVOID* )&pBackBuffer );//��ȡ�󻺳����
    if( FAILED( hr ) )
        return hr;

    hr = g_pd3dDevice->CreateRenderTargetView( pBackBuffer, NULL, &g_pRenderTargetView );//������Ⱦ��ͼ
    pBackBuffer->Release();
    if( FAILED( hr ) )
        return hr;

    g_pImmediateContext->OMSetRenderTargets( 1, &g_pRenderTargetView, NULL );

    // Setup the viewport
    D3D11_VIEWPORT vp;
    vp.Width = (FLOAT)width;
    vp.Height = (FLOAT)height;
    vp.MinDepth = 0.0f;
    vp.MaxDepth = 1.0f;
    vp.TopLeftX = 0;
    vp.TopLeftY = 0;
    g_pImmediateContext->RSSetViewports( 1, &vp );

    return S_OK;
}


//--------------------------------------------------------------------------------------
// Render the frame
//--------------------------------------------------------------------------------------
void Render()
{
    // Just clear the backbuffer
    float ClearColor[4] = { 0.0f, 0.125f, 0.3f, 1.0f }; //red,green,blue,alpha
    g_pImmediateContext->ClearRenderTargetView( g_pRenderTargetView, ClearColor );
    g_pSwapChain->Present( 0, 0 );
}


//--------------------------------------------------------------------------------------
// Clean up the objects we've created
//--------------------------------------------------------------------------------------
void CleanupDevice()
{
    if( g_pImmediateContext ) g_pImmediateContext->ClearState();

    if( g_pRenderTargetView ) g_pRenderTargetView->Release();
    if( g_pSwapChain ) g_pSwapChain->Release();
    if( g_pImmediateContext ) g_pImmediateContext->Release();
    if( g_pd3dDevice ) g_pd3dDevice->Release();
}
