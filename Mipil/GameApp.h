#pragma once

#include  "../Engine/CommonApp.h"

/// <summary>
///	���� �����Ǵ� ���� ���ø����̼�
/// �Ŵ������� Init, Update, Render�Ѵ�.
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
	// __suer::init ������ ���̴� ���� �̸� �����ؾ���
	bool Initialize() final;
	void Update() final;
	void Render() final;
	void Finalize() final;

private:
	bool InitializeScene();	// ������� �غ�
	void FinalizeScene();

	bool InitializeImGUI();
	void FinalizeImGUI();

	void IncreaseModel();
	void DecreaseModel();

	LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam) override;
};

