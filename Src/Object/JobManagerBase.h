#pragma once
#include"../Manager/GameSystem/DataBank.h"
#include"Character/PlayableChara/PlayerBase.h"
class PlayerBase;
class PlayerDodge;
class JobManagerBase
{
public:

	JobManagerBase(void);
	~JobManagerBase(void);

	virtual void Init(void) = 0;
	virtual void Update(void) = 0;
	virtual void Draw(void) = 0;
	virtual void Release(void) = 0;
	PlayerBase* GetPlayer(void) { return obj_; }

	//Transform��������
	const Transform& GetTransform(void)const { return obj_->GetTransform(); }

	//�����蔻��
	const float GetRadius(void)const { return obj_->GetRadius(); }

	//�ʒu
	const VECTOR GetPos(void)const { return obj_->GetPos(); }

	//�h���
	const float GetDef(void)const { return obj_->GetDef(); }
	//�U���֌W
	const UnitBase::ATK GetAtk(void)const { return obj_->GetAtk(); }

	//�O�̃t���[���ʒu�ɒu��
	void SetPrePos(void) { obj_->SetPrePos(); }

	//�v���C���[No���Z�b�g���č��W�����߂�
	void SetPlayerNum(const int _num) { playerNum_ = _num; }

	//�������W�Ƀv���C���[��u��
	void SetInitPos(int _num);

protected:

	PlayerBase* obj_;	//�g�p����C���X�^���X�i�[
	DataBank::PLAYER_INFO info_;
	InputManager::JOYPAD_NO padNum_;		//�p�b�h�ԍ�
	virtual void AtkInput(void)=0;
	virtual void SkillOneInput(void)=0;
	virtual void SkillTwoInput(void)=0;

	int playerNum_;		//��P��(�������W��u���p)

	//�U���n�ȊO�̓��͏���(����Ƃ��X�L���`�F���W�Ƃ�)
	void ActionInput(PlayerBase* _player,PlayerDodge* _dodge);

};