#pragma once

#include <Windows.h>
#include <d3d11.h>
#include <memory>
#include <wrl/client.h>
#include "../D3DRenderer/D3DRenderer.h"

using namespace Microsoft::WRL;

const int MAX_LOADSTRING = 100;

class D3DRenderer;

/// <summary>
/// ���ӿ��� ����� �������� ó���� �ϴ� class
/// ������ ���� ������ CommonApp�� ��� �޾� �ΰ����� ó���� �Ѵ�.
///	�̱��� �������� ����� Window, DirectX �ڿ��� ���� ������ �����ϰ� �Ѵ�.
/// </summary>
class CommonApp
{
/// Window �ڿ�
public:
	static HWND m_hWnd;
	static CommonApp* m_pInstance;	// �� ������ ���ƾ��ϱ� ������ public���� ����

protected:
	HACCEL m_hAccelTable;
	MSG m_msg;
	HINSTANCE m_hInstance;
	WCHAR m_szTitle[MAX_LOADSTRING];
	WCHAR m_szWindowClass[MAX_LOADSTRING];
	WNDCLASSEXW m_wcex;
	int m_nCmdShow;

/// D3DRenderer�� ����� �ڿ�
protected:
	std::shared_ptr<D3DRenderer> m_pD3DRenderer = nullptr;
	ComPtr<ID3D11Device> m_pDevice = nullptr;
	ComPtr<ID3D11DeviceContext> m_pDeviceContext = nullptr;
	ComPtr<IDXGISwapChain> m_pSwapChain = nullptr;

	// GameApp���� D3DRenderer���� ������ ���̴� �����̸�
	const WCHAR* m_VSFiles[MAX_FILE_NUM];
	const WCHAR* m_PSFiles[MAX_FILE_NUM];

public:
	std::shared_ptr<D3DRenderer> GetRenderer() const { return m_pD3DRenderer; }
	ComPtr<ID3D11Device> GetDevice() const { return m_pDevice; }
	ComPtr<ID3D11DeviceContext> GetDevcon() const { return m_pDeviceContext; }
	

protected:
	CommonApp(HINSTANCE hInstance);
	virtual ~CommonApp() = default;

public:
	virtual bool Initialize();
	void Run();
	virtual void Update();
	virtual void Render();
	virtual void Finalize();

	virtual LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);
};

