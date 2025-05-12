#include"../Application.h"
#include"../Manager/Decoration/SoundManager.h"
#include"../Manager/GameSystem/CharacterParamData.h"
#include "../Lib/nlohmann/json.hpp"
#include"../Utility/AsoUtility.h"
#include "UnitBase.h"

UnitBase::UnitBase(void)
{
	trans_.modelId = -1;
	hp_ = -1;
	trans_.pos = { 0.0f,0.0f,0.0f };
	trans_.scl = { 0.0f,0.0f,0.0f };
	trans_.rot = { 0.0f,0.0f,0.0f };	

	for (int i = 0; i < ARRAY_NUM;i++)
	{
		transArray_[i].pos = {0.0f,0.0f,0.0f};
		transArray_[i].scl = { 0.0f,0.0f,0.0f };
		transArray_[i].rot = { 0.0f,0.0f,0.0f };

		animArray_[i] = -1;
		animArrayTotalTime_[i] = -1;
		stepAnimArray_[i] = -1.0f;
		speedAnimArray_[i] = -1.0f;
	}

	anim_ = ANIM::NONE;
	atcAnim_ = -1;
	atkPow_ = -1.0f;
	damage_ = -1;
	animTotalTime_ = -1;
	stepAnim_ = -1.0f;
	speedAnim_ = 1.0f;

	prePos_ = AsoUtility::VECTOR_ZERO;
	def_ = -1.0f;
	radius_ = -1.0f;
	hpMax_ = -1;

	defAtk_ = 1.0f;
	defDef_ = 1.0f;
	defSpeed_ = 1.0f;
	defHp_ = -1;

	atkUpPercent_ = -1.0f;
	defUpPercent_= -1.0f;
	speedUpPercent_ = -1.0f;
}

UnitBase::~UnitBase(void)
{
}

void UnitBase::Destroy(void)
{
}

void UnitBase::Init(void)
{
}

void UnitBase::Update(void)
{
}

void UnitBase::Draw(void)
{
}

const bool UnitBase::IsAlive(void) const
{
	return hp_ > 0;
}

const Transform& UnitBase::GetTransform(void) const
{
	return trans_;
}

const inline Transform* UnitBase::GetTransformEntity(void) const
{
	auto ret = new Transform();

	ret->pos = trans_.pos;
	ret->scl = trans_.scl;
	ret->rot = trans_.rot;
	ret->quaRot = trans_.quaRot;
	return ret;
	return ret;
}

const VECTOR UnitBase::GetPos(void) const
{
	return trans_.pos;
}

const VECTOR UnitBase::GetRot(void) const
{
	return trans_.rot;
}

const VECTOR UnitBase::GetScl(void) const
{
	return trans_.scl;
}

const float UnitBase::GetDef(void) const
{
	return def_;
}

const UnitBase::ATK UnitBase::GetAtk(void) const
{
	return atk_;
}

const float UnitBase::GetAtkPow(void) const
{
	return atkPow_;
}

const int UnitBase::GetHp(void) const
{
	return hp_;
}

const int UnitBase::GetHpMax(void) const
{
	return hpMax_;
}

const float UnitBase::GetRadius(void) const
{
	return radius_;
}

const VECTOR UnitBase::GetPrePos() const
{
	return prePos_;
}

/// <summary>
/// �A�j���[�V�����֐�
/// </summary>
/// <param name="_num">�Đ�����A�j���[�V�����i���o�[</param>
void UnitBase::Anim(void)
{
	// �A�j���[�V�����Đ�
	// �o�ߎ��Ԃ̎擾
	float deltaTime = 1.0f / Application::DEFAULT_FPS;
	// �A�j���[�V�������Ԃ̐i�s
	stepAnim_ += (speedAnim_ * deltaTime);
	if (stepAnim_ > animTotalTime_ )
	{
		//�A�j���[�V�����I���������i�p����ōs�������߂Ă����j
		FinishAnim();
	}
	// �Đ�����A�j���[�V�������Ԃ̐ݒ�
	MV1SetAttachAnimTime(trans_.modelId, atcAnim_, stepAnim_);
}


/// <summary>
/// �A�j���[�V�����Z�b�g
/// </summary>
/// <param name="_anim">�A�j���̎w��</param>
/// <param name="_speed">�A�j���[�V�����X�s�[�h</param>
void UnitBase::ResetAnim(const ANIM _anim, const float _speed)
{
	if (anim_ == _anim)return;

	//�A�j���[�V�����X�s�[�h�̕ύX
	speedAnim_ = _speed;

	//�f�^�b�`
	//����atcAnim�̏�����
	MV1DetachAnim(trans_.modelId, atcAnim_);

	anim_ = _anim;
	//�A�^�b�`
	//����atcAnim�̑��
	atcAnim_ = MV1AttachAnim(trans_.modelId, animNum_[anim_]);

	animTotalTime_ = MV1GetAttachAnimTotalTime(trans_.modelId, atcAnim_);
	stepAnim_ = 0.0f;

	// �Đ�����A�j���[�V�������Ԃ̐ݒ�
	MV1SetAttachAnimTime(trans_.modelId, atcAnim_, stepAnim_);
}

void UnitBase::AnimArray(int i)
{
	// �A�j���[�V�����Đ�
	// �o�ߎ��Ԃ̎擾
	float deltaTime = 1.0f / Application::DEFAULT_FPS;
	stepAnimArray_[i] += (speedAnimArray_[i] * deltaTime);
	if (stepAnimArray_[i] > animArrayTotalTime_[i])
	{
		//�A�j���[�V�����I���������i�p����ōs�������߂Ă����j
		FinishAnimArray(i);
	}

	// �Đ�����A�j���[�V�������Ԃ̐ݒ�
	MV1SetAttachAnimTime(transArray_[i].modelId, animArray_[i], stepAnimArray_[i]);

}

void UnitBase::SetIsHit(const bool _flag)
{
	atk_.isHit_ = _flag;
}

void UnitBase::SetDamage(const int attackerPower, const int skillPower)
{
	//�^����_���[�W�𑝂₷(����defDef�ɂȂ��Ă邩��Ԉ���Ă�\������)
	damage_ += attackerPower * skillPower / defDef_;

	//�U�����������̂�SE�Đ�
	SoundManager::GetInstance().Play(SoundManager::SOUND::HIT);
}

int UnitBase::GetDamage(void)
{
	return damage_;
}

void UnitBase::SubHp()
{
	//�_���[�W��0���傫�������ׂ�
	if(0 < damage_)
	{
		damage_--;

		hp_--;

		if (hp_ < 0) { hp_ = 0; }
	}
}

void UnitBase::ResetAnimArray(const ANIM _anim, const float _speed, int i)
{
	if (animStateArray_[i] == _anim)return;

	speedAnimArray_[i] = _speed;

	//�f�^�b�`
	//����atcAnim�̏�����
	MV1DetachAnim(transArray_[i].modelId, animArray_[i]);

	animStateArray_[i] = _anim;

	//�A�^�b�`
	//����atcAnim�̑��
	animArray_[i] = MV1AttachAnim(transArray_[i].modelId, animNumArray_[i][animStateArray_[i]]);

	animArrayTotalTime_[i] = MV1GetAttachAnimTotalTime(transArray_[i].modelId, animArray_[i]);
	stepAnimArray_[i] = 0.0f;

	// �Đ�����A�j���[�V�������Ԃ̐ݒ�
	MV1SetAttachAnimTime(transArray_[i].modelId, animArray_[i], stepAnimArray_[i]);
}


float UnitBase::GetAnimArrayTime(int i)
{
	float ret = MV1GetAttachAnimTime(transArray_[i].modelId, animArray_[i]);
	return ret;
}
//���W�̐ݒ�
void UnitBase::SetPos(const VECTOR pos)
{
	trans_.pos = pos;
}

void UnitBase::SetPrePos(void)
{
	trans_.pos = prePos_;
}

//�U���͂̋���
void UnitBase::SetAttack(const float percent)
{
	atkUpPercent_ += percent;			//�������㏸
	atkPow_ = defAtk_ * (atkUpPercent_ / DEFAULT_PERCENT);	//�U���͂��㏸
}
 
//�h��͂̋���
void UnitBase::SetDefense(const float percent)
{
	defUpPercent_ += percent;
	def_ = defDef_ * (defUpPercent_ / DEFAULT_PERCENT);
}

//�ړ����x
void UnitBase::SetSpeed(const float percent)
{
	speedUpPercent_ += percent;
	moveSpeed_ = defSpeed_ * (speedUpPercent_ / DEFAULT_PERCENT);
}

//�̗͋���
void UnitBase::SetHpMax(const int hp)
{
	hpMax_ += hp;
}


void UnitBase::SetMoveSpeed(const float _speed)
{
	moveSpeed_ = _speed;
}

//�A�j���I�����̓���
void UnitBase::FinishAnim(void)
{
	//���[�v�Đ�
	stepAnim_ = 0.0f;
}

void UnitBase::FinishAnimArray(int i)
{
	stepAnimArray_[i] = 0.0f;
}

void UnitBase::CntUp(float& _count)
{
	// �o�ߎ��Ԃ̎擾
	float deltaTime = 1.0f / Application::DEFAULT_FPS;
	_count += deltaTime;
}

void UnitBase::CntDown(float& _count)
{
	// �o�ߎ��Ԃ̎擾
	float deltaTime = 1.0f / Application::DEFAULT_FPS;
	_count -= deltaTime;
}

void UnitBase::ParamLoad(CharacterParamData::UNIT_TYPE type)
{
	auto& data = CharacterParamData::GetInstance().GetParamData(type);

	//�f�t�H���g�̃X�e�[�^�X�ݒ�
	defAtk_ = data.atk_;
	defDef_ = data.def_;
	defSpeed_ = data.speed_;
	defHp_ = data.hp_;
	radius_ = data.radius_;

	//�ω��p�̐ݒ�
	atkPow_ = defAtk_;
	def_ = defDef_;
	moveSpeed_ = defSpeed_;
	hp_ = defHp_;

	//HP�̍ő�l�̐ݒ�
	hpMax_ = defHp_;

	//�����p�[�Z���g�̏�����
	atkUpPercent_ = DEFAULT_PERCENT;
	defUpPercent_ = DEFAULT_PERCENT;
	speedUpPercent_ = DEFAULT_PERCENT;
}
