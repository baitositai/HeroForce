#pragma once
#include <functional>
#include <memory>
#include <map>
#include "SceneBase.h"
#include "../Object/Character/PlayableChara/Other/OverPlayers.h"

class OverPlayers;

class GameOverScene : public SceneBase
{
public:

	//���
	enum class STATE
	{
		DOWN,		//�v���C���[���|���
		DISPLAY,	//���b�Z�[�W�̕\��
	};

	GameOverScene();
	~GameOverScene() = default;

	//���b�Z�[�W�ʒu
	static constexpr int MES_DEFAULT_POS_X = Application::SCREEN_SIZE_X / 2;
	static constexpr int MES_DEFAULT_POS_Y = Application::SCREEN_SIZE_Y / 2 - 100;

	static constexpr int MES2_DEFAULT_POS_X = Application::SCREEN_SIZE_X / 2;
	static constexpr int MES2_DEFAULT_POS_Y = Application::SCREEN_SIZE_Y / 2 +100;

	//�J�����֘A
	static constexpr VECTOR DEFAULT_CAMERA_POS = { 0.0f, 100.0f, -500.0f };		//�J�����̍��W
	static constexpr VECTOR DEFAULT_TARGET_POS = { 0.0f, 150.0f, -100.0f };		//�J�����̒����_���W

	//�A���t�@�l�ő�
	static constexpr int ALPHA_MAX = 255;
	static constexpr int ALPHA_MIN = 130;

	//��ԑJ�ڕb��
	static constexpr float CHANGE_SECOND = 2.0f;

	void Init() override;
	void Update() override;
	void Draw() override;
	void Release() override;

private:

	//���
	STATE state_;

	//�摜
	int imgGameOver_;		//�Q�[���I�[�o�[�摜
	int imgBack_;			//�w�i�摜
	int imgMes_;			//���b�Z�[�W�摜

	//�X�V�p�X�e�b�v
	float step_;

	//�A���t�@�l
	float alpha_;
	float alphaAdd_;

	// ��ԊǗ�(��ԑJ�ڎ���������)
	std::map<STATE, std::function<void()>> stateChanges_;

	// ��ԊǗ�
	std::function<void()> stateUpdate_;	//�X�V

	//��ԕύX
	void ChangeState(const STATE state);
	void ChangeStateDown();
	void ChangeStateDisplay();

	//�e��X�V����
	void UpdateDown();
	void UpdateDisplay();
	
	//�v���C���[
	std::unique_ptr<OverPlayers> player_;
};

