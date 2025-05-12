#pragma once
#include "../UnitBase.h"

class PlayerDodge
{
public:

	enum State
	{
		IDLE,			// �ҋ@
		COOL_TIME,		// �N�[���^�C��
		DODGE_DURING,	// �h�b�W��
	};

	static constexpr int ANIM_NUM = 28;
	static constexpr float SPEED_ANIM_DODGE = 30.0f;
	static constexpr float SPEED_DODGE = 15.0f;
	static constexpr float FRAME_DODGE_MAX = 1.0f * UnitBase::CHARACTER_SCALE;
	static constexpr float DODGE_CDT_MAX = 0.5f;

	void Init();
	void Update(Transform& transform);

	void Reset();

	//����֘A
  //---------------------------------------
	const bool IsDodge(void)const;

	//����֘A
	//---------------------------------------
	//����\��
	//��𒆂��ǂ���

	const bool IsDodgeable(void)const;
	
	//�N�[���^�C�������ǂ���
	const bool IsCoolDodge(void)const;

	//// ���
	//void Dodge(Transform* transform);

	//�h�b�W�J�E���g������
	void ResetDodgeFrame(void) { dodgeCnt_ = 0.0f; }

	const float GetDodgeCnt() { return dodgeCnt_; }
	void SetDodgeCnt(const float _dodgeCnt) { dodgeCnt_ = _dodgeCnt; }

private:
	// ����n
	float dodgeCnt_;            //�h�b�W�J�E���g
	float dodgeCdt_;            //�h�b�W�̌㌄

};

