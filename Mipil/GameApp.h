#pragma once

#include  "../Engine/CommonApp.h"

/// <summary>
///	실제 구동되는 게임 애플리케이션
/// 매니저들을 Init, Update, Render한다.
/// </summary>
class GameApp
	: public CommonApp
{
private:
	LARGE_INTEGER m_frequency;
	LARGE_INTEGER m_previousTime;
	LARGE_INTEGER m_currentTime;
	float m_deltaTime;

public:
	GameApp(HINSTANCE hInstance);
	~GameApp() = default;

public:
	// __suer::init 이전에 셰이더 파일 이름 지정해야함
	bool Initialize() final;
	void Update() final;
	void Render() final;
	void Finalize() final;

private:
	bool InitializeScene();	// 상수버퍼 준비
	void FinalizeScene();

	bool InitializeImGUI();
	void FinalizeImGUI();

	void IncreaseModel();
	void DecreaseModel();

	LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam) override;
};

