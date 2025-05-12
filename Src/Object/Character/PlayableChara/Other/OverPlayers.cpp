#include "OverPlayers.h"

OverPlayers::OverPlayers()
{
}

void OverPlayers::InitAnim()
{	
	//�A�j���[�V�����̐ݒ�
	animNum_.emplace(ANIM::IDLE, IDLE_ANIM);
	animNum_.emplace(ANIM::DEATH, DEATH_ANIM);
	ResetAnim(ANIM::DEATH, ANIM_SPEED);
}

void OverPlayers::FinishAnim()
{
	switch (anim_)
	{
	case UnitBase::ANIM::IDLE:
		//���[�v�Đ�
		stepAnim_ = 0.0f;
		break;

	case UnitBase::ANIM::DEATH:
		ResetAnim(ANIM::IDLE, ANIM_SPEED);
		break;
	}
}
