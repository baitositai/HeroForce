#include "ClearChicken.h"

ClearChicken::ClearChicken()
{
}

void ClearChicken::SetParam()
{
	hp_ = LIFE;

	//�������̍s���������_���Ō��߂�
	aliveState_ = ALIVE_MOVE::CALL;

	//�������(���������̏�Ԃ����߂Ă���s��)
	ChangeState(STATE::ALIVE);

	//�A�j���[�V�������
	ResetAnim(ANIM::IDLE, DEFAULT_SPEED_ANIM);
}