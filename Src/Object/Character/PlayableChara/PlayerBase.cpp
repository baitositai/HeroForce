#include"../PlayerInput.h"
#include"../PlayerDodge.h"
#include"../../../Manager/Generic/InputManager.h"
#include"../../../Manager/Generic/SceneManager.h"
#include"../../../Manager/Generic/ResourceManager.h"
#include "PlayerBase.h"

PlayerBase::PlayerBase(void)
{
	skillNo_ = ATK_ACT::SKILL1;

	moveSpeed_ = 0.0f;

	userOnePos_ = { -400.0f,0.0f,0.0f };

	atk_.isHit_ = false;

	multiHitInterval_ = 0.0f;

	atkStartCnt_ = 0.0f;

	isPush_ = false;

	dodge_ = nullptr;

	moveAble_ = true;

	stickDeg_ = -1;

	isSerchArcher_ = false;

	isBuffing_ = false;

	bufAtk_ = atkPow_;
	bufDef_ = defDef_;
	bufSpd_ = defSpeed_;

	speed_ = 0.0f;

	initPos_[0] = PLAYER1_POS;
	initPos_[1] = PLAYER2_POS;
	initPos_[2] = PLAYER3_POS;
	initPos_[3] = PLAYER4_POS;

	for (int i = 0; i < static_cast<int>(ATK_ACT::MAX); i++)
	{
		coolTime_[i] = coolTimeMax_[i];
		coolTimePer_[i] = 1.0f;
	}
}

void PlayerBase::Destroy(void)
{
	delete dodge_;
	dodge_ = nullptr;

	EffectManager::GetInstance().Stop(EffectManager::EFFECT::GUARD);
	EffectManager::GetInstance().Stop(EffectManager::EFFECT::CHARGE_SKILL);
	EffectManager::GetInstance().Stop(EffectManager::EFFECT::CHARGE_AXE_HIT);
	EffectManager::GetInstance().Stop(EffectManager::EFFECT::ARCHER_SKILL2);
	
	SoundManager::GetInstance().Stop(SoundManager::SOUND::SKILL_CHANGE);
}



void PlayerBase::Init(void)
{
	//�A�j���[�V����No������
	InitAnimNum();


	InitCharaAnim();

	SetParam();

	InitAct();

#ifdef DEBUG_ON
	InitDebug();
#endif // DEBUG_ON
	//�֐��|�C���^������
	changeAct_.emplace(ATK_ACT::ATK, std::bind(&PlayerBase::ChangeNmlAtk, this));
	changeAct_.emplace(ATK_ACT::SKILL1, std::bind(&PlayerBase::ChangeSkillOne, this));
	changeAct_.emplace(ATK_ACT::SKILL2, std::bind(&PlayerBase::ChangeSkillTwo, this));

	radius_ = MY_COL_RADIUS;

	skillNo_ = ATK_ACT::SKILL1;

	

	//dodgeCdt_ = DODGE_CDT_MAX;
	dodge_ = new PlayerDodge();
	dodge_->Init();

	//�U���̏�����(�Ƃ肠�����ʏ�U���ŏ��������Ă���)
	ChangeAct(ATK_ACT::ATK);

	ChangeSkillControll(ATK_ACT::SKILL1);

	speed_ = 0.0f;

	userOnePos_ = { -400.0f,0.0f,0.0f };


	atk_.isHit_ = false;

	multiHitInterval_ = 0.0f;

	atkStartCnt_ = 0.0f;

	isPush_ = false;

	hp_ = hpMax_;

	preAtkPow_ = atkPow_;
	preDef_ = def_;
	preSpd_ = moveSpeed_;

	//�o�t���Z
	buffpers_.emplace(STATUSBUFF_TYPE::ATK_BUFF, 0.0f);
	buffpers_.emplace(STATUSBUFF_TYPE::DEF_BUFF, 0.0f);
	buffpers_.emplace(STATUSBUFF_TYPE::SPD_BUFF, 0.0f);

	//�e�o�t�̏�����
	buffs_.emplace(SKILL_BUFF::BUFF_ARROW, (0.0f, 0.0f, 0.0f));
	buffs_.emplace(SKILL_BUFF::GUARD, (0.0f, 0.0f, 0.0f));
	
	auto& snd = SoundManager::GetInstance();
	auto& res = ResourceManager::GetInstance();

	snd.Add(SoundManager::TYPE::SE, SoundManager::SOUND::SKILL_CHANGE
		, res.Load(ResourceManager::SRC::SKILL_CHANGE).handleId_);

	//�v���C���[���͂̃C���X�^���X�����
	PlayerInput::CreateInstance();

	//���f���̏�����
	trans_.Update();

	for (int i = 0; i < static_cast<int>(ATK_ACT::MAX); i++)
	{
		coolTime_[i] = coolTimeMax_[i];
	}

	for (int i = 0; i < static_cast<int>(STATUSBUFF_TYPE::MAX); i++)
	{
		buffPercent_[i] = 1.0f;
	}
}

void PlayerBase::Update(void)
{
	//�A�j���[�V�����̍X�V
	Anim();
	if (!IsAlive())
	{
		ResetAnim(ANIM::DEATH, SPEED_ANIM);
		if (stepAnim_ >= DEATH_STEP_ANIM) { stepAnim_ = DEATH_STEP_ANIM; }
		return;
	}
#ifdef INPUT_DEBUG_ON
	InputUpdate();
#endif // INPUT_DEBUG_ON


	//���W�̃o�b�N�A�b�v
	prePos_ = trans_.pos;

	//���f���̍X�V
	trans_.Update();

	SyncActPos(atk_);

	colPos_ = VAdd(trans_.pos, VScale(PLAYER_COL_LOCAL_POS, CHARACTER_SCALE));

	UserUpdate();

	//�N�[���^�C�������̌v�Z
	CoolTimePerCalc();

	//HP�����炷����
	SubHp();

	BuffUpdate();


#ifdef DEBUG_ON


#endif // DEBUG_ON
	//�X�L�����I�������N�[���^�C���̃J�E���g�J�n
	CoolTimeCnt();
}

void PlayerBase::Draw(void)
{
	MV1DrawModel(trans_.modelId);
#ifdef DEBUG_ON
	DrawDebug();
#endif // DEBUG_ON
}



void PlayerBase::Move(float _deg, VECTOR _axis)
{
	if (!isAtk_&&!isSkill_)
	{
		ResetAnim(ANIM::WALK, SPEED_ANIM_RUN);
	}
	if (!dodge_->IsDodge() && moveAble_)
	{
		speed_ = moveSpeed_;
		Turn(_deg, _axis);
		VECTOR dir = trans_.GetForward();
		//�ړ�����
		VECTOR movePow = VScale(dir, speed_);
		//�ړ�����
		trans_.pos = VAdd(trans_.pos, movePow);
	}
}

void PlayerBase::UserUpdate(void)
{

	//��~�A�j���[�V�����ɂȂ����
	if (!IsMove() && !dodge_->IsDodge() && 0.0f >= atkStartCnt_ &&!isAtk_&&!isSkill_)
	{
		ResetAnim(ANIM::IDLE, SPEED_ANIM_IDLE);
		speed_ = 0.0f;
	}

	//����֌W
	ProcessAct();

	auto& inpMng = InputManager::GetInstance();
	auto& pInp = PlayerInput::GetInstance();

	//���
	dodge_->Update(trans_);
	if (dodge_->IsDodge() && !dodge_->IsCoolDodge()) {
		atk_.ResetCnt();
		atkStartCnt_ = 0.0f;
		isAtk_ = false;
		isSkill_ = false;
		moveAble_ = true;
	}
}


void PlayerBase::InitAnimNum(void)
{
	animNum_.emplace(ANIM::NONE, T_POSE_NUM);
	animNum_.emplace(ANIM::IDLE, IDLE_NUM);
	animNum_.emplace(ANIM::WALK, RUN_NUM);
	animNum_.emplace(ANIM::DODGE, DODGE_NUM);
	animNum_.emplace(ANIM::DAMAGE, DAMAGE_NUM);
	animNum_.emplace(ANIM::DEATH, DEATH_NUM);
}

void PlayerBase::InitCharaAnim(void)
{

}

void PlayerBase::InitAct(void)
{

}

void PlayerBase::ChangeAct(const ATK_ACT _act)
{
	//�N�[���^�C�����Ȃ珈�����Ȃ�
	if (isCool_[static_cast<int>(_act)] && !IsAtkable())return;
	act_ = _act;


	//�ύX�_
	changeAct_[_act]();
}

void PlayerBase::ChangeNmlAtk(void)
{
	ResetAnim(ANIM::UNIQUE_1, SPEED_ANIM_ATK);
	//actUpdate_ = std::bind(&PlayerBase::AtkFunc, this);
}

void PlayerBase::ChangeSkillOne(void)
{
	ResetAnim(ANIM::SKILL_1, SPEED_ANIM_ATK);
	isCool_[static_cast<int>(SKILL_NUM::TWO)] = true;
	//actUpdate_ = std::bind(&PlayerBase::Skill1Func, this);
}

void PlayerBase::ChangeSkillTwo(void)
{
	ResetAnim(ANIM::SKILL_2, SPEED_ANIM_ATK);
	//actUpdate_ = std::bind(&PlayerBase::Skill2Func, this);
}

void PlayerBase::ResetParam(ATK& _atk)
{
	_atk = atkMax_[act_];
}

void PlayerBase::ResetParam(void)
{
	atk_ = atkMax_[act_];
}



#ifdef DEBUG_ON
void PlayerBase::DrawDebug(void)
{
	const unsigned int ATK_COLOR = 0xff0000;
	//����
	DrawSphere3D(trans_.pos, 20.0f, 8, 0x0, 0xff0000, true);
	//�l����p
	DrawFormatString(0, 32, 0xffffff
		, "FrameATK(%f)\nisAtk(%d)\nisBackSrash(%d)\nDodge(%f)\nSkill(%f)\natkStartTime(%f)\natkStartCnt(%f)\nskillType(%d)"
		, atk_.cnt_, atk_.IsAttack(), atk_.IsBacklash()
		, dodge_->GetDodgeCnt(), atk_.cnt_, atkStartTime_[static_cast<int>(SKILL_NUM::ONE)], atkStartCnt_, atkType_);


	//DrawFormatString(0, 200, 0xffffff
	//	, "AtkCooltime(%.2f)\nSkill1Cool(%.2f)\nSkill2Cool(%.2f)\natkDulation(%f)\natkCnt(%f)"
	//	, coolTimePer_[static_cast<int>(ATK_ACT::ATK)]
	//	, coolTimePer_[static_cast<int>(ATK_ACT::SKILL1)]
	//	, coolTimePer_[static_cast<int>(ATK_ACT::SKILL2)]
	//	, atk_.duration_
	//	, atkStartCnt_);

	//DrawFormatString(0, 32, 0xffffff, "atkPos(%f,%f,%f)", atk_.pos_.x, atk_.pos_.y, atk_.pos_.z);
	DrawSphere3D(colPos_, CHARACTER_SCALE * 100, 8, color_Col_, color_Col_, false);
	DrawSphere3D(atk_.pos_, atk_.radius_, 8, color_Atk_, color_Atk_, false);

	DrawFormatString(0, 400, 0xffffff, "actCntl(%d)", PlayerInput::GetInstance().GetAct());

	//�v���C���[�̓����蔻��
	//DrawSphere3D(colPos_, radius_, 4, 0xffff00, 0xffff00, false);
	//�v���C���[�̍��G����
	//DrawSphere3D(trans_.pos, searchRange_, 2, isMove_ ? 0xff0000 : 0xffffff, isMove_ ? 0xff0000 : 0xffffff, false);
	//�v���C���[�̍��G����
	//DrawSphere3D(userOnePos_, 20, 2, 0x0000ff, 0xffffff, false);

	//�U����Ԃ̎��A���̂̐F�ύX
	if (atk_.IsAttack()) { color_Atk_ = ATK_COLOR; }
	else { color_Atk_ = 0x00ffff; }

}
#endif // DEBUG_ON

VECTOR PlayerBase::GetTargetVec(VECTOR _targetPos,bool _isMove)
{
	//�W�I�ւ̕����x�N�g�����擾						��TODO:�x�N�g����SceneGame������炤
	VECTOR targetVec = VSub(_targetPos, trans_.pos);

	//���K��
	targetVec = VNorm(targetVec);

	//Y���W�͕K�v�Ȃ��̂ŗv�f������
	targetVec = { targetVec.x,0.0f,targetVec.z };

	//�ړ��ʂ����߂�
	VECTOR ret = VScale(targetVec, moveSpeed_);

	if (_isMove) { return ret; }
	else { return targetVec; }
}




void PlayerBase::Turn(float _deg, VECTOR _axis)
{
	//transform_.quaRot =
	//	transform_.quaRot.Mult(Quaternion::AngleAxis(AsoUtility::Deg2RadF(deg), axis));

	trans_.quaRot =
		trans_.quaRot.AngleAxis(AsoUtility::Deg2RadF(_deg), _axis);
}



void PlayerBase::InitAtk(void)
{
	//�U���J�E���g������
	atk_.ResetCnt();

	//�X�L�����I�������N�[���^�C���̃J�E���g�J�n
	isCool_[static_cast<int>(act_)] = true;

	//�ړ��\�ɂ���
	moveAble_ = true;

	//��������
	atk_.isHit_ = false;

	//�U���̔���
	atkStartCnt_ = 0.0f;


	EffectManager::GetInstance().Stop(EffectManager::EFFECT::GUARD);



	
}

void PlayerBase::SetBuff(STATUSBUFF_TYPE _type, SKILL_BUFF _skill, float _per, float _second)
{
	buffs_[_skill].cnt = _second;
	buffs_[_skill].percent[static_cast<int>(_type)] = _per;
}

void PlayerBase::SetPreStatus(void)
{
	preAtkPow_ = atkPow_;
	preDef_ = def_;
	preSpd_ = moveSpeed_;
}

void PlayerBase::BuffUpdate(void)
{
	for (auto& buff : buffs_)
	{
		CntDown(buff.second.cnt);
	
		if (buff.second.cnt>0.0f&&buff.second.isBuff)
		{
			for (int i = 0; i < static_cast<int>(SKILL_BUFF::MAX); i++)
			{
				if (buff.second.isBuffing == true)continue;

				//���ꂼ��̃o�t(�X�L�����Ƃɐݒ肳�ꂽ�o�t)���X�e�[�^�X�̃o�t�ɑ���
				buffpers_[STATUSBUFF_TYPE::ATK_BUFF] += buff.second.percent[static_cast<int>(STATUSBUFF_TYPE::ATK_BUFF)]/ DEFAULT_PERCENT;
				buffpers_[STATUSBUFF_TYPE::DEF_BUFF] += buff.second.percent[static_cast<int>(STATUSBUFF_TYPE::DEF_BUFF)]/ DEFAULT_PERCENT;
				buffpers_[STATUSBUFF_TYPE::SPD_BUFF] += buff.second.percent[static_cast<int>(STATUSBUFF_TYPE::SPD_BUFF)]/ DEFAULT_PERCENT;
				buff.second.isBuffing = true;
			}
		}
		else if(buff.second.cnt <= 0.0f)
		{
			buff.second.cnt = 0.0f;
			for (int i = 0; i < static_cast<int>(SKILL_BUFF::MAX); i++)
			{
				if (buff.second.isBuffing == false)continue;

				//�������Ԃ��I�������X�L���ő��₵�������ʕ�����
				buffpers_[STATUSBUFF_TYPE::ATK_BUFF] -= buff.second.percent[static_cast<int>(STATUSBUFF_TYPE::ATK_BUFF)] / DEFAULT_PERCENT;
				buffpers_[STATUSBUFF_TYPE::DEF_BUFF] -= buff.second.percent[static_cast<int>(STATUSBUFF_TYPE::DEF_BUFF)] / DEFAULT_PERCENT;
				buffpers_[STATUSBUFF_TYPE::SPD_BUFF] -= buff.second.percent[static_cast<int>(STATUSBUFF_TYPE::SPD_BUFF)] / DEFAULT_PERCENT;
				buff.second.isBuffing = false;
				buff.second.isBuff = false;
			}
		}
		atkPow_ = preAtkPow_ * (1.0f + buffpers_[STATUSBUFF_TYPE::ATK_BUFF]);
		def_ = preDef_ * (1.0f + buffpers_[STATUSBUFF_TYPE::DEF_BUFF]);
		moveSpeed_ = preSpd_ * (1.0f + buffpers_[STATUSBUFF_TYPE::SPD_BUFF]);
	}
}

void PlayerBase::Reset(void)
{
	//�A�j���[�V����������
	ResetAnim(ANIM::IDLE, SPEED_ANIM);

	skillNo_ = ATK_ACT::SKILL1;

	dodge_->Init();

	userOnePos_ = { -400.0f,0.0f,0.0f };

	ResetParam();

	hp_ = hpMax_;

	isSkill_ = false;
	isAtk_ = false;

	for (int i = 0; i < static_cast<int>(ATK_ACT::MAX); i++)
	{
		coolTime_[i] = coolTimeMax_[i];
	}

	////���f���̏�����
	trans_.Update();

	auto& effIns = EffectManager::GetInstance();
	effIns.Stop(EffectManager::EFFECT::GUARD);
	effIns.Stop(EffectManager::EFFECT::CHARGE_AXE_HIT);

	//�ύX�ӏ�
	//---------------------------------------------
	effIns.Stop(EffectManager::EFFECT::CHARGE_AXE_HIT);
	effIns.Stop(EffectManager::EFFECT::ARCHER_SKILL2);

	SoundManager::GetInstance().Stop(SoundManager::SOUND::SKILL_CHANGE);
	//---------------------------------------------

}


void PlayerBase::BuffPerAdd(STATUSBUFF_TYPE _type, float _per)
{
	buffPercent_[static_cast<int>(_type)] += _per;
}

void PlayerBase::SyncActPos(ATK& _atk)
{
	//�Ǐ]�Ώۂ̈ʒu
	VECTOR followPos = trans_.pos;

	//�Ǐ]�Ώۂ̌���
	Quaternion followRot = trans_.quaRot;

	//�Ǐ]�Ώۂ̊p�x
	VECTOR relativeActPos = atkMax_[act_].pos_;

	VECTOR addPos = followRot.PosAxis(VScale(relativeActPos, CHARACTER_SCALE));

	_atk.pos_ = VAdd(trans_.pos, addPos);
}

void PlayerBase::ChangeSkillControll(ATK_ACT _skill)
{
	atkType_ = atkTypes_[static_cast<int>(_skill)];
	isPush_ = false;
	moveAble_ = true;

}
const bool PlayerBase::IsAtkable(void) const
{
	 return!IsAtkAction() && !dodge_->IsDodge();
}
const bool PlayerBase::IsDodgeable(void) const
{
	 return !dodge_->IsDodge() && !IsAtkAction() && !dodge_->IsCoolDodge(); 
}
void PlayerBase::Damage(void)
{
	//�Ƃ肠����1�_���[�W���炷
	hp_--;

	//�A�j���[�V�����ύX
	ResetAnim(ANIM::DAMAGE, SPEED_ANIM_DODGE);
#ifdef DEBUG_ON
	const unsigned int  DMG_COLOR = 0x00ffff;
	color_Col_ = DMG_COLOR;
#endif // DEBUG_ON
	if (!IsAlive()) { ResetAnim(ANIM::DEATH, SPEED_ANIM); }

}

#ifdef DEBUG_ON
void PlayerBase::InitDebug(void)
{
	color_Col_ = 0xffffff;
	color_Atk_ = 0x00ffff;
	color_skl1_ = 0x00ffff;
	color_skl2_ = 0x00ffff;


}
#endif // DEBUG_ON


void PlayerBase::CoolTimeCnt(void)
{
	if (act_ == ATK_ACT::MAX)return;
	for (int i = 0; i < static_cast<int>(ATK_ACT::MAX); i++)
	{
		if (isCool_[i])
		{
			CntUp(coolTime_[i]);
		}
		if (coolTime_[i] >= coolTimeMax_[i])
		{
			isCool_[i] = false;
			coolTime_[i] = coolTimeMax_[i];
		}
		else if (coolTime_[i] <= 0.0f)
		{
			coolTime_[i] = 0.0f;
		}
	}
}


void PlayerBase::ProcessAct(void)
{
	AtkFunc();
	switch (skillNo_)
	{
	case PlayerBase::ATK_ACT::SKILL1:
		Skill1Func();
		break;
	case PlayerBase::ATK_ACT::SKILL2:
		Skill2Func();
		break;
	default:
		break;
	}

}

void PlayerBase::CoolTimePerCalc(void)
{
	for (int i = 0; i < static_cast<int>(ATK_ACT::MAX); i++)
	{
		coolTimePer_[i] = coolTime_[i] / coolTimeMax_[i];
	}
}

bool PlayerBase::IsSkillable(void)
{
	{ return !IsAtkAction() && !dodge_->IsDodge(); }
}

void PlayerBase::SkillChange(void)
{
	skillNo_ = static_cast<ATK_ACT>(static_cast<int>(skillNo_) + 1);
	auto& snd = SoundManager::GetInstance();
	snd.Play(SoundManager::SOUND::SKILL_CHANGE);
	snd.AdjustVolume(SoundManager::SOUND::SKILL_CHANGE, SKILL_CHANGE_SE_VOL);
	//MAX�ɂȂ�����X�L���P�ɖ߂�
	skillNo_ == ATK_ACT::MAX ? skillNo_ = ATK_ACT::SKILL1 : skillNo_ = skillNo_;
	//�ύX�_
	ChangeSkillControll(skillNo_);
}