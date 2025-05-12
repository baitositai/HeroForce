#pragma once
#include <string>
#include<windows.h>

//#define DEBUG_SCREEN

class Application
{

public:
#ifdef DEBUG_SCREEN
	static constexpr int SCREEN_SIZE_X = 800;
	static constexpr int SCREEN_SIZE_Y = 600;
	// �X�N���[���T�C�Y

	//static constexpr int SCREEN_SIZE_X = 1920;
	//static constexpr int SCREEN_SIZE_Y = 1080;
#endif // DEBUG_SCREEN


	static constexpr int DEFA_SCREEN_SIZE_X = 1920;
	static constexpr int DEFA_SCREEN_SIZE_Y = 1080;

	static constexpr int SCREEN_SIZE_X = 800;
	static constexpr int SCREEN_SIZE_Y = 600;

	//FPS
	static constexpr int DEFAULT_FPS = 60;
	static constexpr float FRAME_RATE = 1000 / 60;
	static constexpr float FPS_USE_SUBWINDOW = 1000000 / 60;

	//�E�B���h�E
	static constexpr int SUBWINDOW_NUM = 3;

	// �f�[�^�p�X�֘A
	//-------------------------------------------
	static const std::string PATH_IMAGE;
	static const std::string PATH_MODEL;
	static const std::string PATH_ENEMY;
	static const std::string PATH_PLAYER;
	static const std::string PATH_ARROW;
	static const std::string PATH_STAGE;
	static const std::string PATH_SKYDOME;
	static const std::string PATH_OBJECT;
	static const std::string PATH_EFFECT;
	static const std::string PATH_TEXT;
	static const std::string PATH_FONT;
	static const std::string PATH_JSON;
	static const std::string PATH_BGM;
	static const std::string PATH_WAVE;
	//-------------------------------------------

	//�E�B���h�E���[�h�ݒ�
	enum class WINDOW
	{
		HIDE,			//��\��
		NOMAL,			//�ʏ�
		MIN,			//�ŏ����\��
		MAX,			//�ő剻�\��
		NOACTIVE,		//�\�����邪�A�N�e�B�u�ł͂Ȃ�
		SHOW,			//�\��(���ݏ�ԂɈˑ�)
		MINIMIZE,		//�ŏ�������
		MIN_NOACTIVE,	//�ŏ������ăA�N�e�B�u���Ȃ���
		SHOW_NOACTIVE,	//�\�����邪�A�N�e�B�u�ɂ͂��Ȃ�
		SIZE_RESET,		//�T�C�Y�����ɖ߂�
		DEFAULT,		//������Ԃŕ\��
	};

	//�T�u�E�B���h�E�̌�(�厖�Ȃ̂͗v�f��)
	//���C���E�B���h�E�����łɂ���̂Œǉ��ō��͎̂O���܂�
	const char* szClassNme[SUBWINDOW_NUM] =
	{
	"window1",
	"window2",
	"window3",
	//�����ƃE�B���h�E�����ꍇ�͂����𑝂₵�܂�
	};

	// �����I�ɃC���X�e���X�𐶐�����
	static void CreateInstance(void);

	// �ÓI�C���X�^���X�̎擾
	static Application& GetInstance(void);

	// ������
	void Init(void);

	// �Q�[�����[�v�̊J�n
	void Run(void);

	// ���\�[�X�̔j��
	void Destroy(void);

	// �����������^���s�̔���
	bool IsInitFail(void) const;

	// ��������^���s�̔���
	bool IsReleaseFail(void) const;

private:

	//�E�B���h�E�n���h��
	HWND hWnd_;

	//�t���[���Œ�p
	int currentFrame_;	//���݂̃t���[����ۑ�
	int lastFrame_;		//�Ō�Ɏ��s�����t���[����ۑ�

	// �ÓI�C���X�^���X
	static Application* instance_;

	// ���������s
	bool isInitFail_;

	// ������s
	bool isReleaseFail_;


	//�E�B���h�E�̏�����
	void InitWindows(const int _num);

	//�G�t�F�N�V�A�̏�����
	void InitEffekseer(void);

	// �f�t�H���g�R���X�g���N�^��private�ɂ��āA
	// �O�����琶���ł��Ȃ��l�ɂ���
	Application(void);

	// �R�s�[�R���X�g���N�^�����l
	Application(const Application&);

	// �f�X�g���N�^�����l
	~Application(void) = default;

};