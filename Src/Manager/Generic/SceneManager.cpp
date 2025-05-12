#include <chrono>
#include <DxLib.h>
#include<EffekseerForDXLib.h>
#include<cassert>

#include "../Scene/TitleScene.h"
#include "../Scene/SelectScene.h"
#include "../Scene/GameScene.h"
#include "../Scene/GameOverScene.h"
#include "../Scene/GameClearScene.h"
#include "../Scene/ExplanScene.h"
#include "ResourceManager.h"
#include "Camera.h"
#include"../Manager/GameSystem/Collision.h"
#include"../Manager/GameSystem/DataBank.h"
#include"../Manager/GameSystem/CharacterParamData.h"
#include"../Manager/Decoration/EffectManager.h"
#include"../Manager/Decoration/SoundManager.h"
#include"../Manager/GameSystem/Timer.h"
#include"../Shader/PixelShader.h"
#include "SceneManager.h"

SceneManager* SceneManager::instance_ = nullptr;

void SceneManager::CreateInstance()
{
	if (instance_ == nullptr){
		instance_ = new SceneManager();
	}
	instance_->Init();
}

SceneManager& SceneManager::GetInstance(void)
{
	return *instance_;
}

void SceneManager::Init(void)
{
	//�e�}�l�[�W���̐���
	//����̐���
	Collision::CreateInstance();
	DataBank::CreateInstance();
	EffectManager::CreateInstance();
	SoundManager::CreateInstance();
	PixelShader::CreateInstance();

	sceneId_ = SCENE_ID::TITLE;
	waitSceneId_ = SCENE_ID::NONE;

	CharacterParamData::CreateInstance();

	fader_ = std::make_unique<Fader>();
	fader_->Init();

	// �J����
	std::shared_ptr<Camera>c = std::make_shared<Camera>();
	c->Init();
	cameras_.push_back(std::move(c));

	scene_ = new TitleScene();
	scene_->Init();

	isSceneChanging_ = false;

	// �f���^�^�C��
	preTime_ = std::chrono::system_clock::now();

	// 3D�p�̐ݒ�
	Init3D();

	// �����V�[���̐ݒ�
	DoChangeScene(SCENE_ID::TITLE);

	//���C���E�B���h�E��ǉ�
	subWindowH_.push_back(NULL);
	activeWindowNum_ = 1;	//���C�����A�N�e�B�u�ɂ���̂ŏ����l�P
	nowWindowNum_ = 0;
}

void SceneManager::Init3D(void)
{

	// �w�i�F�ݒ�
	SetBackgroundColor(0, 0, 0);

	// Z�o�b�t�@��L���ɂ���
	SetUseZBuffer3D(true);

	// Z�o�b�t�@�ւ̏������݂�L���ɂ���
	SetWriteZBuffer3D(true);

	// �o�b�N�J�����O��L���ɂ���
	SetUseBackCulling(true);

	// ���C�g�̐ݒ�
	SetUseLighting(true);

	// ���ʂ���΂߉��Ɍ����������C�g
	ChangeLightTypeDir({ 0.00f, -1.00f, 1.00f });

	// ���C�g�̐ݒ�
	//ChangeLightTypeDir({ 0.3f, -0.7f, 0.8f });

	// �t�H�O�ݒ�
	SetFogEnable(true);
	SetFogColor(5, 5, 5);
	SetFogStartEnd(10000.0f, 20000.0f);

}

void SceneManager::Update(void)
{

	if (scene_ == nullptr)return;

	// �f���^�^�C��
	auto nowTime = std::chrono::system_clock::now();
	deltaTime_ = static_cast<float>(
		std::chrono::duration_cast<std::chrono::nanoseconds>(nowTime - preTime_).count() / 1000000000.0);
	preTime_ = nowTime;

	fader_->Update();
	if (isSceneChanging_){
		Fade();
	}
	else{
		scene_->Update();
	}

	// �J�����X�V
	for (auto& c : cameras_){
		c->Update();
	}
}

void SceneManager::Draw(void)
{
	//�J�E���^�i�z��v�f��)
	int cnt = 0;

	// �`���O���t�B�b�N�̈�̎w��
	// (�R�c�`��Ŏg�p����J�����̐ݒ�Ȃǂ����Z�b�g�����)
	SetDrawScreen(DX_SCREEN_BACK);

	for (HWND hwnd : subWindowH_)
	{
		//�J�E���^���E�B���h�E�̃A�N�e�B�u���𒴂�����I��
		if (cnt >= activeWindowNum_)return;

		//�t�����g�o�b�t�@�̉摜������
		ClearDrawScreen();

		if (hwnd){
			SetScreenFlipTargetWindow(hwnd); // �ǉ��E�B���h�E���^�[�Q�b�g�ɐݒ�

		}else{
			SetScreenFlipTargetWindow(NULL); // ���C���E�B���h�E���^�[�Q�b�g�ɐݒ�
		}
		//�J�����̕`��
		cameras_[cnt]->SetBeforeDraw();

		// Effekseer�ɂ��Đ����̃G�t�F�N�g���X�V����B
		UpdateEffekseer3D();
		//���݂̃E�B���h�E���Z�b�g
		SetNowWindow(cnt);

		//�Q�[�����e�`��
		// �`��
		scene_->Draw();

		// Effekseer�ɂ��Đ����̃G�t�F�N�g��`�悷��B
		DrawEffekseer3D();


		// �Ó]�E���]
		fader_->Draw();

		//�t�����g�o�b�t�@�ɏ����o��
		ScreenFlip();
		//�J�E���^����
		cnt++;
	}
}

void SceneManager::Destroy(void)
{

	scene_->Release();
	delete scene_;

	// �J����
	for (auto& c : cameras_){
		c->Release();
	}

	DataBank::GetInstance().Destroy();
	EffectManager::GetInstance().Destroy();
	SoundManager::GetInstance().Destroy();
	PixelShader::GetInstance().Destroy();
	Timer::GetInstance().Destroy();

	delete instance_;
}

void SceneManager::ChangeScene(SCENE_ID nextId)
{

	// �t�F�[�h�������I����Ă���V�[����ς���ꍇ�����邽�߁A
	// �J�ڐ�V�[���������o�ϐ��ɕێ�
	waitSceneId_ = nextId;

	// �t�F�[�h�A�E�g(�Ó])���J�n����
	fader_->SetFade(Fader::STATE::FADE_OUT);
	isSceneChanging_ = true;

}

SceneManager::SCENE_ID SceneManager::GetSceneID(void)
{
	return sceneId_;
}

float SceneManager::GetDeltaTime(void) const
{
	//return 1.0f / 60.0f;
	return deltaTime_;
}
//�J�������i�S�́j���擾
std::vector<std::shared_ptr<Camera>> SceneManager::GetCameras(void) const
{
	return cameras_;
}
//�J������v�f���@�ɂȂ�܂ō폜����
void SceneManager::ResetCameras(void)
{
	auto size = cameras_.size();
	//������������Ă����̂ōŌ�̈���c��悤�ɒ���
	size -= 1;
	for (int i = 0; i < size; i++)
	{
		cameras_.pop_back();
	}
}


/// <summary>
/// �E�B���h�E�̒ǉ�
/// </summary>
/// <param name="_mode"></param>
void SceneManager::SetSubWindowH(HWND _mode)
{
	subWindowH_.push_back(_mode);
}

void SceneManager::RedySubWindow(void)
{
	//�E�B���h�E�̐ݒ�
	int num = DataBank::GetInstance().Output(DataBank::INFO::USER_NUM);
	SetActiveNum(num);
	ChangeWindowMode(Application::WINDOW::SHOW);
	SetWindowPram();

	//���łɈ�͐�������Ă���̂ŏ����l�͇@
	for (int i = 1; i < num; i++)
	{
		//�����y�я�����
		auto c = std::make_shared<Camera>();
		c->Init();
		//�i�[
		cameras_.push_back(std::move(c));
	}
}

//�E�B���h�E�̏�Ԃ�ς���
void SceneManager::ChangeWindowMode(const Application::WINDOW _mode)
{
	int cnt = 1;
	for (HWND hwnd : subWindowH_)
	{
		if (cnt > activeWindowNum_)return;
		if (cnt == 1)
		{
			cnt++;
			continue;
		}
		ShowWindow(hwnd, static_cast<int>(_mode));

		cnt++;
	}
}
//�T�u�E�B���h�E���B����Ԃ�(�Q�[���V�[������ʂ̃V�[���Ɉړ�����Ƃ��Ɏg�p)
void SceneManager::SetHideSubWindows(void)
{
	//�����G�ɍ��̂ŗv������
	//�����������炱�̊֐����炢��Ȃ��\���͂���
	int cnt = 1;
	for (HWND hwnd : subWindowH_)
	{
		if (cnt == 1)	//���C���E�B���h�E�̓X�L�b�v
		{
			cnt++;
			continue;
		}
		ShowWindow(hwnd, static_cast<int>(Application::WINDOW::HIDE));

		cnt++;
	}
}

//�t���X�N���[���ɖ߂�
void SceneManager::ReturnSolo(void)
{
	//��ʖ������ꖇ�ɖ߂�
	SetActiveNum(1);
	//�T�u�E�B���h�E���B��
	SetHideSubWindows();
	//�t���X�N�ɖ߂�
	SetWindowSize(1960, 1080);
}



SceneManager::SceneManager(void)
{

	sceneId_ = SCENE_ID::NONE;
	waitSceneId_ = SCENE_ID::NONE;

	scene_ = nullptr;
	fader_ = nullptr;

	isSceneChanging_ = false;
	// �f���^�^�C��
	deltaTime_ = 1.0f / 60.0f;	
}

void SceneManager::ResetDeltaTime(void)
{
	deltaTime_ = 0.016f;
	preTime_ = std::chrono::system_clock::now();
}

void SceneManager::DoChangeScene(SCENE_ID sceneId)
{
	auto& resM = ResourceManager::GetInstance();

	// ���\�[�X�̉��
	resM.Release();
	EffectManager::GetInstance().Release();
	SoundManager::GetInstance().Release();

	// �V�[����ύX����
	sceneId_ = sceneId;

	// ���݂̃V�[�������
	if (scene_ != nullptr)
	{
		scene_->Release();
		delete scene_;
	}

	switch (sceneId_)
	{
	case SCENE_ID::TITLE:
		ChangeWindowMode(Application::WINDOW::HIDE);
		scene_ = new TitleScene();
		resM.InitTitle();
		break;		
	
	case SCENE_ID::SELECT:
		scene_ = new SelectScene();
   		resM.InitSelect();
		break;	
	
	case SCENE_ID::GAME:
		//�E�B���h�E�̐ݒ�

		

		RedySubWindow();
		scene_ = new GameScene();
		resM.InitGame();
		break;

	case SCENE_ID::GAMEOVER:
		scene_ = new GameOverScene();
		resM.InitGameOver();
		break;

	case SCENE_ID::GAMECLEAR:
		scene_ = new GameClearScene();
		resM.InitGameClear();
		break;

	case SCENE_ID::EXP:
		scene_ = new ExplanScene();
		resM.InitExplan();
		break;
	}

	scene_->Init();

	ResetDeltaTime();

	waitSceneId_ = SCENE_ID::NONE;

}

void SceneManager::Fade(void)
{

	Fader::STATE fState = fader_->GetState();
	switch (fState)
	{
	case Fader::STATE::FADE_IN:
		// ���]��
		if (fader_->IsEnd())
		{
			// ���]���I��������A�t�F�[�h�����I��
			fader_->SetFade(Fader::STATE::NONE);
			isSceneChanging_ = false;
		}
		break;
	case Fader::STATE::FADE_OUT:
		// �Ó]��
		if (fader_->IsEnd())
		{
			// ���S�ɈÓ]���Ă���V�[���J��
			DoChangeScene(waitSceneId_);
			// �Ó]���疾�]��
			fader_->SetFade(Fader::STATE::FADE_IN);
		}
		break;
	}
}

void SceneManager::SetNowWindow(const int _num)
{
	nowWindowNum_ = _num;
}

//�E�B���h�E�̃T�C�Y�y�шʒu�ݒ�
void SceneManager::SetWindowPram(void)
{
	//���݂̓f�B�X�v���C�ꖇ�Ɖ��肵�Đ��삵�Ă���
	//��őΉ��ł̐��삪�K�v
	const int DISPLAY_X = 1920;
	const int DISPLAY_Y = 1080;


	//�z��̗v�f���Ȃ̂ł������l�O
	int cnt = 0;

	int posX = 0;
	int posY = 0;


	int displayNum = DataBank::GetInstance().Output(DataBank::INFO::DHISPLAY_NUM);
	int userNum = DataBank::GetInstance().Output(DataBank::INFO::USER_NUM);

	int r = 1;
	//�f�B�X�v���C���]���Ă���Ƃ�
	if (displayNum > userNum) r = userNum;
	else r = displayNum;


	int sizeX = DISPLAY_X * r;
	int sizeY = DISPLAY_Y;

	if (activeWindowNum_ != displayNum)sizeX /= 2;
	if (activeWindowNum_ > displayNum)sizeY /= 2;
	if (sizeX > DISPLAY_X)sizeX = DISPLAY_X;

	for (HWND hwnd : subWindowH_)
	{
		//cnt�͔z��̗v�f����\���̂�activeWindow�����̊Ԃ���
		if (cnt >= activeWindowNum_)return;
		cnt++;

		if (cnt == 1)
		{
			SetWindowSize(sizeX - 15, sizeY - 30);
			//SetWindowSizeExtendRate(0.95, 0.95);
			SetWindowPosition(posX, posY);
		}
		else
		{
			SetWindowPos(hwnd, NULL, posX, posY, sizeX, sizeY, NULL);
			//MoveWindow(hwnd, posX, posY, sizeX, sizeY, true);
		}
		posX += sizeX;

		if (cnt == 2)
		{
			posY += sizeY;
			posX = 0;
		}
	}
}

const int SceneManager::GetNowWindow(void) const
{
	return nowWindowNum_;
}
