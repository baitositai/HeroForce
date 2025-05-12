#include "../Object/Stage/SkyDome.h"
#include "../Object/Stage/StageManager.h"
#include "../Object/Character/Chiken/ClearChicken.h"
#include "../Object/Character/PlayableChara/Other/ClearPlayers.h"
#include "GameClearScene.h"

GameClearScene::GameClearScene()
{
	sky_ = nullptr;
	stage_ = nullptr;
	for (auto& c : chickens_) { c = nullptr; }
	player_ = nullptr;
	imgMes_ = -1;
	state_ = STATE::HAPPY;
	stateChanges_.emplace(STATE::HAPPY, std::bind(&GameClearScene::ChangeStateHappy, this));
	stateChanges_.emplace(STATE::DISPLAY, std::bind(&GameClearScene::ChangeStateDisplay, this));
}

void GameClearScene::Init()
{
	auto& res = ResourceManager::GetInstance();
	auto& snd = SoundManager::GetInstance();

	//�X�J�C�h�[��
	sky_ = std::make_unique<SkyDome>();
	sky_->Init();

	//�X�e�[�W
	stage_ = std::make_unique<StageManager>();
	stage_->Init();

	//�`�L��
	VECTOR pos[CHICKEN_CREATES] = { CHICKEN_POS_1,CHICKEN_POS_2,CHICKEN_POS_3,CHICKEN_POS_4 };
	for (int i = 0; i < CHICKEN_CREATES; i++)
	{
		chickens_[i] = std::make_unique<ClearChicken>();
		chickens_[i]->Create(pos[i]);
		chickens_[i]->SetTarget(DEFAULT_CAMERA_POS);
	}

	//�v���C���[
	player_ = std::make_unique<ClearPlayers>();
	player_->Init();

	//�摜
	imgMes_ = ResourceManager::GetInstance().Load(ResourceManager::SRC::CHANGE_TITLE_UI).handleId_;
	imgClear_ = ResourceManager::GetInstance().Load(ResourceManager::SRC::CONGRATULATIONS).handleId_;

	//�G�t�F�N�g
	EffectManager::GetInstance().Add(EffectManager::EFFECT::FIREWORK,
		ResourceManager::GetInstance().Load(ResourceManager::SRC::FIREWORK).handleId_);

	// �J�������[�h�F��_�J����
	auto camera = SceneManager::GetInstance().GetCameras();
	for (auto& c : camera)
	{
		c->SetPos(DEFAULT_CAMERA_POS, DEFAULT_TARGET_POS);
		c->ChangeMode(Camera::MODE::FIXED_POINT);
	}
	
	

	auto& efe = EffectManager::GetInstance();
	efe.Play(
		EffectManager::EFFECT::FIREWORK,
		EFC_POS,
		Quaternion(),
		EFC_SIZE,
		SoundManager::SOUND::NONE);

	//���֌W
	snd.Add(SoundManager::TYPE::SE, SoundManager::SOUND::GAMECLEAR_SE,
		res.Load(ResourceManager::SRC::GAMECLEAR_SE).handleId_);
	snd.Add(SoundManager::TYPE::BGM, SoundManager::SOUND::GAMECLEAR_BGM,
		res.Load(ResourceManager::SRC::GAMECLEAR_BGM).handleId_);

	//�ŏ���SE���Đ�������
	snd.Play(SoundManager::SOUND::GAMECLEAR_SE);

	ChangeState(STATE::HAPPY);
}

void GameClearScene::Update()
{	
	auto& efe = EffectManager::GetInstance();
	auto& ins = InputManager::GetInstance();
	auto& scm = SceneManager::GetInstance();
	auto& snd = SoundManager::GetInstance();

	//��Ԃ��Ƃ̍X�V����
	stateUpdate_();

	//�e��I�u�W�F�N�g����
	sky_->Update();
	stage_->Update();
	for (auto& c : chickens_) { c->Update(); }
	player_->Update();

	//�G�t�F�N�g����Đ����̏ꍇ
	if (efe.IsPlayEffect(EffectManager::EFFECT::FIREWORK))
	{
		//�G�t�F�N�g���Đ�
		efe.Play(
			EffectManager::EFFECT::FIREWORK,
			EFC_POS,
			Quaternion(),
			EFC_SIZE,
			SoundManager::SOUND::NONE);
	}

	// �V�[���J��
	if (ins.IsTrgDown(KEY_INPUT_SPACE) ||
		ins.IsPadBtnNew(InputManager::JOYPAD_NO::PAD1, InputManager::JOYPAD_BTN::RIGHT) ||
		ins.IsPadBtnNew(InputManager::JOYPAD_NO::PAD2, InputManager::JOYPAD_BTN::RIGHT) ||
		ins.IsPadBtnNew(InputManager::JOYPAD_NO::PAD3, InputManager::JOYPAD_BTN::RIGHT) ||
		ins.IsPadBtnNew(InputManager::JOYPAD_NO::PAD4, InputManager::JOYPAD_BTN::RIGHT))
	{
		scm.ChangeScene(SceneManager::SCENE_ID::TITLE);
		snd.Stop(SoundManager::SOUND::GAMECLEAR_BGM);
		snd.Stop(SoundManager::SOUND::GAMECLEAR_SE);
		efe.Stop(EffectManager::EFFECT::FIREWORK);
	}
}

void GameClearScene::Draw()
{
	//��
	sky_->Draw();

	//�X�e�[�W
	stage_->Draw();

	//�v���C���[
	player_->Draw();

	//�`�L��
	for (auto& c : chickens_) { c->Draw(); }

	//�N���A���b�Z�[�W�̕`��
	DrawRotaGraph(
		MES_POS_X, MES_POS_Y,
		1.0f,
		0.0f,
		imgClear_,
		true,
		false);

	//�V�[���J�ڃ��b�Z�[�W�̕`��
	if (state_ == STATE::DISPLAY) {
		SetDrawBlendMode(DX_BLENDMODE_ALPHA, (int)alpha_);
		DrawRotaGraph(
			MES2_POS_X,
			MES2_POS_Y,
			1.0f,
			0.0f,
			imgMes_,
			true);
		SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);
	}
}

void GameClearScene::Release()
{
	SceneManager::GetInstance().ResetCameras();
	SceneManager::GetInstance().ReturnSolo();

	sky_->Release();
	stage_->Destroy();
}

void GameClearScene::ChangeState(const STATE state)
{	
	// ��Ԏ󂯎��
	state_ = state;

	// �e��ԑJ�ڂ̏�������
	stateChanges_[state_]();
}

void GameClearScene::ChangeStateHappy()
{
	stateUpdate_ = std::bind(&GameClearScene::UpdateHappy, this);
}

void GameClearScene::ChangeStateDisplay()
{	
	stateUpdate_ = std::bind(&GameClearScene::UpdateDisplay, this);
}

void GameClearScene::UpdateHappy()
{
	step_ += SceneManager::GetInstance().GetDeltaTime();

	//���b���ŏ�ԑJ��
	if (step_ >= CHANGE_SECOND)
	{
		SoundManager::GetInstance().Stop(SoundManager::SOUND::GAMECLEAR_SE);
		SoundManager::GetInstance().Play(SoundManager::SOUND::GAMECLEAR_BGM);
		ChangeState(STATE::DISPLAY);
	}
}

void GameClearScene::UpdateDisplay()
{
	//�A���t�@�l�̌v�Z
	alpha_ += alphaAdd_;
	if (alpha_ > ALPHA_MAX) { alphaAdd_ = -1.0f; }
	else if (alpha_ < ALPHA_MIN) { alphaAdd_ = 1.0f; }
}