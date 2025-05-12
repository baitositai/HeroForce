#include "Mage.h"

Mage::Mage(void)
{
}

void Mage::SetParam(void)
{
	//�X�e�[�^�X�֌W
//------------------------------------------
	hpMax_ = HP_MAX;
	atkPow_ = POW_ATK;
	def_ = DEF_MAX;
	defSpeed_ = SPEED;

	//���f��
	trans_.SetModel(
		ResourceManager::GetInstance()
		.LoadModelDuplicate(ResourceManager::SRC::PLAYER_MAGE));
	float scale = CHARACTER_SCALE;
	trans_.scl = { scale, scale, scale };
	trans_.pos = { -300.0f, 0.0f, 0.0f };
	trans_.quaRot = Quaternion();
	trans_.quaRotLocal = Quaternion::Euler(
		0.0f, AsoUtility::Deg2RadF(180.0f),
		0.0f
	);
	//�X�e�[�^�X
	ParamLoad(CharacterParamData::UNIT_TYPE::MAGE);

	//��E�̊��蓖��
	role_ = SceneManager::ROLE::MAGE;

	ResetAnim(ANIM::IDLE, SPEED_ANIM_IDLE);
}

void Mage::AtkFunc(void)
{
}

void Mage::Skill1Func(void)
{
}

void Mage::Skill2Func(void)
{
}

void Mage::NmlAtkInit(void)
{
}

void Mage::SkillOneInit(void)
{
}

void Mage::SkillTwoInit(void)
{
}
