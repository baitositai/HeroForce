#include"../Application.h"
#include"../Manager/Generic/SceneManager.h"
#include"../Utility/AsoUtility.h"
#include "Arrow.h"

Arrow::Arrow(void)
{
}

void Arrow::Init(const int _mdlId, const Transform& _trans, const float _speed)
{
	//�����̈��p��
	trans_ = _trans;
	trans_.modelId = _mdlId;
	speed_ = _speed;

	//���X���f���̏�����
	VECTOR localPos = trans_.quaRot.PosAxis(ARROW_LOCAL_POS);
	trans_.pos = VAdd(trans_.pos, localPos);

	trans_.scl = { 1.0f,1.0f,1.0f };
	//trans_.quaRot = trans_.quaRot.AngleAxis(START_UP_ANGLE, AsoUtility::AXIS_X);
	trans_.quaRotLocal =
		Quaternion::Euler(AsoUtility::Deg2RadF(90.0f), 0.0f, 0.0f);

	trans_.Update();

	ChangeState(STATE::NONE);
	SetIsAlive(true);
}

void Arrow::Update(UnitBase::ATK& _atk)
{
	switch (state_)
	{
	case Arrow::STATE::NONE:
		break;
	case Arrow::STATE::SHOT:
		//�ړ�����
		Move();
		break;
	case Arrow::STATE::DESTROY:
		break;
	case Arrow::STATE::END:
		break;
	default:
		break;
	}

	// ���f������̊�{���X�V
	trans_.Update();

	//�Ή������U���̏��X�V
	_atk.pos_ = trans_.pos;
}

void Arrow::Draw(void)
{
	//�V���b�g��Ԃ̂Ƃ��ɂ̂ݕ`�悷��
	if (state_ == STATE::SHOT)//DrawSphere3D(trans_.pos, 5.0f, 20, 0x00ff00, 0x00ff00, true);
	MV1DrawModel(trans_.modelId);
}

void Arrow::Release()
{
}

void Arrow::Destroy(void)
{
	ChangeState(STATE::DESTROY);
	SetIsAlive(false);
}

void Arrow::Move(void)
{
	// �O�������擾
	VECTOR forward = trans_.GetForward();
	//�������̎擾
	VECTOR downward = trans_.GetDown();

	//���x�N�g��
	VECTOR widthMovePow = VScale(forward, speed_);

	// �ړ�
	//�O��
	trans_.pos =
		VAdd(trans_.pos, widthMovePow);
	//����
	trans_.pos =
		VAdd(trans_.pos, VScale(downward, GRAVITY));
}
