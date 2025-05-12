//#include <string>
#include <DxLib.h>
#include "../Application.h"
#include "../Utility/AsoUtility.h"
#include "../Manager/Decoration/SoundManager.h"
#include "../Manager/Generic/ResourceManager.h"
#include "../Manager/Generic/SceneManager.h"
#include "../Manager/Generic/InputManager.h"
#include "../Manager/Generic/Camera.h"
#include "../Shader/PixelShader.h"
#include "TitleScene.h"

TitleScene::TitleScene()
{
	imgLogo_ = -1;
	imgMes_ = -1;
	sky_ = nullptr;
	step_ = -1.0;
}

void TitleScene::Init()
{
	auto& snd = SoundManager::GetInstance();
	auto& res = ResourceManager::GetInstance();

	//�X�J�C�h�[��
	sky_ = std::make_unique<SkyDome>();
	sky_->Init();

	//�摜�ǂݍ���
	imgLogo_ = res.Load(ResourceManager::SRC::TITLE_LOGO).handleId_;
	imgMes_ = res.Load(ResourceManager::SRC::PLEASE_KEY).handleId_;

	//���y�ǂݍ���
	snd.Add(SoundManager::TYPE::BGM, SoundManager::SOUND::TITLE,
		res.Load(ResourceManager::SRC::TITLE_BGM).handleId_);

	snd.Add(SoundManager::TYPE::SE, SoundManager::SOUND::SCENE_CHANGE_1,
		res.Load(ResourceManager::SRC::SCENE_CHANGE_SE1).handleId_);

	//���y�Đ�
	snd.Play(SoundManager::SOUND::TITLE);

	int ret = res.Load(ResourceManager::SRC::SCENE_CHANGE_SE1).handleId_;
	if (ret == -1)
	{
		return;
	}

	//�J�����ݒ�
	auto camera = SceneManager::GetInstance().GetCameras();
	camera[0]->SetPos(DEFAULT_CAMERA_POS, DEFAULT_TARGET_POS);
	camera[0]->ChangeMode(Camera::MODE::FIXED_POINT);
}

void TitleScene::Update()
{
	InputManager& ins = InputManager::GetInstance();
	SceneManager& mng = SceneManager::GetInstance();
	auto& snd = SoundManager::GetInstance();

	//�J�E���g�X�V
	step_ += mng.GetDeltaTime();

	//�X�J�C�h�[���X�V
	sky_->Update();

	// �V�[���J��
	if (ins.IsTrgDown(KEY_INPUT_SPACE)|| 
		ins.IsPadBtnNew(InputManager::JOYPAD_NO::PAD1, InputManager::JOYPAD_BTN::RIGHT) ||
		ins.IsPadBtnNew(InputManager::JOYPAD_NO::PAD2, InputManager::JOYPAD_BTN::RIGHT) ||
		ins.IsPadBtnNew(InputManager::JOYPAD_NO::PAD3, InputManager::JOYPAD_BTN::RIGHT) ||
		ins.IsPadBtnNew(InputManager::JOYPAD_NO::PAD4, InputManager::JOYPAD_BTN::RIGHT))
	{
		snd.Stop(SoundManager::SOUND::TITLE);
		snd.Play(SoundManager::SOUND::SCENE_CHANGE_1);
		mng.ChangeScene(SceneManager::SCENE_ID::SELECT);
	}
}

void TitleScene::Draw()
{
	//�X�J�C�h�[���`��
	sky_->Draw();

	//���S�̕`��
	DrawRotaGraph(
		LOGO_POS_X,LOGO_POS_Y,
		1.0f,
		0.0f,
		imgLogo_,
		true,
		false);

	auto& ps = PixelShader::GetInstance();
	//���b�Z�[�W�`��
	COLOR_F buf = COLOR_F();//= COLOR_F{ step_ };
	buf.r = step_;

	ps.DrawExtendGraphToShader(
		{ MES_POS_X,MES_POS_Y },
		{ MES_SIZE_X,
		  MES_SIZE_Y},
		imgMes_,
		PixelShader::PS_TYPE::COLOR_BLINK,
		buf
	);
}

void TitleScene::Release()
{
	sky_->Release();
}
