#include "./USER/PlAxeMan.h"
#include "AxeMan.h"
#ifdef DEBUG_ON
void AxeMan::DrawDebug(void)
{
	DrawFormatString(0, 0, 0x000000, "atkPos(%f)", atkPow_);
}

#endif // DEBUG_ON


AxeMan::AxeMan(void)
{
	
}

void AxeMan::SetParam(void)
{
	InitAct();

	//�U���̏�����
	InitAtk();

	trans_.SetModel(
		ResourceManager::GetInstance()
		.LoadModelDuplicate(ResourceManager::SRC::PLAYER_AXEMAN));
	float scale = CHARACTER_SCALE;
	trans_.scl = { scale, scale, scale };
	trans_.pos = { -300.0f, 0.0f, 0.0f };
	trans_.quaRot = Quaternion();
	trans_.quaRotLocal = Quaternion::Euler(
		0.0f, AsoUtility::Deg2RadF(180.0f),
		0.0f
	);
	auto& effIns = EffectManager::GetInstance();
	auto& resIns = ResourceManager::GetInstance();
	using EFFECT = EffectManager::EFFECT;

	//���ߍU���G�t�F�N�g���[�h
	EffectManager::GetInstance().Add(EFFECT::CHARGE_AXE_HIT, 
		ResourceManager::GetInstance().Load(ResourceManager::SRC::CHARGE_AXE_HIT).handleId_);

	//�X�L���`���[�W(���[�h����ƂȂ����G�t�F�N�g���o�Ȃ��Ȃ邩��O�̂��߃R�����g�A�E�g)
	EffectManager::GetInstance().Add(EFFECT::CHARGE_SKILL,
		ResourceManager::GetInstance().Load(ResourceManager::SRC::CHARGE_SKILL).handleId_);


	ResetAnim(ANIM::IDLE, SPEED_ANIM_IDLE);

	//�X�e�[�^�X�֌W
	ParamLoad(CharacterParamData::UNIT_TYPE::AXEMAN);

	//��E�̊��蓖��
	role_ = SceneManager::ROLE::AXEMAN;

	

	moveAble_ = true;

	//�����蔻��̐ݒ�
	radius_ = MY_COL_RADIUS;
	//acts_[ATK_ACT::ATK].radius_ = COL_ATK;

}
void AxeMan::InitAct(void)
{
	//�ʏ�U���̍ő�l
	atkMax_.emplace(ATK_ACT::ATK, ATK_MAX);

	//�X�L���P�̍ő�l
	atkMax_.emplace(ATK_ACT::SKILL1, SKILL_ONE_MAX);

	//�X�L���Q�̍ő�l
	atkMax_.emplace(ATK_ACT::SKILL2, SKILL_TWO_MAX);


	//�N�[���^�C��
	coolTimeMax_[static_cast<int>(ATK_ACT::ATK)] = ATK_COOLTIME;
	coolTimeMax_[static_cast<int>(ATK_ACT::SKILL1)] = SKILL_ONE_COOLTIME;
	coolTimeMax_[static_cast<int>(ATK_ACT::SKILL2)] = SKILL_TWO_COOLTIME;

	//�U����������
	atkStartTime_[static_cast<int>(ATK_ACT::ATK)] = ATK_START;
	atkStartTime_[static_cast<int>(ATK_ACT::SKILL1)] = SKILL_ONE_START;
	atkStartTime_[static_cast<int>(ATK_ACT::SKILL2)] = SKILL_TWO_START;


}

void AxeMan::InitSkill(ATK_ACT _act)
{
	if (isAtk_ || isSkill_)return;
	//�X�L�����ƂɃA�j���[�V���������߂āA�J�E���g�J�n
	ChangeAct(static_cast<ATK_ACT>(_act));
	ResetParam(atk_);
	CntUp(atkStartCnt_);
	moveAble_ = false;
	isSkill_ = true;
}

void AxeMan::Draw(void)
{
	PlayerBase::Draw();
#ifdef DEBUG_ON
	DrawDebug();
#endif // DEBUG_ON

	
}

void AxeMan::InitCharaAnim(void)
{
	animNum_.emplace(ANIM::UNIQUE_1, ATK_NUM);
	animNum_.emplace(ANIM::UNIQUE_2, SPIN_NUM);
	animNum_.emplace(ANIM::SKILL_1, SKILL_ONE_NUM);
	animNum_.emplace(ANIM::SKILL_2, SKILL_TWO_NUM);
}

void AxeMan::NmlAtkInit(void)
{
}

void AxeMan::SkillOneInit(void)
{

}

void AxeMan::SkillTwoInit(void)
{
	
}

void AxeMan::AtkFunc(void)
{
	if (!isAtk_)return;

	if (IsAtkStart())
	{
		moveAble_ = false;
		CntUp(atkStartCnt_);
		if (IsFinishAtkStart())
		{
			CntUp(atk_.cnt_);
		}
	}
	else if (IsFinishAtkStart())
	{
		if ((atk_.IsAttack() || atk_.IsBacklash()))
		{
			CntUp(atk_.cnt_);
			//�N�[���^�C���̏�����
			coolTime_[static_cast<int>(act_)] = 0.0f;
		}
		if(atk_.IsFinishMotion())/*�������ƒʏ�A�ł̎��Ƀo�O��*/
		{
			InitAtk();
			isAtk_ = false;
		}
	}
}

void AxeMan::Skill1Func(void)
{
	auto& efeIns = EffectManager::GetInstance();
	if (isCool_[static_cast<int>(skillNo_)])
	{
		return;
	}
	if (0.0f < atkStartCnt_ && atkStartCnt_ < atkStartTime_[static_cast<int>(act_)])
	{
		if (atkStartCnt_ <= DELTATIME)
		{
			EffectManager::GetInstance().Play(
				EffectManager::EFFECT::CHARGE_SKILL,
				trans_.pos,
				Quaternion(),
				CHARGE_SKILL_EFF_SIZE,
				SoundManager::SOUND::NONE);
		}

		CntUp(atkStartCnt_);
		if (stepAnim_ >= SKILL_CHARGE_STEPANIM)
		{
			stepAnim_ = SKILL_CHARGE_STEPANIM;
		}
	}
	else if (atkStartCnt_ >= atkStartTime_[static_cast<int>(skillNo_)])
	{
		if (atkStartCnt_ > PlAxe::SKILL_ONE_START_NOCHARGE) { atk_.pow_ = SKILL_ONE_CHARGE_POW; }
		
		CntUp(atk_.cnt_);
		if (atk_.cnt_ <=DELTATIME)
		{
			EffectManager::GetInstance().Play(
				EffectManager::EFFECT::CHARGE_AXE_HIT,
				atk_.pos_,
				Quaternion(),
				CHARGE_AXE_EFF_SIZE,
				SoundManager::SOUND::NONE);
		}
		if (atk_.IsFinishMotion())
		{
			coolTime_[static_cast<int>(ATK_ACT::SKILL1)] = 0.0f;

			//efeIns.Stop(EffectManager::EFFECT::CHARGE_SKILL);

			//�X�L���I�������U���������Ԃ̍ő厞�Ԃ��Z�b�g����
			atkStartTime_[static_cast<int>(ATK_ACT::SKILL1)] = SKILL_ONE_START;

			InitAtk();
			isSkill_ = false;
		}
	}
}

void AxeMan::Skill2Func(void)
{
	if (isAtk_)return;

	if (!isSkill_)return;
	//�ߐڍU���p(atk_�ϐ��Ɖ������ŕ�����)
	if (IsAtkStart())
	{
		moveAble_ = false;
		CntUp(atkStartCnt_);
		if (IsFinishAtkStart())
		{
			CntUp(atk_.cnt_);
		}
	}
	else if (IsFinishAtkStart())
	{
		if ((atk_.IsAttack() || atk_.IsBacklash()))
		{
			CntUp(atk_.cnt_);
			//�N�[���^�C���̏�����
			coolTime_[static_cast<int>(act_)] = 0.0f;
		}
		else //if(atk_.IsFinishMotion())/*�������ƒʏ�A�ł̎��Ƀo�O��*/
		{
			InitAtk();
			isSkill_ = false;
		}
	}


	if (atk_.cnt_ >= SKILL2_CHANGE_ANIM_TIME)
	{
		if (stepAnim_ > 14.0f)ResetAnim(ANIM::UNIQUE_2, SPEED_ANIM_ATK);
		//��]���ړ��ł���
		moveAble_ = true;
		//�U�����W���ړ�������������
		SyncActPos(atk_);
		//������]�؂�
		if (atk_.isHit_)
		{
			CntUp(multiHitInterval_);
			//�N�[���^�C���̏�����
			coolTime_[static_cast<int>(act_)] = 0.0f;
			if (multiHitInterval_ >= MULTIHIT_INTERVAL)
			{
				atk_.isHit_ = false;
				multiHitInterval_ = 0.0f;
			}
		}
		else
		{
			multiHitInterval_ = 0.0f;
		}
	}
}