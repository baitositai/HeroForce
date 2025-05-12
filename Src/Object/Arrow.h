#pragma once

#include"UnitBase.h"
#include"Common/Transform.h"

class Arrow
{
public:

	//�萔
	static constexpr VECTOR ARROW_LOCAL_POS = { 15.0f,30.0f,50.0f };	//�ł��n�߂Ɉ�a�����Ȃ��悤�ɒ���
	static constexpr float GRAVITY = 0.5f;			//�d��
	static constexpr float START_UP_ANGLE = 10.0f;	//�ŏ��̏�A���O��

	enum class STATE
	{
		NONE,
		SHOT,
		DESTROY,
		END,
	};

	//���f��ID�A�����ʒu�A�s����,����,�Ώۈʒu,�U����
	Arrow(void);	
	~Arrow(void) = default;

	/// <summary>
	/// ������
	/// </summary>
	/// <param name="_mdlId">��̃��f��ID</param>
	/// <param name="_trans">�A�[�`���[�̈ʒu���</param>
	/// <param name="_speed">���x</param>
	void Init(const int _mdlId, const Transform& _trans, const float _speed);
	void Update(UnitBase::ATK& _atk);
	void Draw(void);
	void Release();
	void Destroy(void);

	const VECTOR GetPos(void)const { return trans_.pos; }
	const bool GetIsAlive(void)const { return isAlive_; }
	const STATE GetState(void)const { return state_; }

	//�󋵕ψ�
	void ChangeState(const STATE _state) { state_ = _state; }

	////��]��������
	//void SetQuaRot(const Quaternion& _quaRot) { trans_.quaRot = _quaRot; }

	////�ʒu�ݒ�
	//void SetPos(const VECTOR& _pos) { trans_.pos = _pos; }

private:
	//�ʒu���
	Transform trans_;
	//�U���Ώ�
	VECTOR targetPos_;
	//���
	STATE state_;
	//�U����
	float atkPow_;
	//���x
	float speed_;
	//��������
	bool isAlive_;

	//�ړ�����
	void Move(void);
	//�����t���O�ݒ�
	void SetIsAlive(const bool _flag) { isAlive_ = _flag; }
};

