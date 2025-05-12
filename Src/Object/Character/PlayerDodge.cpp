#include "PlayerDodge.h"
#include "../Application.h"

void PlayerDodge::Init()
{
	dodgeCdt_ = DODGE_CDT_MAX;
	dodgeCnt_ = 0.0f;
}

void PlayerDodge::Update(Transform& transform)
{
	float deltaTime = 1.0f / Application::DEFAULT_FPS;
	//�h�b�W�t���O��true�ɂȂ�����
	if (IsDodge() && !IsCoolDodge())
	{
		// �J�E���g�����Z
		dodgeCnt_ += deltaTime;

		// �h�b�W�̃J�E���g��ǉ�
		//CntUp(dodgeCnt_);

		//�X�L�����ɉ�����o�����ɃX�L���̃J�E���g�����Z�b�g
		//atk_.ResetCnt();

		// �ړ���
		if (IsDodge())
		{
			VECTOR dir = transform.GetForward();
			//�ړ�����
			VECTOR movePow = VScale(dir, SPEED_DODGE);
			//�ړ�����
			transform.pos = VAdd(transform.pos, movePow);
		}
		else
		{
			dodgeCdt_ = 0.0f;
		}
	}
	else
	{
		dodgeCdt_ += deltaTime;
		ResetDodgeFrame();
#ifdef DEBUG_ON
		color_Col_ = 0xffffff;
#endif // DEBUG_ON
	}
}

const bool PlayerDodge::IsDodge(void)const
{
	return 0.0f < dodgeCnt_ && dodgeCnt_ < FRAME_DODGE_MAX; 
}

const bool PlayerDodge::IsCoolDodge(void)const
{
	return dodgeCdt_ < DODGE_CDT_MAX;
}

void PlayerDodge::Reset()
{
	dodgeCdt_ = DODGE_CDT_MAX;
}

const bool PlayerDodge::IsDodgeable()const
{ 
	return !IsDodge() &&  !IsCoolDodge(); 
}

//void PlayerDodge::Dodge(Transform* trans)
//{
////	//�h�b�W�t���O��true�ɂȂ�����
////	if (IsDodge() && !IsCoolDodge())
////	{
////		// �h�b�W�̃J�E���g��ǉ�
////		CntUp(dodgeCnt_);
////
////		//�X�L�����ɉ�����o�����ɃX�L���̃J�E���g�����Z�b�g
////		atk_.ResetCnt();
////
////		// �ړ���
////		if (dodgeCnt_ < FRAME_DODGE_MAX)
////		{
////			VECTOR dir = trans->GetForward();
////			//�ړ�����
////			VECTOR movePow = VScale(dir, SPEED_DODGE);
////			//�ړ�����
////			trans->pos = VAdd(trans_.pos, movePow);
////		}
////		else
////		{
////			dodgeCdt_ = 0.0f;
////		}
////	}
////	else
////	{
////		CntUp(dodgeCdt_);
////		ResetDodgeFrame();
////#ifdef DEBUG_ON
////		color_Col_ = 0xffffff;
////#endif // DEBUG_ON
//	}
////}
