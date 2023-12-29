#pragma once

#include <d3d11.h>
#include <wrl/client.h>

using namespace Microsoft::WRL;

const int MAX_FILE_NUM = 5;

/// <summary>
/// Renderer를 싱글톤으로 만들지 않기위해 싱글톤인 CommonApp(Core)를 활용한다.
/// GameApp 단에서는 렌더러의 존재를 몰라야 하고,
/// Engine까지만 렌더러의 존재를 안다.
/// </summary>
class D3DRenderer
{
private:
	ComPtr<ID3D11Device> m_pDevice = nullptr;
	ComPtr<ID3D11DeviceContext> m_pDeviceContext = nullptr;
	ComPtr<IDXGISwapChain> m_pSwapChain = nullptr;
	ComPtr<ID3D11RenderTargetView> m_pRenderTargetView = nullptr;
	ComPtr<ID3D11DepthStencilView> m_pDepthStencilView = nullptr;

	ComPtr<ID3D11VertexShader> m_pVertexShaders[MAX_FILE_NUM];	// 정점 셰이더
	ComPtr<ID3D11PixelShader> m_pPixelShaders[MAX_FILE_NUM];		// 픽셀 셰이더
	ComPtr<ID3D11InputLayout> m_pInputLayout = nullptr;			// 입력 레이아웃
	ComPtr<ID3D11SamplerState> m_pSamplerLinear = nullptr;		// 샘플러 상태
	ComPtr<ID3D11BlendState> m_pAlphaBlendState = nullptr;		// 블렌더 상태

	const HWND m_hWnd;

	const WCHAR* m_VSFiles[MAX_FILE_NUM];
	const WCHAR* m_PSFiles[MAX_FILE_NUM];

	const int m_screenWidth;
	const int m_screenHeight;

public:
	ComPtr<ID3D11Device> GetDevice() { return m_pDevice; }
	ComPtr<ID3D11DeviceContext> GetDevcon() { return m_pDeviceContext; }
	ComPtr<IDXGISwapChain> GetSwapChain() { return m_pSwapChain; }

public:
	D3DRenderer(HWND hwnd, const WCHAR** VSFiles, const WCHAR** PSFiles, int screenWidth, int screenHeight);
	~D3DRenderer();

public:
	bool Initialize();
	void Render();
};

