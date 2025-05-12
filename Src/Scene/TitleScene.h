#pragma once
#include "SceneBase.h"
#include "../Application.h"
#include "../Object/Common/Transform.h"
#include "../Object/Stage/SkyDome.h"

class SpaceDome;

class TitleScene : public SceneBase
{

public:

	//�J�����֘A
	static constexpr VECTOR DEFAULT_CAMERA_POS = { 0.0f, 300.0f, 0.0f };		//�J�����̍��W
	static constexpr VECTOR DEFAULT_TARGET_POS = { 0.0f, 1000.0f, 800.0f };		//�J�����̒����_���W

	//���S�ʒu
	static constexpr int LOGO_POS_X = Application::SCREEN_SIZE_X / 2;
	static constexpr int LOGO_POS_Y = Application::SCREEN_SIZE_Y / 2 -30;

	//���b�Z�[�W�T�C�Y
	static constexpr int MES_SIZE_X = 300;
	static constexpr int MES_SIZE_Y = 44;

	//���b�Z�[�W�ʒu
	static constexpr int MES_POS_X = Application::SCREEN_SIZE_X / 2 - MES_SIZE_X / 2;
	static constexpr int MES_POS_Y = Application::SCREEN_SIZE_Y - 100;


	// �R���X�g���N�^
	TitleScene();

	//�f�X�g���N�^
	~TitleScene() = default;

	void Init() override;
	void Update() override;
	void Draw() override;
	void Release() override;

private:

	//�X�V�p�X�e�b�v
	float step_;

	// �^�C�g�����S
	int imgLogo_;
	int imgMes_;

	//�X�J�C�h�[��
	std::unique_ptr<SkyDome> sky_;

};
