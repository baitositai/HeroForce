#include <DxLib.h>
#include "../../Utility/AsoUtility.h"
#include "Cursor.h"

Cursor::Cursor()
{
	pad_ = InputManager::JOYPAD_NO::PAD1;
	key_.right_ = -1;
	key_.left_ = -1;
	key_.up_ = -1;
	key_.down_ = -1;
	key_.decide_ = -1;

	pos_ = { -1,-1 };
	playerNum_ = -1;
	decide_ = false;
	rate_ = -1.0f;
	img_ = -1;
}

void Cursor::Init(const int _plNum,const int _img)
{
	//�v���C���[�i���o�[
	playerNum_ = _plNum;

	//�摜
	img_ = _img;

	//�p�b�h���
	pad_ = static_cast<InputManager::JOYPAD_NO>(playerNum_ + 1);

	//�����ݒ�
	Reset();
}

void Cursor::Update()
{
	//����ς݂̏ꍇ
	if (decide_) { return; }

	auto& ins = InputManager::GetInstance();

	//�X�e�B�b�N
	int stickX = ins.GetJPadInputState(pad_).AKeyLX;
	int stickY = ins.GetJPadInputState(pad_).AKeyLY;

	//���쏈��
	if (ins.IsNew(key_.right_) || stickX > 0) 
	{ 
		pos_.x += MOVE_POW; 
		if (pos_.x > Application::SCREEN_SIZE_X)
		{
			pos_.x = Application::SCREEN_SIZE_X;
		}
	}
	if (ins.IsNew(key_.left_) || stickX < 0)	
	{
		pos_.x -= MOVE_POW; 
		if (pos_.x < 0)
		{
			pos_.x = 0;
		}
	}
	if (ins.IsNew(key_.up_) || stickY < 0)	
	{ 
		pos_.y -= MOVE_POW; 
		if (pos_.y < 0)
		{
			pos_.y = 0;
		}
	}
	if (ins.IsNew(key_.down_) || stickY > 0)	
	{ 
		pos_.y += MOVE_POW;
		if (pos_.y > Application::SCREEN_SIZE_Y)
		{
			pos_.y = Application::SCREEN_SIZE_Y;
		}
	}

	//����
	if (ins.IsTrgDown(key_.decide_) ||
		ins.IsPadBtnTrgDown(pad_, InputManager::JOYPAD_BTN::RIGHT))
	{
		decide_ = true;
	}
}

void Cursor::Draw()
{
	//�p�x
	float angle = AsoUtility::Deg2RadF(ANGLE);

	//����ς݂̏ꍇ�͕`�悵�Ȃ�
	if (decide_) { return; }

	//�A�C�R���`��
	DrawRotaGraph(
		pos_.x,
		pos_.y,
		rate_,
		angle,
		img_,
		true,
		false);
}

void Cursor::Reset()
{
	//����
	decide_ = false;

	//���W
	pos_ = { DEFAULT_POS_X * playerNum_,DEFAULT_POS_Y};

	//�g�嗦
	rate_ = SCALE_RATE;
}

void Cursor::SetControllKey(const int _right, const int _left, const int _up, const int _down, const int _decide)
{
	key_.right_ = _right;
	key_.left_ = _left;
	key_.up_ = _up;
	key_.down_ = _down;
	key_.decide_ = _decide;
}

void Cursor::SetDecide(const bool _value)
{
	decide_ = _value;
}
