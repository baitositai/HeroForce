#include <DxLib.h>
#include<EffekseerForDXLib.h>
#include "Manager/Generic/ResourceManager.h"
#include "Manager/Generic/InputManager.h"
#include "Manager/Generic/SceneManager.h"
#include "Application.h"

Application* Application::instance_ = nullptr;

//�t�@�C���w��p�X
const std::string Application::PATH_IMAGE = "Data/Image/";
const std::string Application::PATH_MODEL = "Data/Model/";
const std::string Application::PATH_ENEMY = "Data/Model/Enemy/";
const std::string Application::PATH_PLAYER = "Data/Model/Player/";
const std::string Application::PATH_ARROW = "Data/Model/Arrow/";
const std::string Application::PATH_STAGE = "Data/Model/Stage/";
const std::string Application::PATH_SKYDOME = "Data/Model/SkyDome/";
const std::string Application::PATH_OBJECT = "Data/Model/Object/";
const std::string Application::PATH_EFFECT = "Data/Effect/";
const std::string Application::PATH_TEXT = "Data/Text/";
const std::string Application::PATH_FONT = "Data/Font/";
const std::string Application::PATH_JSON = "Data/Json/";
const std::string Application::PATH_BGM = "Data/Sound/BGM/";
const std::string Application::PATH_WAVE = "Data/Sound/wave/";

void Application::CreateInstance(void)
{
	if (instance_ == nullptr)
	{
		instance_ = new Application();
	}
	instance_->Init();
}

Application& Application::GetInstance(void)
{
	return *instance_;
}

void Application::Init(void)
{

	// �A�v���P�[�V�����̏����ݒ�
	SetWindowText("Main");

	// �E�B���h�E�T�C�Y
	SetGraphMode(SCREEN_SIZE_X, SCREEN_SIZE_Y, 32);
	ChangeWindowMode(true);
	// ��A�N�e�B�u��Ԃł����삷��
	SetAlwaysRunFlag(TRUE);					

	// DxLib�̏�����
	SetUseDirect3DVersion(DX_DIRECT3D_11);
	isInitFail_ = false;
	if (DxLib_Init() == -1)
	{
		isInitFail_ = true;
		return;
	}

	//�G�t�F�N�V�A�̏�����
	InitEffekseer();

	// �L�[���䏉����
	SetUseDirectInputFlag(true);
	InputManager::CreateInstance();

	// ���\�[�X�Ǘ�������
	ResourceManager::CreateInstance();

	// �V�[���Ǘ�������
	SceneManager::CreateInstance();


	//�E�B���h�E
	for (int i = 0; i < SUBWINDOW_NUM; i++)
	{
		InitWindows(i);
		SceneManager::GetInstance().SetSubWindowH(hWnd_); // SceneManager�ɗp�ӂ���HWND subWindowH_�ϐ��ɃZ�b�g����֐�
	}

	//FPS�p������
	currentFrame_ = 0;
	lastFrame_ = 0;
}

void Application::Run(void)
{

	auto& inputManager = InputManager::GetInstance();
	auto& sceneManager = SceneManager::GetInstance();
	//�E�B���h�E�֌W
	MSG msg;
	LONGLONG time = GetNowHiPerformanceCount();
	LONGLONG temp_time;

	// �Q�[�����[�v
	while (ProcessMessage() == 0 && CheckHitKey(KEY_INPUT_ESCAPE) == 0)
	{
		//�E�B���h�E���b�Z�[�W(�����)
		if (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE))
		{
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}

		Sleep(1);	//�V�X�e���ɏ�����Ԃ�
		currentFrame_ = GetNowCount();	//���݂̃t���[�������l��

		//���݂̃t���[���ƍŌ�̎��s�t���[���̍��������l�𒴂�����X�V�������s���B
		if (currentFrame_ - lastFrame_ >= FRAME_RATE)
		{
			lastFrame_ = currentFrame_;	//�t���[���̍X�V
			InputManager::GetInstance().Update();
			sceneManager.Update();	//�X�V
		}
		//sceneManager.Update();


		sceneManager.Draw();

	
		//�T�u�E�B���h�E�֌W
		temp_time = GetNowHiPerformanceCount();
		while (temp_time - time < FPS_USE_SUBWINDOW)
		{
			Sleep(0);
			temp_time = GetNowHiPerformanceCount();
		}
		time = temp_time;
	}

}

void Application::Destroy(void)
{

	InputManager::GetInstance().Destroy();
	ResourceManager::GetInstance().Destroy();
	SceneManager::GetInstance().Destroy();
	
	// Effekseer���I������B
	Effkseer_End();

	// DxLib�I��
	if (DxLib_End() == -1)
	{
		isReleaseFail_ = true;
	}

	delete instance_;

}

bool Application::IsInitFail(void) const
{
	return isInitFail_;
}

bool Application::IsReleaseFail(void) const
{
	return isReleaseFail_;
}

//�E�B���h�E�֌W
//********************************************************************************
LRESULT CALLBACK WndProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	switch (msg)
	{
	case WM_DESTROY:
		PostQuitMessage(0);
		break;
	default:
		return DefWindowProc(hWnd, msg, wParam, lParam);
	}
	return 0;
}
/// <summary>
/// �E�B���h�E�̏�����
/// </summary>
/// <param name="_num">�E�B���h�E�ԍ�</param>
void Application::InitWindows(const int _num)
{
	LPCSTR windowName = "01234" + _num;

	HINSTANCE hInstance = GetModuleHandle(NULL);
	WNDCLASS myWindow;
	myWindow.style = CS_HREDRAW | CS_VREDRAW;
	myWindow.lpfnWndProc = WndProc;
	myWindow.cbClsExtra = 0;
	myWindow.cbWndExtra = 0;
	myWindow.hInstance = hInstance;
	myWindow.hIcon = NULL;
	myWindow.hCursor = LoadCursor(NULL, IDC_ARROW);
	myWindow.hbrBackground = (HBRUSH)GetStockObject(BLACK_BRUSH);
	myWindow.lpszMenuName = NULL;
	myWindow.lpszClassName = szClassNme[_num];
	if (!RegisterClass(&myWindow))
	{
		return;
	}
	RECT windowRect = { 0, 0, SCREEN_SIZE_X, SCREEN_SIZE_Y };
	AdjustWindowRect(&windowRect, WS_OVERLAPPEDWINDOW, FALSE);
	hWnd_ = CreateWindow(
		szClassNme[_num],
		windowName,
		WS_OVERLAPPEDWINDOW,
		0, 0, windowRect.right - windowRect.left,
		windowRect.bottom - windowRect.top,
		NULL, NULL, hInstance, NULL
	);
	//�\����Ԃ�ݒ肷��
	ShowWindow(hWnd_, static_cast<int>(WINDOW::HIDE));
	UpdateWindow(hWnd_);
}

void Application::InitEffekseer(void)
{
	if (Effekseer_Init(8000) == -1)
	{
		DxLib_End();
	}
	SetChangeScreenModeGraphicsSystemResetFlag(FALSE);
	Effekseer_SetGraphicsDeviceLostCallbackFunctions();

}

Application::Application(void)
{
	isInitFail_ = false;
	isReleaseFail_ = false;
}
