#include <DxLib.h>
#include "../../Application.h"
#include "../../Manager/Generic/ResourceManager.h"
#include "../../Manager/Generic/SceneManager.h"
#include "../../Manager/GameSystem/TextManager.h"
#include "../../Manager/Generic/Camera.h"
#include "../../Common/Vector2.h"
#include "LevelNotice.h"

LevelNotice::LevelNotice() 
{
	newLevel_ = -1;
	cnt_ = -1;
	scl_ = -1.0f;
	state_ = STATE::NONE;
	alphaMes_ = -1.0f;
	alphaLevel_ = -1.0f;
	fontMes_ = -1;
	fontLevel_ = -1;
	text_ = "";

	// ��ԊǗ�
	stateChanges_.emplace(STATE::NONE, std::bind(&LevelNotice::ChangeStateNone, this));
	stateChanges_.emplace(STATE::FADE_IN, std::bind(&LevelNotice::ChangeStateFade, this));
	stateChanges_.emplace(STATE::MAINTAIN, std::bind(&LevelNotice::ChangeStateMaintain, this));
	stateChanges_.emplace(STATE::FIN, std::bind(&LevelNotice::ChangeStateFin , this));
}

void LevelNotice::Init()
{
	//�ǂݍ���
	Load();

	//������
	Reset();
}

void LevelNotice::Update()
{
	//�X�V����
	stateUpdate_();
}

void LevelNotice::Draw()
{
	//�`��ݒ�
	Vector2 pos ={ 0,0 };
	float angle = 0.0f;
	bool trans = true;
	bool reverse = false;
	int c = TextManager::CENTER_TEXT;
	int cH = TextManager::CENTER_TEXT_H;	
	
	//�G�t�F�N�g�`��
	if (isEfe_) {
		DrawRotaGraph(EFFECT_POS_X,
			EFFECT_POS_Y,
			EFFECT_RATE,
			0.0f,
			imgEfe_[efeAnimNum_],
			true,
			false);
	}

	//���b�Z�[�W�`��
	SetDrawBlendMode(DX_BLENDMODE_ALPHA, (int)alphaMes_);
	DrawMessage();	
	SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);

	//���x���̕`��
	SetDrawBlendMode(DX_BLENDMODE_ALPHA, (int)alphaLevel_);
	DrawLevel();
	SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);
}

void LevelNotice::Release()
{
	DeleteFontToHandle(fontMes_);
	DeleteFontToHandle(fontLevel_);
}

void LevelNotice::Load()
{
	//�e�L�X�g
	auto& text_m = TextManager::GetInstance();
	text_ = text_m.TextLoad(TextManager::TEXTS::LEVEL_UP);

	//�t�H���g
	fontMes_ = CreateFontToHandle(
		text_m.GetFontName(TextManager::FONT_TYPE::KAKUMEI).c_str(),
		FONT_TEXT_SIZE,
		FONT_THICK);
	fontLevel_ = CreateFontToHandle(
		text_m.GetFontName(TextManager::FONT_TYPE::KAKUMEI).c_str(),
		FONT_LEVEL_SIZE,
		FONT_THICK);

	//�G�t�F�N�g
	imgEfe_ = ResourceManager::GetInstance().Load(ResourceManager::SRC::LEVEL_SCREEN_EFE).handleIds_;
}

void LevelNotice::Reset()
{
	cnt_ = START_ALPHA_LEVEL;
	scl_ = 1.0f;	
	alphaMes_ = 0.0f;
	alphaLevel_ = 0.0f;
	ChangeState(LevelNotice::STATE::NONE);
	efeStep_ = 0;
	efeSpeed_ = EFFECT_ANIM_SPPED;
	efeAnimNum_ = 0;
	isEfe_ = false;
}

void LevelNotice::ChangeState(const STATE _state)
{
	// ��Ԏ󂯎��
	state_ = _state;

	// �e��ԑJ�ڂ̏�������
	stateChanges_[state_]();
}

void LevelNotice::SetNewLevel(const int& _newLevel)
{
	newLevel_ = _newLevel;
}

void LevelNotice::ChangeStateNone()
{
	stateUpdate_ = std::bind(&LevelNotice::UpdateNone, this);
}

void LevelNotice::ChangeStateFade()
{
	stateUpdate_ = std::bind(&LevelNotice::UpdateFade, this);
}

void LevelNotice::ChangeStateMaintain()
{
	stateUpdate_ = std::bind(&LevelNotice::UpdateMaintain, this);
}

void LevelNotice::ChangeStateFin()
{
	stateUpdate_ = std::bind(&LevelNotice::UpdateFin, this);
}

void LevelNotice::UpdateNone()
{
	ChangeState(STATE::FADE_IN);
}

void LevelNotice::UpdateFade()
{
	float rate = EXPANSION_RATE;	
	float max = Fader::ALPHA_MAX;		

	//�J�E���g�X�V
	cnt_ -= SceneManager::GetInstance().GetDeltaTime();

	//���ߏ���
	alphaMes_ += rate;
	if (cnt_ <= 0.0f) { 
		alphaLevel_ += rate; 
		isEfe_ = true;
		EffectUpdate();
	}

	//�A���t�@�l�̍ő�m�F
	if (alphaMes_ >= max) { alphaMes_ = max; }
	if (alphaLevel_ >= max) //���x���̃A���t�@�l���ő�ɂȂ������͏�Ԃ�ς���
	{
		alphaLevel_ = max; 
		cnt_ = MAINTAIN_SECONDS;
		ChangeState(STATE::MAINTAIN);
	}
}

void LevelNotice::UpdateMaintain()
{
	//�J�E���g�X�V
	cnt_ -= SceneManager::GetInstance().GetDeltaTime();
	
	//�G�t�F�N�g�X�V
	EffectUpdate();

	if (cnt_ <= 0.0f)
	{
		//���̏�Ԃ֕ύX
		ChangeState(STATE::FIN);
	}
}

void LevelNotice::UpdateFin()
{
}

void LevelNotice::EffectUpdate()
{
	//�G�t�F�N�g�X�V����
	efeStep_++;

	//�A�j���[�V�����ԍ����蓖��
	const int ANIM_NUM = ResourceManager::LV_EFE_NUM_X * ResourceManager::LV_EFE_NUM_Y;
	efeAnimNum_ = efeStep_ / efeSpeed_ % ANIM_NUM;
	
	//�G�t�F�N�g�̔�\��
	if (efeSpeed_ * ANIM_NUM < efeStep_) {
		isEfe_ = false;
	}
}

void LevelNotice::DrawMessage()
{
	int c = TextManager::CENTER_TEXT;
	int cH = TextManager::CENTER_TEXT_H;
	int length = static_cast<int>(text_.length());

	Vector2 pos{
	Application::SCREEN_SIZE_X / c - length * FONT_TEXT_SIZE / cH - MES_TEXT_POS_X,
	Application::SCREEN_SIZE_Y / c - FONT_TEXT_SIZE / c - MES_TEXT_POS_Y };
	
	DrawFormatStringToHandle(pos.x, pos.y, AsoUtility::WHITE, fontMes_, text_.c_str());
}

void LevelNotice::DrawLevel()
{
	std::string levelText = std::to_string(newLevel_);
	int c = TextManager::CENTER_TEXT;
	int cH = TextManager::CENTER_TEXT_H;
	int length = static_cast<int>(levelText.length());
	Vector2 pos{
	Application::SCREEN_SIZE_X / c - length * FONT_LEVEL_SIZE / cH,
	(Application::SCREEN_SIZE_Y - FONT_LEVEL_SIZE) / c + LEVEL_TEXT_POS_Y };
	
	DrawFormatStringToHandle(pos.x, pos.y, AsoUtility::WHITE, fontLevel_, levelText.c_str() );
}
