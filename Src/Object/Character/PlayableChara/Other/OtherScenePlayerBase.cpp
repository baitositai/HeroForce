#include"../../../../Manager/GameSystem/DataBank.h"
#include"../../../../Utility/AsoUtility.h"
#include "OtherScenePlayerBase.h"

using S_ROLE = SceneManager::ROLE;

OtherScenePlayerBase::OtherScenePlayerBase()
{
	for (int i = 0; i < PLAYERS; i++) { role_[i] = S_ROLE::NONE; }
}

void OtherScenePlayerBase::Init()
{
	//��E�ݒ�
	for (int i = 0; i < PLAYERS; i++)
	{
		role_[i] = DataBank::GetInstance().Output(i + 1).role_;
	}

	//3D���f��������
	Init3DModel();

	//�A�j���[�V�����̏�����
	InitAnim();
}

void OtherScenePlayerBase::Update()
{
	//�A�j���[�V����
	Animation();

	for (auto& tran_ : transArray_)
	{
		//���f���̏�����
		tran_.Update();
	}
}

void OtherScenePlayerBase::Draw()
{
	for (int i = 0; i < PLAYERS; i++)
	{
		MV1DrawModel(transArray_[i].modelId);
	}
}

void OtherScenePlayerBase::Init3DModel()
{
	//��E���f���̐ݒ�
	for (int i = 0; i < PLAYERS; i++) {

		switch (role_[i])
		{
		case S_ROLE::KNIGHT:
			transArray_[i].SetModel(
				ResourceManager::GetInstance()
				.LoadModelDuplicate(ResourceManager::SRC::PLAYER_KNIGHT));
			break;

		case S_ROLE::AXEMAN:
			transArray_[i].SetModel(
				ResourceManager::GetInstance()
				.LoadModelDuplicate(ResourceManager::SRC::PLAYER_AXEMAN));
			break;

		case S_ROLE::MAGE:
			transArray_[i].SetModel(
				ResourceManager::GetInstance()
				.LoadModelDuplicate(ResourceManager::SRC::PLAYER_MAGE));
			break;

		case S_ROLE::ARCHER:
			transArray_[i].SetModel(
				ResourceManager::GetInstance()
				.LoadModelDuplicate(ResourceManager::SRC::PLAYER_ARCHER));
			break;

		default:
			break;
		}

		//���f���̐ݒ�
		transArray_[i].pos = PLAYER_POS[i];
		transArray_[i].scl = SCALE;
		transArray_[i].quaRot = Quaternion();
		transArray_[i].quaRotLocal = Quaternion::Euler(
			0.0f,
			AsoUtility::Deg2RadF(PLAYER_ROT[i]),
			0.0f);
		transArray_[i].Update();
	}
}

void OtherScenePlayerBase::Animation()
{
	float deltaTime = 1.0f / Application::DEFAULT_FPS;
	// �A�j���[�V�������Ԃ̐i�s
	stepAnim_ += (speedAnim_ * deltaTime);
	if (stepAnim_ > animTotalTime_)
	{
		//�A�j���[�V�����I���������i�p����ōs�������߂Ă����j
		FinishAnim();
	}
	// �Đ�����A�j���[�V�������Ԃ̐ݒ�
	for (auto& t : transArray_)
	{
		MV1SetAttachAnimTime(t.modelId, atcAnim_, stepAnim_);
	}
}

void OtherScenePlayerBase::ResetAnim(const ANIM _anim, const float _speed)
{
	if (anim_ == _anim)return;

	//�A�j���[�V�����X�s�[�h�̕ύX
	speedAnim_ = _speed;

	//�A�j���[�V�����X�e�[�g�ݒ�
	anim_ = _anim;
	stepAnim_ = 0.0f;

	for (auto& t : transArray_)
	{
		//�f�^�b�`
		//����atcAnim�̏�����
		MV1DetachAnim(t.modelId, atcAnim_);

		//�A�^�b�`
		//����atcAnim�̑��
		atcAnim_ = MV1AttachAnim(t.modelId, animNum_[anim_]);
		animTotalTime_ = MV1GetAttachAnimTotalTime(t.modelId, atcAnim_);

		// �Đ�����A�j���[�V�������Ԃ̐ݒ�
		MV1SetAttachAnimTime(t.modelId, atcAnim_, stepAnim_);
	}
}