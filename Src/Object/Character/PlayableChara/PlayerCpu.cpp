//#include"../Manager/Collision.h"
#include"../PlayerDodge.h"
#include "PlayerCpu.h"

PlayerCpu::PlayerCpu(void)
{
	
}
void PlayerCpu::SetParam(void)
{
	cpuStateChanges_.emplace(CPU_STATE::NORMAL, std::bind(&PlayerCpu::CpuChangeNml, this));
	cpuStateChanges_.emplace(CPU_STATE::ATTACK, std::bind(&PlayerCpu::CpuChangeAtk, this));
	cpuStateChanges_.emplace(CPU_STATE::BREAK, std::bind(&PlayerCpu::CpuChangeBreak, this));
}

void PlayerCpu::Update(void)
{
	PlayerBase::Update();
	//�e��Ԃ̍X�V
	CpuStateUpdate();
	auto& ins = InputManager::GetInstance();
	//CPU�������I�ɌĂяo��
	if (ins.IsTrgDown(KEY_INPUT_X))
	{
		isCall_ = true;
	}
	if (!isMove2CallPlayer_			//�v���C���[�Ɍ������ē����ĂȂ��Ƃ�		
		&& isCall_					//�Ă񂾎�
		&& !AsoUtility::IsHitSpheres(//�v���C���[�Ƃ̊Ԋu�O�ɂ���Ƃ�
			trans_.pos				 
			, SEARCH_RANGE
			, userOnePos_
			, MY_COL_RADIUS)
		&& cpuState_ == CPU_STATE::BREAK)//
	{
		isMove2CallPlayer_ = true;
		ChangeState(CPU_STATE::NORMAL);

		//�O�̍U���̏�����
		preAtk_ = ATK_ACT::MAX;

		//�Ăяo����Ĉړ�����Ƃ��̉����x
		calledMoveSpeed_ = CALLED_MOVE_SPEED_SCALE;

		//�v���C���[�ɗD��I�ɂ��Ă�����������G���@�m���Ȃ��悤�ɂ���
		isEnemySerch_ = false;

		//�^�[�Q�b�g���Ăяo���ꂽ�v���C���[�ɐݒ肷��
		//�^�[�Q�b�g���Ăяo���ꂽ�v���C���[�ɐݒ肷��
		SetTargetPos(userOnePos_);
	}



#ifdef DEBUG_ON
	{
		auto& ins = InputManager::GetInstance();
		const float SPEED = 5.0f;
		if (ins.IsNew(KEY_INPUT_W)) { userOnePos_.z += SPEED; }
		if (ins.IsNew(KEY_INPUT_D)) { userOnePos_.x += SPEED; }
		if (ins.IsNew(KEY_INPUT_S)) { userOnePos_.z -= SPEED; }
		if (ins.IsNew(KEY_INPUT_A)) { userOnePos_.x -= SPEED; }



		// ���X�e�B�b�N�̉���
		leftStickX_ = ins.GetJPadInputState(InputManager::JOYPAD_NO::PAD1).AKeyLX;

		//�c��
		leftStickY_ = ins.GetJPadInputState(InputManager::JOYPAD_NO::PAD1).AKeyLY;
		auto stickRad = static_cast<float>(atan2(static_cast<double>(leftStickY_), static_cast<double>(leftStickX_)));
		stickDeg_ = static_cast<float>(AsoUtility::DegIn360(AsoUtility::Rad2DegF(stickRad) + 90.0f));
		//�O
		if (leftStickY_ < -1) { userOnePos_.z += SPEED; }
		//�E
		else if (leftStickX_ > 1) { userOnePos_.x += SPEED; }
		//��
		else if (leftStickY_ > 1) { userOnePos_.z -= SPEED; }
		//��
		else if (leftStickX_ < -1) { userOnePos_.x -= SPEED; }

	}

#endif // DEBUG_ON

}

void PlayerCpu::CpuStateUpdate(void)
{
	cpuStateUpdate_();
}


void PlayerCpu::CpuMove(VECTOR _targetPos)
{
	//�ړ����x�̍X�V
	speed_ = SPEED_MOVE * calledMoveSpeed_;
	isMove_ = true;

	//�����x�N�g���擾
	VECTOR targetVec = GetTargetVec(_targetPos);

	//��]
	trans_.quaRot = trans_.quaRot.LookRotation(targetVec);

	//�ړ�
	trans_.pos = VAdd(trans_.pos, targetVec);
}

void PlayerCpu::ChangeState(CPU_STATE _state)
{
	cpuState_ = _state;
	cpuStateChanges_[_state]();
}

void PlayerCpu::CpuReset(void)
{
	searchRange_ = SEARCH_RANGE;

	preAtk_ = ATK_ACT::MAX;

	isEnemySerch_ = false;

	isMove2CallPlayer_ = false;
}

//�������Ƃ�CPU���������̂Ŏc���Ă���
void PlayerCpu::CpuActUpdate(ATK_ACT _act)
{

}

void PlayerCpu::CpuChangeNml(void)
{
	cpuStateUpdate_ = std::bind(&PlayerCpu::CpuNmlUpdate, this);
}

void PlayerCpu::CpuChangeAtk(void)
{
	isCall_ = false;
	cpuStateUpdate_ = std::bind(&PlayerCpu::CpuAtkUpdate, this);
}

void PlayerCpu::CpuChangeBreak(void)
{
	breakCnt_ = 0.0f;
	cpuStateUpdate_ = std::bind(&PlayerCpu::CpuBreakUpdate, this);
}

void PlayerCpu::CpuNmlUpdate(void)
{
	//auto& col = Collision::GetInstance();
	const float MOVE_DELAY_MAX = 0.3f;

	//�ҋ@�A�j���[�V����
	//��~��Ԃ̎��̃A�j���[�V����
	if (!isMove_ && !dodge_->IsDodge() && !IsAtkAction())
	{
		ResetAnim(ANIM::IDLE, SPEED_ANIM_IDLE);
		speed_ = 0.0f;
	}
	//�����A�j���[�V����
	else
	{
		ResetAnim(ANIM::WALK, SPEED_ANIM_RUN);
	}

	//�ړ��ʂ̏�����
	speed_ = 0.0f;

	//preAtk_ = ATK_ACT::MAX;

	if (!isEnemySerch_)
	{
		if (AsoUtility::IsHitSpheres(trans_.pos, FOLLOW_PLAYER_RADIUS, userOnePos_, FOLLOW_PLAYER_RADIUS))
		{
			//�Ăяo�����I������炻�ꂼ��̏�����
			isMove_ = false;
			isMove2CallPlayer_ = false;
			calledMoveSpeed_ = 1.0f;
			isCall_ = false;
			moveStartDelay_ = 0.0f;
		}
		else
		{
			//�v���C���[�̂Ƃ���܂œ���
			targetPos_ = userOnePos_;
			CntUp(moveStartDelay_);
			if (moveStartDelay_ >= MOVE_DELAY_MAX)
			{
				moveStartDelay_ = MOVE_DELAY_MAX;
				CpuMove(targetPos_);
			}
		}
	}
	else
	{
		//�G���@�m������A�Q�[���V�[������SetTargetPos�̒��ɓG�̍��W������
		//����Ɍ����ē�����
		CpuMove(targetPos_);
	}


}

void PlayerCpu::CpuAtkUpdate(void)
{
	//�U�����I����Ă���Ȃ��ԑJ��
	if (atk_.IsFinishMotion())
	{
		//�O�̍U���̏�����
		preAtk_ = ATK_ACT::MAX;

		//�x�e��ԂɑJ��
		ChangeState(CPU_STATE::BREAK);
		return;
	}
	//�O�̍U���������Ȃ�������A�U����ނ����߂�
	if (preAtk_ == ATK_ACT::MAX)
	{
		for (int i = ATK_TOTAL - 1; i > -1; i--)
		{
			if (!isCool_[i])
			{
				ChangeAct(static_cast<ATK_ACT>(i));
				preAtk_ = static_cast<ATK_ACT>(i);
				return;
			}
			else
			{
				continue;
			}
		}
	}
}

void PlayerCpu::CpuBreakUpdate(void)
{
	const float BREAK_TIME = 2.0f;
	ResetAnim(ANIM::IDLE, SPEED_ANIM_IDLE);
	if (breakCnt_ > BREAK_TIME)
	{
		//�u���C�N���I�������ʏ��Ԃֈڍs
		ChangeState(CPU_STATE::NORMAL);
		return;
	}
	CntUp(breakCnt_);
}