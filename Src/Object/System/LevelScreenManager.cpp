#include <DxLib.h>
#include "../../Application.h"
#include "../../Manager/Decoration/SoundManager.h"
#include "../../Manager/Generic/InputManager.h"
#include "../../Manager/Generic/ResourceManager.h"
#include "../../Manager/Decoration/EffectManager.h"
#include "../../Manager/GameSystem/DataBank.h"
#include "../../Common/Vector2.h"
#include "../../Common/Fader.h"
#include "../../Common/ShaderFade.h"
#include "LevelNotice.h"
#include "LevelSelect.h"

LevelScreenManager::LevelScreenManager()
{
	notice_ = nullptr;
	select_ = nullptr;
	fader_ = nullptr;
	exp_ = -1.0f;
	restExp_ = -1.0f;
	nowLevel_ = -1;
	gauge_ = -1.0f;
	alpha_ = -1.0f;
	state_ = STATE::NONE;
	isFader_ = false;

	// ��ԊǗ�
	stateChanges_.emplace(STATE::NONE, std::bind(&LevelScreenManager::ChangeStateNone, this));
	stateChanges_.emplace(STATE::NOTICE, std::bind(&LevelScreenManager::ChangeStateNotice, this));
	stateChanges_.emplace(STATE::SELECT, std::bind(&LevelScreenManager::ChangeStateSelect, this));
	stateChanges_.emplace(STATE::END, std::bind(&LevelScreenManager::ChangeStateEnd, this));
}

LevelScreenManager::~LevelScreenManager()
{
}

void LevelScreenManager::Init()
{
	//���x���̏����ݒ�
	nowLevel_ = DEFAULT_LEVEL;
	restExp_ = 0;

	//�C���X�^���X�ݒ�
	Load();

	//����������
	Reset();

	//�O�f�[�^�̕ێ�
	preTypeData_.resize(playerNum_, TYPE::MAX);
}

void LevelScreenManager::Update()
{
	//�t�F�[�h�X�V����
	fader_->Update();

	stateUpdate_();
}

void LevelScreenManager::ChangeState(const STATE _state)
{
	// ��Ԏ󂯎��
	state_ = _state;

	// �e��ԑJ�ڂ̏�������
	stateChanges_[state_]();
}

void LevelScreenManager::ChangeStateNone()
{	
	stateUpdate_ = std::bind(&LevelScreenManager::UpdateNone, this);
	stateDraw_ = std::bind(&LevelScreenManager::DrawNone, this);
}

void LevelScreenManager::ChangeStateNotice()
{
	stateUpdate_ = std::bind(&LevelScreenManager::UpdateNotice, this);
	stateDraw_ = std::bind(&LevelScreenManager::DrawNotice, this);

	//SE�̍Đ�
	SoundManager::GetInstance().Play(SoundManager::SOUND::LEVEL_UP);
}

void LevelScreenManager::ChangeStateSelect()
{
	stateUpdate_ = std::bind(&LevelScreenManager::UpdateSelect, this);
	stateDraw_ = std::bind(&LevelScreenManager::DrawSelect, this);
}

void LevelScreenManager::ChangeStateEnd()
{
	stateUpdate_ = std::bind(&LevelScreenManager::UpdateEnd, this);
	stateDraw_ = std::bind(&LevelScreenManager::DrawEnd, this);
}

void LevelScreenManager::UpdateNone()
{
	//�f�o�b�O����(B�Ōo���l�𓾂�)
	//DebagUpdate();

	//�o���l�̊m�F
	CheckExp();
}

void LevelScreenManager::UpdateNotice()
{
	if (!isFader_) {	
		
		//�X�L�b�v
		SkipState(STATE::SELECT);

		//�����̊g��
		notice_->Update();

		//�w�i��ʂ̓��ߏ���
		alpha_ += ALPHA_SPEED;
		if (alpha_ >= ALPHA_MAX)
		{
			alpha_ = ALPHA_MAX;
		}

		//�����̏I���m�F
		if (notice_->GetState() == LevelNotice::STATE::FIN)
		{
			isFader_ = true;
			fader_->SetFade(Fader::STATE::FADE_OUT);
		}
	}
	else
	{
		Fade();
	}
}

void LevelScreenManager::UpdateSelect()
{
	if (!isFader_)
	{
		Fade();
	}
	else
	{
		//�v���C���[�̋����̑I������
		select_->Update();

		//�����̏I���m�F
		if (select_->GetState() == LevelSelect::STATE::FIN)
		{
			ChangeState(STATE::END);
		}
	}
}

void LevelScreenManager::UpdateEnd()
{
	//�����v�f�ݒ�
	for (int i = 0; i < playerNum_; i++) {
		selectTypes_[i] = select_->GetType(i);
		preTypeData_[i] = selectTypes_[i];
	}

	//�e�탊�Z�b�g
	ChangeState(STATE::NONE);	//��ԕω�
	notice_->Reset();			//�ʒm�����̃��Z�b�g
	select_->Reset();			//�I�������̃��Z�b�g
	alpha_ = 0.0f;
	isFader_ = false;
}

void LevelScreenManager::Draw()
{
	Vector2 pos = { 0,0 };

	DrawLevelUI();
	
	//�������̓t�F�[�h���s��
	if(state_ != STATE::NONE){ FaderDraw(); }

	//��ԕʕ`�揈��
	stateDraw_();

	//�f�o�b�O�`��
	//DebagDraw();

	//�t�F�[�_�[�`��
	fader_->Draw();
}

void LevelScreenManager::DrawNone()
{
}

void LevelScreenManager::DrawNotice()
{
	notice_->Draw();
}

void LevelScreenManager::DrawSelect()
{
	select_->Draw();
}

void LevelScreenManager::DrawEnd()
{
}

void LevelScreenManager::DrawLevelUI()
{
	Vector2 pos = {
		GAGE_IMG_SIZE / 2 + GAGE_POS_X,
		GAGE_IMG_SIZE / 2 + GAGE_POS_Y };
	float percent = 100.0f / gauge_ * exp_;

	//�o���l�Q�[�W
	DrawRotaGraph(pos.x, pos.y,
		GAGE_SCALE_RATE, 0.0f, imgGage_, true, false);

	//�o���l
	DrawCircleGauge(pos.x, pos.y,
		percent, imgGageExp_, 0.0f, GAGE_SCALE_RATE);

	//1���̏ꍇ
	if (nowLevel_ < AsoUtility::TWO_DIGIT_MIN)
	{	
		//1���̏ꍇ
		DrawRotaGraph(pos.x, pos.y, NUM_SCALE_RATE, 0.0f, imgNumbers_[nowLevel_], true, false);
	}
	else
	{	
		//2���̏ꍇ
		int leftNum = nowLevel_ / AsoUtility::TWO_DIGIT_MIN;
		int rightNum = nowLevel_ % AsoUtility::TWO_DIGIT_MIN;

		//������
		Vector2 leftPos = pos;
		leftPos.x -= RIGHT_LOCAL_POS_X;
		DrawRotaGraph(leftPos.x, leftPos.y, NUM_SCALE_RATE, 0.0f, imgNumbers_[leftNum], true, false);

		//�E����
		Vector2 rightPos = pos;
		rightPos.x += LEFT_LOCAL_POS_X;
		DrawRotaGraph(rightPos.x, rightPos.y, NUM_SCALE_RATE, 0.0f, imgNumbers_[rightNum], true, false);
	}
}

void LevelScreenManager::Release()
{
	notice_->Release();
	select_->Release();
}

void LevelScreenManager::Load()
{
	//�e�C���X�^���X�ǂݍ���
	notice_ = std::make_unique<LevelNotice>();
	notice_->Init();

	select_ = std::make_unique<LevelSelect>();
	select_->Init();

	fader_ = std::make_unique<ShaderFade>();
	fader_->Init();

	//�摜
	imgGage_ = ResourceManager::GetInstance().Load(ResourceManager::SRC::CIRCLE_GAGE).handleId_;
	imgGageExp_ = ResourceManager::GetInstance().Load(ResourceManager::SRC::CIRCLE_EXP_GAGE).handleId_;
	imgNumbers_ = ResourceManager::GetInstance().Load(ResourceManager::SRC::NUMBERS).handleIds_;

	//�G�t�F�N�g�֌W
	EffectManager::GetInstance().Add(
		EffectManager::EFFECT::ATTACK_UP,
		ResourceManager::GetInstance().Load(ResourceManager::SRC::ATTACK_UP_EFE).handleId_);

	EffectManager::GetInstance().Add(
		EffectManager::EFFECT::DEFENCE_UP,
		ResourceManager::GetInstance().Load(ResourceManager::SRC::DEFENCE_UP_EFE).handleId_);

	EffectManager::GetInstance().Add(
		EffectManager::EFFECT::SPEED_UP,
		ResourceManager::GetInstance().Load(ResourceManager::SRC::SPEED_UP_EFE).handleId_);

	EffectManager::GetInstance().Add(
		EffectManager::EFFECT::LIFE_UP,
		ResourceManager::GetInstance().Load(ResourceManager::SRC::LIFE_UP_EFE).handleId_);

	//SE�̓o�^
	SoundManager::GetInstance().Add(
		SoundManager::TYPE::SE,
		SoundManager::SOUND::LEVEL_UP,
		ResourceManager::GetInstance().Load(ResourceManager::SRC::LEVEL_UP_SE).handleId_);

	SoundManager::GetInstance().Add(
		SoundManager::TYPE::SE,
		SoundManager::SOUND::POWER_UP,
		ResourceManager::GetInstance().Load(ResourceManager::SRC::POWER_UP_SE).handleId_);
}

void LevelScreenManager::Reset()
{
	//�e�v���C���[���Ƃ̋����v�f������
	playerNum_ = DataBank::GetInstance().Output(DataBank::INFO::USER_NUM);
	selectTypes_.resize(playerNum_, TYPE::MAX);

	//�o���l�Q�[�W��
	gauge_ = CONSTANT_GAGE;

	//�����o���l
	exp_ = 0.0f;

	//��ʓ��ߒl
	alpha_ = 0.0f;

	//���
	ChangeState(STATE::NONE);
}

void LevelScreenManager::SetGage(const int _level)
{
	gauge_ = CONSTANT_GAGE * (1.0f + (_level / LEVEL_DIV));
}

void LevelScreenManager::Reflection(PlayerBase& _player, const int _playerNum)
{
	//���f����
	TYPE type = selectTypes_[_playerNum];

	switch (type)
	{
	case TYPE::ATTACK:
		_player.SetAttack(ADD_ATK_PER);

		//�ύX�ӏ�
		_player.SetPreStatus();

		//�����̒l�͏㏸%�̒l
		//���݂̃X�e�[�^�X��%�ŏ㏸������
		EffectManager::GetInstance().Play(
			EffectManager::EFFECT::ATTACK_UP,
			_player.GetPos(),
			Quaternion(),
			EFFECT_SCALE,SoundManager::SOUND::POWER_UP);
		break;

	case TYPE::DEFENSE:
		_player.SetDefense(ADD_DEF_PER);

		//�ύX�ӏ�
		_player.SetPreStatus();

		EffectManager::GetInstance().Play(
			EffectManager::EFFECT::DEFENCE_UP,
			_player.GetPos(),
			Quaternion(),
			EFFECT_SCALE, SoundManager::SOUND::POWER_UP);
		break;

	case TYPE::LIFE:
		_player.SetHpMax(ADD_LIFE);
		EffectManager::GetInstance().Play(
			EffectManager::EFFECT::LIFE_UP,
			_player.GetPos(),
			Quaternion(),
			EFFECT_SCALE, SoundManager::SOUND::POWER_UP);

		break;

	case TYPE::SPEED:
		_player.SetSpeed(ADD_SPEED_PER);

		//�ύX�ӏ�
		_player.SetPreStatus();

		EffectManager::GetInstance().Play(
			EffectManager::EFFECT::SPEED_UP,
			_player.GetPos(),
			Quaternion(),
			EFFECT_SCALE, SoundManager::SOUND::POWER_UP);
		break;

	default:
		break;
	}


	selectTypes_[_playerNum] = TYPE::MAX;
}

void LevelScreenManager::CheckExp()
{
	//�o���l��1�����₷
	if (restExp_ > 0)
	{
		restExp_-= EXP_SPEED;
		exp_+= EXP_SPEED;
	}

	//�o���l�����̃��x�������܂ŒB������
	if (exp_ >= gauge_)
	{
		//���x�����グ��
		nowLevel_++;	
		
		//�o���l�̏�����
		exp_ = 0.0f;					
		
		//�ʒm�Ɉڂ�			
		ChangeState(STATE::NOTICE);

		//���x���̎擾
		notice_->SetNewLevel(nowLevel_);
	}
}

void LevelScreenManager::EffectSyne(PlayerBase& _player, const int _playerNum)
{
	//���f����
	TYPE type = preTypeData_[_playerNum];
	EffectManager::EFFECT effectType = EffectManager::EFFECT::NONE;
	switch (type)
	{
	case TYPE::ATTACK:
		effectType = EffectManager::EFFECT::ATTACK_UP;
		break;

	case TYPE::DEFENSE:
		effectType = EffectManager::EFFECT::DEFENCE_UP;
		break;

	case TYPE::LIFE:
		effectType = EffectManager::EFFECT::LIFE_UP;
		break;

	case TYPE::SPEED:
		effectType = EffectManager::EFFECT::SPEED_UP;
		break;

	default:
		break;
	}

	//�G�t�F�N�g�ݒ�
	EffectManager::GetInstance().SyncEffect(
		effectType,
		_player.GetPos(),
		Quaternion(),
		EFFECT_SCALE);
}

void LevelScreenManager::FaderDraw()
{
	SetDrawBlendMode(DX_BLENDMODE_ALPHA, (int)alpha_);
	DrawBox(
		0, 0,
		Application::SCREEN_SIZE_X,
		Application::SCREEN_SIZE_Y,
		AsoUtility::BLACK, true);
	SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);
}

void LevelScreenManager::Fade()
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
			isFader_ = false;
		}
		break;
	case Fader::STATE::FADE_OUT:
		// �Ó]��
		if (fader_->IsEnd())
		{
			// �Ó]���疾�]��
			fader_->SetFade(Fader::STATE::FADE_IN);

			//��ԕύX
			ChangeState(STATE::SELECT);
		}
		break;
	}
}

void LevelScreenManager::SkipState(const STATE& _nextState)
{
	// �V�[���J��
	InputManager& ins = InputManager::GetInstance();
	if (ins.IsTrgDown(KEY_INPUT_RETURN)||
		ins.IsPadBtnTrgDown(InputManager::JOYPAD_NO::PAD1,InputManager::JOYPAD_BTN::RIGHT)||
		ins.IsPadBtnTrgDown(InputManager::JOYPAD_NO::PAD2,InputManager::JOYPAD_BTN::RIGHT)||
		ins.IsPadBtnTrgDown(InputManager::JOYPAD_NO::PAD3,InputManager::JOYPAD_BTN::RIGHT)||
		ins.IsPadBtnTrgDown(InputManager::JOYPAD_NO::PAD4,InputManager::JOYPAD_BTN::RIGHT))
	{
		ChangeState(_nextState);
		isFader_ = true;
		select_->SetSkipState();
	}
}

void LevelScreenManager::DebagUpdate()
{
	// �V�[���J��
	InputManager& ins = InputManager::GetInstance();
	if (ins.IsTrgDown(KEY_INPUT_B))
	{
		AddExp(DEBAG_EXP);
	}
}

void LevelScreenManager::DebagDraw()
{
	Vector2 pos = { 0,0 };
	constexpr int INTERVAL = 16;
	DrawFormatString
	(pos.x, pos.y, AsoUtility::WHITE, "���݂̌o���l%2f", exp_);
	pos.y += INTERVAL;
	DrawFormatString
	(pos.x, pos.y, AsoUtility::WHITE, "���݂̃��x��%d", nowLevel_);
	pos.y += INTERVAL;
	DrawFormatString
	(pos.x, pos.y, AsoUtility::WHITE, "���݂̃X�e�[�g%d", static_cast<int>(state_));
}