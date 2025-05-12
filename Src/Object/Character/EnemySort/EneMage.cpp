#include"Enemy.h"
#include "EneMage.h"

EneMage::EneMage(const VECTOR& _spawnPos) : Enemy(_spawnPos)
{
	trans_.pos = _spawnPos;
}

void EneMage::Destroy(void)
{
	auto& eff = EffectManager::GetInstance();

	//����
	Enemy::Destroy();

	eff.Stop(EffectManager::EFFECT::STATE_DOWN);
}

void EneMage::SetParam(void)
{
	//�U���x��
	alertSkills_.emplace(ATK_ACT::SKILL_ONE, std::bind(&EneMage::AlertSkill_One, this));

	//�U���̑J��
	changeSkill_.emplace(ATK_ACT::SKILL_ONE, std::bind(&EneMage::Skill_One, this));

	//���f���ǂݍ���
	trans_.SetModel(ResourceManager::GetInstance().LoadModelDuplicate(ResourceManager::SRC::ENEMY_MAGE));

	//���X�Őݒ肷��
	trans_.scl = { CHARACTER_SCALE,CHARACTER_SCALE,CHARACTER_SCALE };
	colPos_ = VAdd(trans_.pos, LOCAL_CENTER_POS);
	localCenterPos_ = LOCAL_CENTER_POS;
	searchRange_ = SEARCH_RANGE;
	atkStartRange_ = ATK_START_RANGE;
	
	skillOneShot_ = AsoUtility::VECTOR_ZERO;
	skillOneDelayCnt_ = 0.0f;
	skillAllCnt_ = 0.0f;

	//�O������X�e�[�^�X���擾
	ParamLoad(CharacterParamData::UNIT_TYPE::E_MAGE);
}

void EneMage::InitAnim(void)
{
	//���ʃA�j���[�V����������
	Enemy::InitAnim();

	//�ŗL�A�j���[�V����������
	animNum_.emplace(ANIM::SKILL_1, ANIM_SKILL_ONE);
	animNum_.emplace(ANIM::UNIQUE_1, ANIM_CHARGE);

	//�A�j���[�V�������x�ݒ�
	changeSpeedAnim_.emplace(ANIM::SKILL_1, SPEED_ANIM);
	changeSpeedAnim_.emplace(ANIM::UNIQUE_1, SPEED_ANIM);

	//�A�j���[�V�������Z�b�g
	ResetAnim(ANIM::IDLE, changeSpeedAnim_[ANIM::IDLE]);
}

void EneMage::InitEffect(void)
{
	auto& eff = EffectManager::GetInstance();
	auto& res = ResourceManager::GetInstance();

	eff.Add(EffectManager::EFFECT::STATE_DOWN,
		res.Load(ResourceManager::SRC::STATE_DOWN_EFE).handleId_);
}

void EneMage::InitSkill(void)
{
	//�����ɃX�L���̐����i�[������
	skills_.emplace(ATK_ACT::SKILL_ONE, SKILL_ONE);

	//�����ɃX�L���̐����A�j���[�V�������i�[������
	//----------------------------------------------

	//�\������A�j���[�V����
	skillPreAnims_.emplace_back(ANIM::UNIQUE_1);

	//����A�j���[�V����
	skillAnims_.emplace_back(ANIM::SKILL_1);


	//�����X�L����ݒ肵�Ă���
	RandSkill();
}

void EneMage::AlertSkill_One(void)
{
}

void EneMage::Attack(void)
{
	//���ʏ���
	Enemy::Attack();

	//�X�L���S�̂̃J�E���g
	CntUp(skillAllCnt_);
}

void EneMage::Skill_One(void)
{
	//�G�t�F�N�g
	auto& eff =	EffectManager::GetInstance();

	//�I������
	if (skillAllCnt_ > SKILL_ONE_ALL_TIME)
	{
		//�U���I��
		isEndAllAtk_ = true;

		//�����I��
		return;
	}

	//�X�L���P�̐������
	if (skillOneShotCnt_ < SKILL_ONE_MAX_CNT)
	{
		//���W�̐ݒ�
		skillOneShot_ = VAdd(skillOneShot_, GetMoveVec(skillOneShot_, targetPos_,SKILL_ONE_SPEED));

		//�X�L���P�̔����Ԋu
		if (skillOneDelayCnt_ >= SKILL_ONE_SHOT_DELAY)
		{
			//�U������
			//----------------------------

			//�X�L���̎������ԏ�����
			skillOneDelayCnt_ = 0.0f;

			//�U���񐔑���
			skillOneShotCnt_++;

			//�U���쐬
			ATK& thisAtk = createSkill_();

			//���������U���̈ʒu�����킹��
			thisAtk.pos_ = skillOneShot_;

			//�G�t�F�N�g�Đ�
			eff.Play(EffectManager::EFFECT::STATE_DOWN,
				thisAtk.pos_,
				Quaternion(),
				SKILL_ONE_EFF_SIZE,
				SoundManager::SOUND::NONE);
		}
	}

	//�J�E���^
	CntUp(skillOneDelayCnt_);
}

void EneMage::DrawDebug(void)
{
	Enemy::DrawDebug();
	
	if(!isEndAllAtk_)DrawSphere3D(skillOneShot_, 25.0f, 20, 0xf0f0f0, 0xf0f0f0, true);
}

void EneMage::FinishAnim(void)
{
	//���ʃA�j���[�V�����̏I������
	Enemy::FinishAnim();

	switch (anim_)
	{
	case UnitBase::ANIM::UNIQUE_1:
		//���[�v�Đ�
		stepAnim_ = 0;
		break;
	}
}

void EneMage::ResetAtkJudge(void)
{
	//����
	Enemy::ResetAtkJudge();

	//�X�L���̎������ԏ�����
	skillOneDelayCnt_ = 0.0f;

	//�U���񐔏�����
	skillOneShotCnt_ = 0;

	//�X�L���̃J�E���^������
	skillAllCnt_ = 0.0f;
}

void EneMage::ChangeStateAtk(void)
{
	//�X�V�����̒��g������
	stateUpdate_ = std::bind(&EneMage::UpdateAtk, this);

	//���������߂Đݒ�
	trans_.quaRot = trans_.quaRot.LookRotation(GetMoveVec(trans_.pos, targetPos_));

	//���W�̏����ݒ�
	skillOneShot_ = MV1GetFramePosition(trans_.modelId, FRAME_ROD);
}