#include<DxLib.h>
#include<cassert>
#include"../Manager/Decoration/SoundManager.h"
#include"../Manager/Generic/InputManager.h"
#include"../Manager/Generic/ResourceManager.h"
#include"../Manager/Generic/SceneManager.h"
#include"../Manager/GameSystem/TextManager.h"
#include"../Manager/GameSystem/DataBank.h"

#include "FazeResult.h"
FazeResult::FazeResult(void)
{
	exp_ = 0.0f;
	afterExp_ = 0.0f;
	rank_ = RANK::MAX;
	isEnd_ = false;
	imgBack_ = -1;

	int i = -1;
	imgRank_ = &i;
	state_ = STATE::NOMAL;
	isPlay_ = false;
}
FazeResult::~FazeResult(void)
{
}
void FazeResult::Init(void)
{
	//�摜�i�[
	rankImg_[static_cast<int>(RANK::S)] = ResourceManager::GetInstance().Load(ResourceManager::SRC::RANK_S).handleId_;
	rankImg_[static_cast<int>(RANK::A)] = ResourceManager::GetInstance().Load(ResourceManager::SRC::RANK_A).handleId_;
	rankImg_[static_cast<int>(RANK::B)] = ResourceManager::GetInstance().Load(ResourceManager::SRC::RANK_B).handleId_;
	rankImg_[static_cast<int>(RANK::C)] = ResourceManager::GetInstance().Load(ResourceManager::SRC::RANK_C).handleId_;
	backImg_ = ResourceManager::GetInstance().Load(ResourceManager::SRC::RESULT).handleId_;

	rank_ = RANK::S;
	isEnd_ = false;

	//�l���o���l�ʂ̓t�F�[�Y�I�����̃v���C���[�����̌o���l���f�[�^�o���N����擾����
	exp_ = 1500.0f;

	//�摜
	imgBack_ = ResourceManager::GetInstance().Load(ResourceManager::SRC::REZALT_BACK).handleId_;
	imgRank_ = ResourceManager::GetInstance().Load(ResourceManager::SRC::RANKS).handleIds_;

	//�t�H���g����
	font_ = CreateFontToHandle(
		TextManager::GetInstance().GetFontName(TextManager::FONT_TYPE::LOGO).c_str(),
		FONT_SIZE,
		0);

	heasdFont_ = CreateFontToHandle(
		TextManager::GetInstance().GetFontName(TextManager::FONT_TYPE::LOGO).c_str(),
		FONT_HEAD_SIZE,
		0);

	SoundManager::GetInstance().Add(SoundManager::TYPE::SE,
		SoundManager::SOUND::FAZE_REZALT,
		ResourceManager::GetInstance().Load(ResourceManager::SRC::FAZE_REZALT_SE).handleId_);

	SetResult();
}

void FazeResult::Update(void)
{
	if (!isPlay_) { SoundManager::GetInstance().Play(SoundManager::SOUND::FAZE_REZALT); isPlay_ = true; }

	ChangeRank();
	step_++;
}

void FazeResult::Draw(void)
{
	//�w�i
	DrawExtendGraph(
		0, 0,
		Application::SCREEN_SIZE_X,
		Application::SCREEN_SIZE_Y,
		imgBack_,
		true
	);

	int fezeCnt = 1;
	std::string mes = "%d�t�F�[�Y�I��";
	//�e�L�X�g�̕`��
	DrawFormatStringToHandle(
		HEAD_TEXT_POS_X,
		HEAD_TEXT_POS_Y - mes.length() * FONT_HEAD_SIZE / 2,
		0xffffff,
		font_,
		mes.c_str(),
		fezeCnt);

	if (step_ < INTERVEL) { return; }
	DrawFormatStringToHandle(
		MES1_POS_X, MES1_POS_Y,
		0xffffff,
		font_,
		"�G��|�������� : %d��",
		dunkEnm_);

	if (state_ == STATE::NOMAL) {
		if (step_ < INTERVEL * 2) { return; }
		DrawFormatStringToHandle(
			MES2_POS_X, MES2_POS_Y,
			0xffffff,
			font_,
			"�j���g���̐����� : %d��",
			aliveChicken_);
	}

	//�o���l�̕`��
	if (step_ < INTERVEL * 3) { return; }
	DrawFormatStringToHandle(
		MES3_POS_X, MES3_POS_Y,
		0xffffff,
		font_,
		"Exp.%d",
		static_cast<int>(afterExp_));


	//�����N�̕`��
	if (step_ < INTERVEL * 4) { return; }
	DrawRotaGraph(
		RANK_POS_X,
		RANK_POS_Y,
		0.85f,
		0.0f,
		imgRank_[static_cast<int>(rank_)],
		true,
		false);

}

void FazeResult::Release(void)
{
}

void FazeResult::Reset(void)
{
	isEnd_ = false;
	isPlay_ = false;
}

void FazeResult::SetLast(void)
{
	dunkEnm_ = DataBank::GetInstance().Output(DataBank::INFO::GAME_DUNK_ENEMY);
}

void FazeResult::SetResult(void)
{
	//���U���g���
	dunkEnm_ = DataBank::GetInstance().Output(DataBank::INFO::FAZE_DUNK_ENEMY);
	aliveChicken_ = DataBank::GetInstance().Output(DataBank::INFO::ALIVE_CHICKEN);
	//�����N�̌���
	JudgeRank();

}

float FazeResult::GetExp(void)
{
	return afterExp_;
}

float FazeResult::GetBonusExp(const RANK _rank) const
{
	float exp = exp_;

	switch (_rank)
	{
	case RANK::S:
		exp = exp * BNS_RANK_S;
		break;
	case RANK::A:
		exp = exp * BNS_RANK_A;
		break;
	case RANK::B:
		exp = exp * BNS_RANK_B;
		break;
	case RANK::C:
		exp = exp * BNS_RANK_C;
		break;
	default:
		//�f�o�b�O�p
		assert("�����N�K�p�O�̂��̂��g�p����Ă��܂��B");
		break;
	}

	return exp;
}

void FazeResult::ChangeRank(void)
{
	InputManager& ins = InputManager::GetInstance();

	if (ins.IsTrgDown(KEY_INPUT_S))rank_ = RANK::S;
	if (ins.IsTrgDown(KEY_INPUT_A))rank_ = RANK::A;
	if (ins.IsTrgDown(KEY_INPUT_B))rank_ = RANK::B;
	if (ins.IsTrgDown(KEY_INPUT_C))rank_ = RANK::C;

	//�t�F�[�Y�̏I��
	if (ins.IsTrgDown(KEY_INPUT_SPACE)||ins.IsPadBtnTrgDown(InputManager::JOYPAD_NO::PAD1,InputManager::JOYPAD_BTN::RIGHT))isEnd_ = true;

	afterExp_ = GetBonusExp(rank_);
}

void FazeResult::JudgeRank(void)
{
	int value = (aliveChicken_ * CHICKEN_VALUE) + (dunkEnm_ * ENEMY_VALUE);
	rank_ = RANK::C;

	if (value >= RANK_S_BORDER) {
		rank_ = RANK::S;
		return;
	}
	if (value >= RANK_A_BORDER) {
		rank_ = RANK::A;
		return;
	}
	if (value >= RANK_B_BORDER) {
		rank_ = RANK::B;
		return;
	}
}
