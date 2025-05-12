#include"Enemy.h"
#include "EneBrig.h"

EneBrig::EneBrig(const VECTOR& _spawnPos) : Enemy(_spawnPos)
{
	trans_.pos = _spawnPos;
}

void EneBrig::SetParam(void)
{
	//�U���x��
	alertSkills_.emplace(ATK_ACT::SKILL_ONE, std::bind(&EneBrig::AlertSkill_One, this));

	//�U���̑J��
	changeSkill_.emplace(ATK_ACT::SKILL_ONE, std::bind(&EneBrig::Skill_One, this));

	//���f���ǂݍ���
	trans_.SetModel(ResourceManager::GetInstance().LoadModelDuplicate(ResourceManager::SRC::ENEMY_BRIGANT));

	//���X�Őݒ肷��
	trans_.scl = { CHARACTER_SCALE,CHARACTER_SCALE,CHARACTER_SCALE };
	colPos_ = VAdd(trans_.pos, LOCAL_CENTER_POS);
	localCenterPos_ = LOCAL_CENTER_POS;
	searchRange_ = SEARCH_RANGE;
	atkStartRange_ = ATK_START_RANGE;

	//�O������X�e�[�^�X���擾
	ParamLoad(CharacterParamData::UNIT_TYPE::E_KNIGHT);
}

void EneBrig::InitAnim(void)
{
	//���ʃA�j���[�V����������
	Enemy::InitAnim();

	//�ŗL�A�j���[�V����������
	animNum_.emplace(ANIM::SKILL_1, ANIM_SKILL_ONE);
	animNum_.emplace(ANIM::UNIQUE_1, ANIM_PRE_SWING);

	//�A�j���[�V�������x�ݒ�
	changeSpeedAnim_.emplace(ANIM::SKILL_1, SPEED_ANIM);
	changeSpeedAnim_.emplace(ANIM::UNIQUE_1, SPEED_ANIM);

	//�A�j���[�V�������Z�b�g
	ResetAnim(ANIM::IDLE, changeSpeedAnim_[ANIM::IDLE]);
}

void EneBrig::InitSkill(void)
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

void EneBrig::AlertSkill_One(void)
{
	//�G�̑O��
	VECTOR pos = trans_.GetForward();

	//�U���͈�
	pos = VScale(pos, SKILL_ONE_COL_RADIUS);

	//���W���킹
	pos = VAdd(trans_.pos, pos);

	//�͈͍쐬
	CreateAlert(pos, SKILL_ONE_COL_RADIUS * 2, SKILL_ONE_COL_RADIUS * 2);
}

void EneBrig::Skill_One(void)
{
	//�I������
	if (lastAtk_->IsFinishMotion())
	{
		//�U���I��
		isEndAllAtk_ = true;

		//�����I��
		return;
	}

	for (auto& nowSkill : nowSkill_)
	{
		//���W�̐ݒ�
		nowSkill.pos_ = MV1GetFramePosition(trans_.modelId, FRAME_KNIFE);
	}
}

void EneBrig::FinishAnim(void)
{
	//���ʃA�j���[�V�����̏I������
	Enemy::FinishAnim();

	switch (anim_)
	{
	case UnitBase::ANIM::UNIQUE_1:
		break;
	}
}