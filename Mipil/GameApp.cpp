#include "framework.h"
#include "GameApp.h"

#include <directxtk/SimpleMath.h>

#pragma comment (lib, "d3d11.lib")
#pragma comment(linker, "/entry:wWinMainCRTStartup /subsystem:console")

using namespace DirectX;
using namespace DirectX::SimpleMath;

int APIENTRY wWinMain(_In_ HINSTANCE hInstance,
	_In_opt_ HINSTANCE hPrevInstance,
	_In_ LPWSTR    lpCmdLine,
	_In_ int       nCmdShow)
{
	UNREFERENCED_PARAMETER(hPrevInstance);
	UNREFERENCED_PARAMETER(lpCmdLine);

	// Debug Memory Leak Check at start point
	//_CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);
	//_CrtSetBreakAlloc(17369);

	// 전역 문자열을 초기화합니다.
	GameApp App(hInstance);
	App.Initialize();
	App.Run();
	App.Finalize();

	// Debug Memory Leak Check at end point
	_CrtSetReportMode(_CRT_WARN, _CRTDBG_MODE_DEBUG);
	_CrtDumpMemoryLeaks();

	return (int)1;
}

GameApp::GameApp(HINSTANCE hInstance)
	: CommonApp(hInstance)
{
}

bool GameApp::Initialize()
{
	m_PSFiles[0] = L"StaticVertexShader";
	m_PSFiles[1] = L"SkeletalVertexShader";
	m_VSFiles[0] = L"PBRPixelShader";

	QueryPerformanceFrequency(&m_frequency);
	QueryPerformanceCounter(&m_previousTime);
	QueryPerformanceCounter(&m_currentTime);

	return __super::Initialize();
}

void GameApp::Update()
{
	CommonApp::Update();
}

void GameApp::Render()
{
	__super::Render();
}

void GameApp::Finalize()
{
	CommonApp::Finalize();
}

bool GameApp::InitializeScene()
{
	return true;
}

void GameApp::FinalizeScene()
{
}

bool GameApp::InitializeImGUI()
{
	return true;
}

void GameApp::FinalizeImGUI()
{
}

void GameApp::IncreaseModel()
{
}

void GameApp::DecreaseModel()
{
}

LRESULT GameApp::WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	return __super::WndProc(hWnd, message, wParam, lParam);
}
