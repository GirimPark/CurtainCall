#pragma once

#include <d3d11.h>
#include <wrl/client.h>

using namespace Microsoft::WRL;

const int MAX_FILE_NUM = 5;

/// <summary>
/// Renderer�� �̱������� ������ �ʱ����� �̱����� CommonApp(Core)�� Ȱ���Ѵ�.
/// GameApp �ܿ����� �������� ���縦 ����� �ϰ�,
/// Engine������ �������� ���縦 �ȴ�.
/// </summary>
class D3DRenderer
{
private:
	ComPtr<ID3D11Device> m_pDevice = nullptr;
	ComPtr<ID3D11DeviceContext> m_pDeviceContext = nullptr;
	ComPtr<IDXGISwapChain> m_pSwapChain = nullptr;
	ComPtr<ID3D11RenderTargetView> m_pRenderTargetView = nullptr;
	ComPtr<ID3D11DepthStencilView> m_pDepthStencilView = nullptr;

	ComPtr<ID3D11VertexShader> m_pVertexShaders[MAX_FILE_NUM];	// ���� ���̴�
	ComPtr<ID3D11PixelShader> m_pPixelShaders[MAX_FILE_NUM];		// �ȼ� ���̴�
	ComPtr<ID3D11InputLayout> m_pInputLayout = nullptr;			// �Է� ���̾ƿ�
	ComPtr<ID3D11SamplerState> m_pSamplerLinear = nullptr;		// ���÷� ����
	ComPtr<ID3D11BlendState> m_pAlphaBlendState = nullptr;		// ���� ����

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

