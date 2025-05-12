#pragma once
#include <chrono>
#include<windows.h>
#include<vector>
#include<memory>
#include"../../Application.h"
#include"Camera.h"
#include"../../Common/Fader.h"

class SceneBase;
class Fader;
class Camera;

class SceneManager
{

public:

	static constexpr int PLAYER_NUM = 4;	//�v���C���[�l��

	// �V�[���Ǘ��p
	enum class SCENE_ID
	{
		NONE,
		TITLE,
		SELECT,
		GAME,
		GAMECLEAR,
		GAMEOVER,
		EXP,
	};

	//�R���g���[���n��
	enum class CNTL
	{
		NONE,
		KEYBOARD,
		PAD
	};


	//�v���C���[���[�h
	enum class PLAY_MODE
	{
		USER,
		CPU,
		MAX
	};

	//��E
	enum class ROLE
	{
		NONE,
		KNIGHT,
		AXEMAN,
		MAGE,
		ARCHER
	};
	
	//�萔

	// �C���X�^���X�̐���
	static void CreateInstance(void);

	// �C���X�^���X�̎擾
	static SceneManager& GetInstance(void);

	void Init(void);
	void Init3D(void);
	void Update(void);
	void Draw(void);

	// ���\�[�X�̔j��
	void Destroy(void);

	// ��ԑJ��
	void ChangeScene(SCENE_ID nextId);

	// �V�[��ID�̎擾
	SCENE_ID GetSceneID(void);

	// �f���^�^�C���̎擾
	float GetDeltaTime(void) const;

	// �J�����̎擾
	std::vector<std::shared_ptr<Camera>> GetCameras(void) const;
	//�J��������ɖ߂�
	void ResetCameras(void);


	//�E�B���h�E�֌W
	// ************************************************************************
	//�E�B���h�E�̒ǉ�
	void SetSubWindowH(HWND _mode);
	//�E�B���h�E�̏���(���̌��Ɍ������J�����̕�������)
	void RedySubWindow(void);

	//���[�h��ς���
	void ChangeWindowMode(const Application::WINDOW _mode);
	//���C���E�B���h�E�ȊO��HIDE��Ԃɂ���
	void SetHideSubWindows(void);
	//�t���X�N�ɖ߂�
	void ReturnSolo(void);

	//�E�B���h�E�̈ʒu��T�C�Y�̒���
	void SetWindowPram(void);

	//�g�p����E�B���h�E�̐��̃Q�b�^�[
	const int GetActiveNum(void) { return activeWindowNum_; }
	//��L�̃Z�b�^�[
	void SetActiveNum(const int _num) { activeWindowNum_ = _num; }

	//���ݕ`�撆�̃E�B���h�E�̔ԍ����擾
	const int GetNowWindow(void)const;

private:
	// �ÓI�C���X�^���X
	static SceneManager* instance_;

	//�E�B���h�E�֌W
	std::vector<HWND> subWindowH_;	//�E�B���h�E�n���h���̑S�̊Ǘ�(���I�z��)
	int activeWindowNum_;			//�g�p����E�B���h�E�̐�
	int nowWindowNum_;				//���݂̃E�B���h�E

	SCENE_ID sceneId_;				//���݂̃V�[�����
	SCENE_ID waitSceneId_;			//���̃V�[��

	// �t�F�[�h
	std::unique_ptr<Fader> fader_;

	// �e��V�[��
	SceneBase* scene_;

	// �J����
	std::vector<std::shared_ptr<Camera>> cameras_;

	// �V�[���J�ڒ�����
	bool isSceneChanging_;

	// �f���^�^�C��
	std::chrono::system_clock::time_point preTime_;
	float deltaTime_;
	
	// �f�t�H���g�R���X�g���N�^��private�ɂ��āA
	// �O�����琶���ł��Ȃ��l�ɂ���
	SceneManager(void);
	// �R�s�[�R���X�g���N�^�����l
	SceneManager(const SceneManager&);
	// �f�X�g���N�^�����l
	~SceneManager(void) = default;

	// �f���^�^�C�������Z�b�g����
	void ResetDeltaTime(void);

	// �V�[���J��
	void DoChangeScene(SCENE_ID sceneId);

	// �t�F�[�h
	void Fade(void);

	//���݉��Ԗڂ̃E�B���h�E�����Z�b�g
	void SetNowWindow(const int _num);
};