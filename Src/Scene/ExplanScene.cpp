#include "../Manager/Generic/ResourceManager.h"
#include "../Manager/Generic/SceneManager.h"
#include "../Manager/Generic/InputManager.h"
#include "../Manager/Decoration/SoundManager.h"
#include "../Manager/GameSystem/TextManager.h"
#include "ExplanScene.h"


ExplanScene::ExplanScene()
{
	int i = -1;
	cntExp_ = -1;
	alpha_ = -1.0;
	alphaAdd_ = -1.0;
	isStick_ = false;
	font_ = -1;
	imgPoint_ =&i;
	step_ = -1.0f;
	for (int i = 0; i < EXPLAN_CNT; i++) {
		imgExp_[i] = -1;
	}
}

void ExplanScene::Init()
{
	auto& res = ResourceManager::GetInstance();
	auto& snd = SoundManager::GetInstance();

	//�摜�ǂݍ���
	imgExp_[0] = res.Load(ResourceManager::SRC::EXPLAN).handleId_;
	imgExp_[1] = res.Load(ResourceManager::SRC::EXPLAN2).handleId_;
	imgExp_[2] = res.Load(ResourceManager::SRC::EXPLAN3).handleId_;
	imgExp_[3] = res.Load(ResourceManager::SRC::EXPLAN4).handleId_;
	imgExp_[4] = res.Load(ResourceManager::SRC::EXPLAN5).handleId_;
	imgExp_[5] = res.Load(ResourceManager::SRC::EXPLAN6).handleId_;
	imgExp_[6] = res.Load(ResourceManager::SRC::EXPLAN7).handleId_;
	imgPoint_ = res.Load(ResourceManager::SRC::LEFT_POINT).handleIds_;

	//���ʉ�
	SoundManager::GetInstance().Add(
		SoundManager::TYPE::SE,
		SoundManager::SOUND::EXPLAN_SWITCH,
		res.Load(ResourceManager::SRC::EXPLAN_SWITCH).handleId_);
	SoundManager::GetInstance().Add(
		SoundManager::TYPE::SE,
		SoundManager::SOUND::SCENE_CHANGE_1,
		res.Load(ResourceManager::SRC::SCENE_CHANGE_SE1).handleId_);


	//�t�H���g����
	font_ = CreateFontToHandle(
		TextManager::GetInstance().GetFontName(TextManager::FONT_TYPE::LOGO).c_str(),
		FONT_SIZE,
		0);

	//�����ԍ�
	cntExp_ = 0;
	alphaAdd_ = 1.0f;
	alpha_ = ALPHA_MAX;
	step_ = 0.0f;
}

void ExplanScene::Update()
{
	auto& ins = InputManager::GetInstance();
	auto& snd = SoundManager::GetInstance();
	auto& mng = SceneManager::GetInstance();

	//�V�[���J��
	if (ins.IsTrgDown(KEY_INPUT_SPACE) ||
		ins.IsPadBtnNew(InputManager::JOYPAD_NO::PAD1, InputManager::JOYPAD_BTN::RIGHT) ||
		ins.IsPadBtnNew(InputManager::JOYPAD_NO::PAD2, InputManager::JOYPAD_BTN::RIGHT) ||
		ins.IsPadBtnNew(InputManager::JOYPAD_NO::PAD3, InputManager::JOYPAD_BTN::RIGHT) ||
		ins.IsPadBtnNew(InputManager::JOYPAD_NO::PAD4, InputManager::JOYPAD_BTN::RIGHT))
	{
		snd.Play(SoundManager::SOUND::SCENE_CHANGE_1);
		mng.ChangeScene(SceneManager::SCENE_ID::GAME);
	}

	//��������ς���
	float stickX = ins.GetJPadInputState(InputManager::JOYPAD_NO::PAD1).AKeyLX;
	if (ins.IsTrgDown(KEY_INPUT_A) ||
		ins.IsTrgDown(KEY_INPUT_LEFT) ||
		stickX < 0 && !isStick_)
	{
		//1�y�[�W�߂�
		cntExp_--;

		//0�ȉ��̎��ő�y�[�W�ɂ���
		if (cntExp_ < 0) { cntExp_ = EXPLAN_CNT - 1; }

		//���ʉ��Đ�
		snd.Play(SoundManager::SOUND::EXPLAN_SWITCH);

		//�X�e�B�b�N�͓��삵�Ă�
		isStick_ = true;
	}
	else if (ins.IsTrgDown(KEY_INPUT_D) ||
		ins.IsTrgDown(KEY_INPUT_RIGHT) ||
		stickX > 0 && !isStick_)
	{
		//1�y�[�W�i��
		cntExp_++;

		//�ő�y�[�W�̎�0�y�[�W�ɖ߂�
		if (cntExp_ >= EXPLAN_CNT) { cntExp_ = 0; }

		//���ʉ��Đ�
		snd.Play(SoundManager::SOUND::EXPLAN_SWITCH);

		//�X�e�B�b�N�͓��삵�Ă�
		isStick_ = true;
	}

	//���ߌv�Z
	alpha_ += alphaAdd_;
	if (alpha_ > ALPHA_MAX) { alphaAdd_ = -1.0f; }
	else if (alpha_ < ALPHA_MIN) { alphaAdd_ = 1.0f; }

	//�X�e�B�b�N��߂����Ƃ������߂�
	if (stickX == 0) { isStick_ = false; }
}

void ExplanScene::Draw()
{
	//������
	DrawRotaGraph(
		Application::SCREEN_SIZE_X / 2,
		Application::SCREEN_SIZE_Y / 2,
		EXP_RATE,
		0.0f,
		imgExp_[cntExp_],
		true
	);
	//�|�C���g��
	DrawRotaGraph(
		POINT_SIZE / 2,
		Application::SCREEN_SIZE_Y / 2,
		1.0f,
		0.0f,
		imgPoint_[0],
		true);
	//�|�C���g�E
	DrawRotaGraph(
		Application::SCREEN_SIZE_X - POINT_SIZE / 2,
		Application::SCREEN_SIZE_Y / 2,
		1.0f,
		0.0f,
		imgPoint_[0],
		true,
		true);//���]������

	//���b�Z�[�W
	SetDrawBlendMode(DX_BLENDMODE_ALPHA, (int)alpha_);
	DrawFormatStringToHandle(
		Application::SCREEN_SIZE_X / 2,
		Application::SCREEN_SIZE_Y - FONT_SIZE,
		0xffffff,
		font_,
		"SPACE or B �X�L�b�v");
	SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);
}

void ExplanScene::Release()
{
	DeleteFontToHandle(font_);
	SceneManager::GetInstance().ResetCameras();
}
