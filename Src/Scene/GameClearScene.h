#pragma once
#include <functional>
#include <memory>
#include <map>
#include <DxLib.h>
#include "../Application.h"
#include "../Utility/AsoUtility.h"
#include "../Manager/Decoration/EffectManager.h"
#include "../Manager/Decoration/SoundManager.h"
#include "../Manager/Generic/ResourceManager.h"
#include "../Manager/Generic/SceneManager.h"
#include "../Manager/Generic/InputManager.h"
#include "../Manager/Generic/Camera.h"
#include "../Manager/GameSystem/DataBank.h"
#include "../Object/Character/PlayableChara/Other/ClearPlayers.h"
#include "SceneBase.h"

class SkyDome;
class StageManager;
class ClearChicken;
class ClearPlayers;
class GameClearScene : public SceneBase
{
public:

	//���
	enum class STATE
	{
		HAPPY,		//���̊Ԃ̊��
		DISPLAY,	//���b�Z�[�W�̕\��
	};

	//�`�L��������
	static constexpr int CHICKEN_CREATES = 4;

	//���b�Z�[�W�ʒu
	static constexpr int MES_POS_X = Application::SCREEN_SIZE_X / 2;
	static constexpr int MES_POS_Y = Application::SCREEN_SIZE_Y / 2 - 100;

	static constexpr int MES2_POS_X = Application::SCREEN_SIZE_X / 2;
	static constexpr int MES2_POS_Y = Application::SCREEN_SIZE_Y / 2 + 100;

	//�J�����֘A
	static constexpr VECTOR DEFAULT_CAMERA_POS = { 0.0f, 100.0f, -500.0f };		//�J�����̍��W
	static constexpr VECTOR DEFAULT_TARGET_POS = { 0.0f, 150.0f, -100.0f };		//�J�����̒����_���W

	static constexpr VECTOR CHICKEN_POS_1 = { -200.0f, -50.0f, 20.0f };
	static constexpr VECTOR CHICKEN_POS_2 = { -120.0f, -50.0f, 30.0f };
	static constexpr VECTOR CHICKEN_POS_3 = { 150.0f, -50.0f, 40.0f };
	static constexpr VECTOR CHICKEN_POS_4 = { 230.0f, -50.0f, 40.0f };

	//�G�t�F�N�g�Đ��ʒu
	static constexpr VECTOR EFC_POS = { 0.0f, -100.0f, 100.0f };		

	//�G�t�F�N�g�T�C�Y
	static constexpr float EFC_SIZE = 20.0f;

	//�A���t�@�l�ő�
	static constexpr int ALPHA_MAX = 255;
	static constexpr int ALPHA_MIN = 130;

	//��ԑJ�ڕb��
	static constexpr float CHANGE_SECOND = 2.0f;

	//�R���X�g���N�^
	GameClearScene();

	//�f�X�g���N�^
	~GameClearScene() = default;

	void Init() override;
	void Update() override;
	void Draw() override;
	void Release() override;

private:

	//�N���A���b�Z�[�W�摜
	int imgClear_;

	//�V�[���J�ڃ��b�Z�[�W�摜
	int imgMes_;
	
	//�X�V�p
	float step_;

	//�A���t�@�l
	float alpha_;

	//�A���t�@�ǉ��l
	float alphaAdd_;

	//���
	STATE state_;

	//�X�J�C�h�[��
	std::unique_ptr<SkyDome> sky_;

	//�`�L��
	std::unique_ptr<ClearChicken> chickens_[CHICKEN_CREATES];

	//�v���C���[
	std::unique_ptr<ClearPlayers> player_;

	//�X�e�[�W
	std::unique_ptr<StageManager> stage_;

	// ��ԊǗ�(��ԑJ�ڎ���������)
	std::map<STATE, std::function<void()>> stateChanges_;

	// ��ԊǗ�
	std::function<void()> stateUpdate_;	//�X�V

	//��ԕύX
	void ChangeState(const STATE state);
	void ChangeStateHappy();
	void ChangeStateDisplay();

	//�e��X�V����
	void UpdateHappy();
	void UpdateDisplay();
};