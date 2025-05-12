#include<DxLib.h>
#include<cmath>
#include"../../../Application.h"
#include"../../../Utility/AsoUtility.h"
#include "Enemy.h"

Enemy::Enemy(const VECTOR& _spawnPos)
{
	trans_.pos = _spawnPos;
}

void Enemy::Destroy(void)
{
	animNum_.clear();
	stateChanges_.clear();
	changeSpeedAnim_.clear();
	skills_.clear();
	nowSkill_.clear();
	alertSkills_.clear();
	changeSkill_.clear();
	skillPreAnims_.clear();
	skillAnims_.clear();
	SearchStateInfo_.clear();

	lastAtk_ = nullptr;
	delete lastAtk_;
}

void Enemy::Init(void)
{
	//��ԊǗ�
	stateChanges_.emplace(STATE::NORMAL, std::bind(&Enemy::ChangeStateNormal, this));
	stateChanges_.emplace(STATE::ALERT, std::bind(&Enemy::ChangeStateAlert, this));
	stateChanges_.emplace(STATE::ATTACK, std::bind(&Enemy::ChangeStateAtk, this));
	stateChanges_.emplace(STATE::BREAK, std::bind(&Enemy::ChangeStateBreak, this));

	//�T����ԊǗ�
	SearchStateInfo_.emplace(SEARCH_STATE::CHICKEN_SEARCH, std::bind(&Enemy::SearchMoveInfo, this));
	SearchStateInfo_.emplace(SEARCH_STATE::CHICKEN_FOUND, std::bind(&Enemy::FoundMoveInfo, this));
	SearchStateInfo_.emplace(SEARCH_STATE::PLAYER_FOUND, std::bind(&Enemy::FoundMoveInfo, this));

	//�L�����ŗL�ݒ�
	SetParam();

	//�A�j���[�V�����ԍ��̏�����
	InitAnim();

	//�G�t�F�N�g�̏�����
	InitEffect();

	//���ʂ̕ϐ��̏�����
	trans_.quaRot = Quaternion();
	trans_.quaRotLocal = Quaternion::AngleAxis(AsoUtility::Deg2RadF(180.0f), AsoUtility::AXIS_Y);
	ChangeState(STATE::NORMAL);
	alertCnt_ = 0.0f;
	breakCnt_ = 0.0f;
	atkAct_ = ATK_ACT::MAX;
	isEndAlert_ = false;
	isEndAllAtkSign_ = false;
	isEndAllAtk_ = false;
	isColStage_ = false;
	colStageCnt_ = 0.0f;
	startCnt_ = 0.0f;
	damage_ = 0.0f;
	targetPos_ = preTargetPos_ = AsoUtility::VECTOR_ZERO;
	ChangeSearchState(SEARCH_STATE::CHICKEN_SEARCH);

	//�U�����̏�����
	InitSkill();
	atk_.ResetCnt();

	trans_.Update();

	ui_ = std::make_unique<EnemyHpBar>();
	ui_->Init();
}

void Enemy::Update(void)
{
	//�ړ��O���W
	prePos_ = trans_.pos;

	//�A�j���[�V����
	Anim();

#ifdef DEBUG_ENEMY
	//���͗p
	InputManager& ins = InputManager::GetInstance();

	//if (ins.IsNew(KEY_INPUT_W)) { targetPos_.z+= 3.0f; }
	//if (ins.IsNew(KEY_INPUT_D)) { targetPos_.x+= 3.0f; }
	//if (ins.IsNew(KEY_INPUT_S)) { targetPos_.z-= 3.0f; }
	//if (ins.IsNew(KEY_INPUT_A)) { targetPos_.x-= 3.0f; }

	if (InputManager::GetInstance().IsTrgDown(KEY_INPUT_Q)) { Damage(1, 10); }
#endif // DEBUG_ENEMY

	SubHp();

	//����Ă���Ȃ牽�����Ȃ�
	if (!IsAlive()) 
	{
		//���ꂽ�玀�S�A�j���[�V����
		ResetAnim(ANIM::DEATH, changeSpeedAnim_[ANIM::DEATH]);
		return;
	}

	//�����蔻����W�̍X�V
	colPos_ = VAdd(trans_.pos, localCenterPos_);

	//��Ԃ��Ƃ�Update
	stateUpdate_();


	//���f������
	trans_.Update();
}

bool Enemy::IsFinishAnim(const ANIM _anim)
{
	//�������w�肳��Ă��Ȃ��@���́@�w�肳�ꂽ�A�j���[�V�����ԍ��ƌ��݂̃A�j���[�V����������
	if (_anim == ANIM::NONE || anim_ == _anim)
	{
		if (stepAnim_ > animTotalTime_)
		{
			return true;
		}
	}

	return false;
}

void Enemy::ChangeSearchState(const SEARCH_STATE _searchState)
{
	//��ԑJ��
	searchState_ = _searchState;

	//��Ԃɂ����X�V
	SearchStateInfo_[searchState_]();
}

//void Enemy::Damage(const int _damage, const int _stunPow)
//{
//	//���ɂ���Ă���Ȃ珈�����Ȃ�
//	if (!IsAlive()) 
//	{
//		//���ꂽ�玀�S�A�j���[�V����
//		ResetAnim(ANIM::DEATH, changeSpeedAnim_[ANIM::DEATH]);
//		return;
//	}
//
//	//�_���[�W�J�E���g
//	hp_ -= _damage;
//
//	//�X�^���l�J�E���g
//	//stunDef_ += _stunPow;
//}

void Enemy::ChangeState(const STATE _state)
{
	//��ԑJ��
	state_ = _state;

	// �e��ԑJ�ڂ̏�������
	stateChanges_[state_]();
}

void Enemy::KeepCollStageDistance(void)
{
	//�^�[�Q�b�g�܂ł̃x�N�g���𑪂��āAx,z�̒l���傫�������ɋ������Ƃ�
	VECTOR targetVec = GetMoveVec(trans_.pos, targetPos_);

	if (fabsf(targetVec.x) > fabsf(targetVec.z))
	{
		SetPreTargetPos(VSub(prePos_, { 0.0f,0.0f,targetVec.z * KEEP_COL_STAGE_POS }));
	}
	else
	{
		SetPreTargetPos(VSub(prePos_, { targetVec.x * KEEP_COL_STAGE_POS,0.0f,0.0f }));
	}

	//�X�e�[�W�ɓ�������
	isColStage_ = true;
}

void Enemy::ChangeStateNormal(void)
{
	//�X�V�����̒��g������
	stateUpdate_ = std::bind(&Enemy::UpdateNml, this);
}

void Enemy::ChangeStateAlert(void)
{
	//�X�V�����̒��g������
	stateUpdate_ = std::bind(&Enemy::UpdateAlert, this);

	//�\���͈͂�������
	ResetAlertVertex();

	//���������߂Đݒ�
	trans_.quaRot = trans_.quaRot.LookRotation(GetMoveVec(trans_.pos,targetPos_));

	//�����_���ōU�����𐶐�
	RandSkill();

	//�\������A�j���[�V����
	ResetAnim(nowSkillPreAnim_, changeSpeedAnim_[nowSkillPreAnim_]);

	//�x���J�E���^������
	alertCnt_ = 0.0f;
}

void Enemy::ChangeStateAtk(void)
{
	//�X�V�����̒��g������
	stateUpdate_ = std::bind(&Enemy::UpdateAtk, this);

	//�U������
	lastAtk_ = &createSkill_();
}

void Enemy::ChangeStateBreak(void)
{
	//�X�V�����̒��g������
	stateUpdate_ = std::bind(&Enemy::UpdateBreak, this);

	//�U���x�e���Ԃ̏�����
	breakCnt_ = 0;
}

void Enemy::ParamLoad(CharacterParamData::UNIT_TYPE type)
{
	//����
	UnitBase::ParamLoad(type);

	auto& data = CharacterParamData::GetInstance().GetParamData(type);

	//�����̑��x
	walkSpeed_ = data.speed_;
	runSpeed_ = data.speed_ * RUN_SPEED_MULTI;
	exp_ = data.exp_;
}

void Enemy::InitAnim()
{
	//���ʃA�j���[�V����
	animNum_.emplace(ANIM::IDLE, ANIM_IDLE);
	animNum_.emplace(ANIM::WALK, ANIM_WALK);
	animNum_.emplace(ANIM::RUN, ANIM_RUN);
	animNum_.emplace(ANIM::DAMAGE, ANIM_DAMAGE);
	animNum_.emplace(ANIM::DEATH, ANIM_DEATH);
	animNum_.emplace(ANIM::ENTRY, ANIM_ENTRY);

	//�A�j���[�V�������x�ݒ�
	changeSpeedAnim_.emplace(ANIM::IDLE, SPEED_ANIM);
	changeSpeedAnim_.emplace(ANIM::WALK, SPEED_ANIM_WALK);
	changeSpeedAnim_.emplace(ANIM::RUN, SPEED_ANIM);
	changeSpeedAnim_.emplace(ANIM::DAMAGE, SPEED_ANIM);
	changeSpeedAnim_.emplace(ANIM::DEATH, SPEED_ANIM);
	changeSpeedAnim_.emplace(ANIM::ENTRY, SPEED_ANIM);
}

void Enemy::InitEffect(void)
{
}

void Enemy::Alert(void)
{
	//�x��
	alertNowSkill_();

	//�N�[���_�E���J�E���^
	CntUp(alertCnt_);

	if (!IsAlertTime())isEndAlert_ = true;
}

void Enemy::Attack(void)
{
	//�Ή��X�L������
	processSkill_();

	//�e�U���̃J�E���g
	for (auto& nowSkill : nowSkill_)
	{
		//�U���̃J�E���g
		CntUp(nowSkill.cnt_);
	}
}

void Enemy::UpdateNml(void)
{
	//**********************************************************
	//�I������
	//**********************************************************

	/*�N�����U���͈͂ɓ��������Ԃ�J�ڂ��܂�*/
	
	/*�����̓Q�[���V�[���ɂ���܂�*/

	//**********************************************************
	//���쏈��
	//**********************************************************

	//�ҋ@�A�j���[�V����
	if (moveSpeed_ == 0.0)
		ResetAnim(ANIM::IDLE, changeSpeedAnim_[ANIM::IDLE]);
	//�����A�j���[�V����
	else if (runSpeed_ > moveSpeed_ > 0.0f)
		ResetAnim(ANIM::WALK, changeSpeedAnim_[ANIM::WALK]);
	//����A�j���[�V����
	else if (moveSpeed_ >= runSpeed_)
		ResetAnim(ANIM::RUN, changeSpeedAnim_[ANIM::RUN]);
	
	//���G

	//�ŏ��̂�
	if (startCnt_ < START_CNT)CntUp(startCnt_);

	//�X�e�[�W�ɓ��������Ȃ�
	if (isColStage_)CntUp(colStageCnt_);
	if (colStageCnt_ >= COL_STAGE_CNT)
	{
		isColStage_ = false;
		colStageCnt_ = 0.0f;
	}

	//�܂������؂�Ă��Ȃ��Ȃ�
	if (searchState_ == SEARCH_STATE::CHICKEN_SEARCH)
		//�\�肵�Ă���ڕW�Ɍ�����
		targetPos_ = preTargetPos_;

	//�ړ�����
	Move();
}

void Enemy::UpdateAlert(void)
{
	//**********************************************************
	//�I������
	//**********************************************************

	//�x���J�E���^���I������Ȃ�U���J�n
	if (isEndAlert_)
	{		
		//�x���I������̏�����
		ResetAlertJudge();

		//�U����ԂɑJ��
		ChangeState(STATE::ATTACK);

		return;
	}

	//**********************************************************
	//���쏈��
	//**********************************************************

	//�x��
	Alert();
}

void Enemy::UpdateAtk(void)
{
	//**********************************************************
	//�I������
	//**********************************************************

	//�U�����I����Ă���Ȃ��ԑJ��
	if (isEndAllAtk_)
	{
		//�U���I������̏�����
		ResetAtkJudge();

		//�x�e��ԂɑJ��
		ChangeState(STATE::BREAK);
		return;
	}

	//**********************************************************
	//���쏈��
	//**********************************************************

	//�U������
	Attack();

	//�U���A�j���[�V����
	ResetAnim(nowSkillAnim_, changeSpeedAnim_[nowSkillAnim_]);
}

void Enemy::UpdateBreak(void)
{
	//**********************************************************
	//�I������
	//**********************************************************

	//�x�e���Ԃ��I�������
	if (!IsBreak())
	{
		//�ʏ��ԂɑJ��
		ChangeState(STATE::NORMAL);
		return;
	}

	//**********************************************************
	//���쏈��
	//**********************************************************

	//�ҋ@�A�j���[�V����
	ResetAnim(ANIM::IDLE, changeSpeedAnim_[ANIM::IDLE]);

	//�U���x�e���ԃJ�E���^
	CntUp(breakCnt_);
}

void Enemy::DrawDebug(void)
{
	//�f�o�b�O
	DrawFormatString(0, Application::SCREEN_SIZE_Y - 16, 0xffffff, "EnemyHP = %d", hp_);
	int statePos = Application::SCREEN_SIZE_Y - 32;
	switch (state_)
	{
	case Enemy::STATE::NORMAL:
		DrawFormatString(0, statePos, 0x00ff00, "�G���ݏ��:NORMAL");
		break;

	case Enemy::STATE::ALERT:
		DrawFormatString(0, statePos, 0x00ff00, "�G���ݏ��:ALERT_ATTACK");
		break;

	case Enemy::STATE::ATTACK:
		DrawFormatString(0, statePos, 0x00ff00, "�G���ݏ��:ATTACK");
		break;

	case Enemy::STATE::BREAK:
		DrawFormatString(0, statePos, 0x00ff00, "�G���ݏ��:BREAK");
		break;
	}
	//�G�̓����蔻��
	DrawSphere3D(trans_.pos, radius_, 4, 0xffff00, 0xffff00, false);
	//�G�̍��G����
	DrawSphere3D(trans_.pos, searchRange_, 2, searchState_ != SEARCH_STATE::CHICKEN_SEARCH ? 0xff0000 : 0xffffff, searchState_ != SEARCH_STATE::CHICKEN_SEARCH ? 0xff0000 : 0xffffff, false);
	//�G�̍��G����
	DrawSphere3D(trans_.pos, atkStartRange_, 2, state_ == STATE::ALERT ? 0xff0000 : 0xffffff, state_ == STATE::ALERT ? 0x0000ff : 0xffffff, false);
}

void Enemy::SearchMoveInfo(void)
{
	//�ړ����x�X�V
	moveSpeed_ = walkSpeed_;
}

void Enemy::FoundMoveInfo(void)
{
	//�ړ����x�X�V
	moveSpeed_ = runSpeed_;
}

void Enemy::Draw(void)
{
#ifdef DEBUG_ENEMY
	
	//�f�o�b�O
	DrawDebug();

#endif // DEBUG_ENEMY

	//UI�̕`��
	ui_->Draw(*this);

	if (IsAlive() || anim_ == ANIM::DEATH && animTotalTime_ >= stepAnim_)
	{
		//�G���f���̕`��
		MV1DrawModel(trans_.modelId);
		


		//for (auto& nowSkill : nowSkill_)
		//{
		//	//�U���̕`��
		//	if (nowSkill.IsAttack()) { DrawSphere3D(nowSkill.pos_, nowSkill.radius_, 50.0f, 0xff0f0f, 0xff0f0f, true); }
		//	else if (nowSkill.IsBacklash()) { DrawSphere3D(nowSkill.pos_, nowSkill.radius_, 5.0f, 0xff0f0f, 0xff0f0f, false); }
		//}

		//�U���\���̕`��
		if (state_ == STATE::ALERT)
		{
			DrawPolygon3D(alertVertex_, 2, DX_NONE_GRAPH, false);
		}
	}

}

const VECTOR Enemy::GetMoveVec(const VECTOR _start, const VECTOR _goal, const float _speed)
{
	//�W�I�ւ̕����x�N�g�����擾
	VECTOR targetVec = VSub(_goal, _start);

	//���K��
	targetVec = VNorm(targetVec);

	//Y���W�͕K�v�Ȃ��̂ŗv�f������
	targetVec = { targetVec.x,0.0f,targetVec.z };

	//�ړ��ʂ����߂�
	VECTOR ret = VScale(targetVec, _speed);

	return ret;
}

void Enemy::Move(void)
{
	//�ړ��x�N�g���擾
	VECTOR targetVec = GetMoveVec(trans_.pos, targetPos_, moveSpeed_);

	//������]
	trans_.quaRot = trans_.quaRot.LookRotation(targetVec);

	//�ړ�
	trans_.pos = VAdd(trans_.pos, targetVec);

	//�ړ��ʂ̏�����
	moveSpeed_ = 0.0f;
}

Enemy::ATK& Enemy::CreateSkill(ATK_ACT _atkAct)
{
	//**********************************************************
	//�g���I������U��������ꍇ
	//**********************************************************

	//�g���I������U���ɏ㏑��
	for (auto& nowSkill : nowSkill_)
	{
		if (nowSkill.IsFinishMotion())
		{
			//�X�L���㏑��
			nowSkill = skills_[_atkAct];
			
			//�J�E���^�̏�����
			nowSkill.ResetCnt();

			//�q�b�g����̏�����
			nowSkill.isHit_ = false;

			//�����I��
			return nowSkill;
		}
	}

	//**********************************************************
	//�Ȃ��ꍇ
	//**********************************************************

	//�����_���łƂ��Ă����U���̎�ނ������甭������X�L���ɐݒ�
	nowSkill_.emplace_back(skills_[_atkAct]);

	//�J�E���^�̏�����
	nowSkill_.back().ResetCnt();

	//�q�b�g����̏�����
	nowSkill_.back().isHit_ = false;

	//�����I��
	return nowSkill_.back();
}

void Enemy::FinishAnim(void)
{
	//�A�j���[�V��������
	switch (anim_)
	{
	case UnitBase::ANIM::IDLE:
	case UnitBase::ANIM::WALK:
	case UnitBase::ANIM::RUN:
		//���[�v�Đ�
		stepAnim_ = 0.0f;
		break;

	case UnitBase::ANIM::SKILL_1:
	case UnitBase::ANIM::SKILL_2:
	case UnitBase::ANIM::DAMAGE:
	case UnitBase::ANIM::DEATH:
	case UnitBase::ANIM::ENTRY:
		break;
	}
}

void Enemy::ResetAlertJudge(void)
{
	//�I�����菉����
	isEndAlert_ = false;
}

void Enemy::ResetAlertVertex(void)
{
	for (auto& ver : alertVertex_)
	{
		ver.pos = AsoUtility::VECTOR_ZERO;
		ver.u = 0.0f;
		ver.v = 0.0f;
		ver.norm = { 0.0f,0.0f,0.0f };
		ver.dif = GetColorU8(0, 0, 0, 0);
		ver.spc = GetColorU8(0, 0, 0, 0);
		ver.su = 0;
		ver.sv = 0;
	}
}

void Enemy::ResetAtkJudge(void)
{
	//�U���I������̏�����
	isEndAllAtk_ = false;
}

void Enemy::RandSkill(void)
{
	//�X�L���̐�
	size_t size = skills_.size();

	//�X�L���̐������烉���_�������
	int rand = GetRand(size - 1);

	//�X�L��
	atkAct_ = static_cast<ATK_ACT>(rand);

	//�X�L����������
	SetUpSkill(atkAct_);

	//�X�L���ɑΉ������\������A�j���[�V�����̋L�^
	nowSkillPreAnim_ = skillPreAnims_[static_cast<int>(atkAct_)];

	//�X�L���ɑΉ������A�j���[�V�����̋L�^
	nowSkillAnim_ = skillAnims_[static_cast<int>(atkAct_)];

	//�X�L������
	createSkill_ = std::bind(&Enemy::CreateSkill, this, atkAct_);
}

void Enemy::SetUpSkill(ATK_ACT _atkAct)
{
	//�x�������Z�b�g
	alertNowSkill_ = alertSkills_[_atkAct];

	//�U�������Z�b�g
	processSkill_ = changeSkill_[_atkAct];
}

void Enemy::CreateAlert(const VECTOR& _pos, const float _widthX, const float _widthZ)
{
	//���a
	float radX = _widthX / 2;
	float radZ = _widthZ / 2;

	alertVertex_[0].pos = { _pos.x + radX,0.0f,_pos.z + radZ };
	alertVertex_[0].u = _widthX;
	alertVertex_[0].v = 0.0f;

	alertVertex_[1].pos = { _pos.x - radX,0.0f,_pos.z - radZ };
	alertVertex_[1].u = 0.0f;
	alertVertex_[1].v = _widthX;

	alertVertex_[2].pos = { _pos.x - radX,0.0f,_pos.z + radZ };
	alertVertex_[2].u = 0.0f;
	alertVertex_[2].v = 0.0f;

	alertVertex_[3].pos = { _pos.x + radX,0.0f,_pos.z + radZ };
	alertVertex_[3].u = _widthX;
	alertVertex_[3].v = 0.0f;

	alertVertex_[4].pos = { _pos.x + radX,0.0f,_pos.z - radZ };
	alertVertex_[4].u = _widthX;
	alertVertex_[4].v = _widthZ;

	alertVertex_[5].pos = { _pos.x - radX,0.0f,_pos.z - radZ };
	alertVertex_[5].u = 0.0f;
	alertVertex_[5].v = _widthZ;

	for (auto& ver : alertVertex_)
	{
		ver.pos = VSub(ver.pos, _pos);
		ver.pos = Quaternion::PosAxis(trans_.quaRot, ver.pos);
		ver.pos = VAdd(ver.pos, _pos);
		
		ver.norm = { 0.0f,0.0f,-1.0f };
		ver.dif = GetColorU8(255, 0, 0, 100);
		ver.spc = GetColorU8(0, 0, 0, 0);
		ver.su = 0;
		ver.sv = 0;
	}
}
