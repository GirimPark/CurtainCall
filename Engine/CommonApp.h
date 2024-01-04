#pragma once

#include <Windows.h>
#include <d3d11.h>
#include <memory>
#include "../D3DRenderer/D3DRenderer.h"

const int MAX_LOADSTRING = 100;

class D3DRenderer;

/// <summary>
/// 게임에서 사용할 공통적인 처리를 하는 class
/// 실제로 만들 게임은 CommonApp을 상속 받아 부가적인 처리를 한다.
///	싱글톤 패턴으로 만들어 Window, DirectX 자원에 대한 접근을 용이하게 한다.
/// </summary>
class CommonApp
{
/// Window 자원
public:
	static CommonApp* m_pInstance;	// 매 프레임 돌아야하기 때문에 public으로 선언

protected:
	static HWND m_hWnd;

	HACCEL m_hAccelTable;
	MSG m_msg;
	HINSTANCE m_hInstance;
	WCHAR m_szTitle[MAX_LOADSTRING];
	WCHAR m_szWindowClass[MAX_LOADSTRING];
	WNDCLASSEXW m_wcex;
	int m_nCmdShow;

/// D3DRenderer와 연결된 자원
protected:
	std::shared_ptr<D3DRenderer> m_pD3DRenderer = nullptr;
	ComPtr<ID3D11Device> m_pDevice = nullptr;
	ComPtr<ID3D11DeviceContext> m_pDeviceContext = nullptr;
	ComPtr<IDXGISwapChain> m_pSwapChain = nullptr;

	// GameApp에서 D3DRenderer까지 전달할 셰이더 파일이름
	const WCHAR* m_VSFiles[MAX_FILE_NUM];
	const WCHAR* m_PSFiles[MAX_FILE_NUM];

public:
	static HWND GetHwnd() { return m_hWnd; }
	std::shared_ptr<D3DRenderer> GetRenderer() const { return m_pD3DRenderer; }
	ComPtr<ID3D11Device> GetDevice() const { return m_pDevice; }
	ComPtr<ID3D11DeviceContext> GetDevcon() const { return m_pDeviceContext; }
	const WCHAR** GetVSFiles() { return m_VSFiles; }
	const WCHAR** GetPSFiles() { return m_PSFiles; }

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

