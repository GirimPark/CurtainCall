#include "pch.h"
#include "CommonApp.h"

#include "Helper.h"

#include "../D3DRenderer/D3DRenderer.h"

#ifdef ENGINE_DEBUG
#pragma comment(linker, "/entry:wWinMainCRTStartup /subsystem:console")
#endif

CommonApp* CommonApp::m_pInstance = nullptr;
HWND CommonApp::m_hWnd;

LRESULT CALLBACK DefaultWndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
    return CommonApp::m_pInstance->WndProc(hWnd, message, wParam, lParam);
}

CommonApp::CommonApp(HINSTANCE hInstance)
    : m_hInstance(hInstance)
    , m_szWindowClass(L"DefaultWindowClass")
    , m_szTitle(L"CommonApp")
{
    CommonApp::m_pInstance = this;

    m_wcex.hInstance = hInstance;
    m_wcex.cbSize = sizeof(WNDCLASSEX);
    m_wcex.style = CS_HREDRAW | CS_VREDRAW;
    m_wcex.lpfnWndProc = DefaultWndProc;
    m_wcex.cbClsExtra = 0;
    m_wcex.cbWndExtra = 0;
    m_wcex.hCursor = LoadCursor(nullptr, IDC_ARROW);
    m_wcex.hbrBackground = (HBRUSH)(COLOR_WINDOW + 1);
    m_wcex.lpszClassName = m_szWindowClass;

    m_pD3DRenderer = std::make_shared<D3DRenderer>(m_hWnd, m_VSFiles, m_PSFiles, ScreenWidth, ScreenHeight);
}

bool CommonApp::Initialize()
{
    // 등록
    RegisterClassExW(&m_wcex);

    RECT rcClinet = { 0, 0, ScreenWidth, ScreenHeight };
    AdjustWindowRect(&rcClinet, WS_OVERLAPPEDWINDOW, FALSE);

    // 윈도우 화면 중앙에 띄우기
    const int screenCenterX = GetSystemMetrics(SM_CXSCREEN) / 2;
    const int screenCenterY = GetSystemMetrics(SM_CYSCREEN) / 2;
    const int startX = screenCenterX - ScreenWidth / 2;
    const int startY = screenCenterY - ScreenHeight / 2;

    // 윈도우 생성
    m_hWnd = CreateWindowW(m_szWindowClass, m_szTitle, WS_OVERLAPPEDWINDOW,
        startX, startY,                       // 시작 위치
        rcClinet.right - rcClinet.left, rcClinet.bottom - rcClinet.top,                     // 가로, 세로
        nullptr, nullptr, m_hInstance, nullptr);

    if (!m_hWnd)
    {
        return false;
    }

    SetForegroundWindow(m_hWnd);    // 윈도우를 최상위로
    SetFocus(m_hWnd);               // 윈도우에 포커스를 준다
    ShowCursor(TRUE);	            // 커서 보이기

    // 윈도우 보이기
    ShowWindow(m_hWnd, SW_SHOW);
    UpdateWindow(m_hWnd);

    // Renderer 초기화
    HR_T(m_pD3DRenderer->Initialize());

    m_pDevice = m_pD3DRenderer->GetDevice();
    m_pDeviceContext = m_pD3DRenderer->GetDevcon();
    m_pSwapChain = m_pD3DRenderer->GetSwapChain();

    return true;
}

void CommonApp::Run()
{
    while (true)
    {
        if (PeekMessage(&m_msg, NULL, 0, 0, PM_REMOVE))
        {
            if (m_msg.message == WM_QUIT)
                break;

            if (!TranslateAccelerator(m_msg.hwnd, m_hAccelTable, &m_msg))
            {
                TranslateMessage(&m_msg);
                DispatchMessage(&m_msg);
            }
        }
        else
        {
            Update();
            Render();
        }
    }
}

void CommonApp::Update()
{
}

void CommonApp::Render()
{
    m_pD3DRenderer->Render();

    // 매니저 렌더링

    m_pSwapChain->Present(0, 0);
}

void CommonApp::Finalize()
{
}

//extern IMGUI_IMPL_API LRESULT ImGui_ImplWin32_WndProcHandler(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);

LRESULT CommonApp::WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
    //if (ImGui_ImplWin32_WndProcHandler(hWnd, message, wParam, lParam))
    //{
    //    return true;
    //}

    switch (message)
    {
    case WM_DESTROY:
        PostQuitMessage(0);
        break;
    default:
        return DefWindowProc(hWnd, message, wParam, lParam);
    }

    return 0;
}
