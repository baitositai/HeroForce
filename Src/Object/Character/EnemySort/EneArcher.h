#pragma once
#include "Enemy.h"

class Arrow;

class EneArcher : public Enemy
{
public:
	//****************************************************************
	//�萔(�L�����ŗL)
	//****************************************************************

	//�|��֌W
	static constexpr int ARROW_SIZE_MAX = 5;	//��̍ő�ێ���
	static constexpr float RELOAD_TIME = 5.0f;	//��̃����[�h����
	static constexpr float ARROW_SPEED = 8.0f;	//��̑���

	//�A�j���[�V�����ԍ�(�L�����ŗL)
	static constexpr int ANIM_SKILL_ONE = 95;	//�X�L��1�A�j���[�V����
	static constexpr int ANIM_AIMING = 5;		//�\���A�j���[�V����(�ŗL�A�j���[�V����1)
	static constexpr int ANIM_RELOAD = 6;		//�e��[�A�j���[�V����(�ŗL�A�j���[�V����2)

	//�A�j���[�V�������x(�L�����ŗL)
	static constexpr int SPEED_ANIM_RELOAD = ARROW_SIZE_MAX * 7.0f;		//�e��[�A�j���[�V����(�ŗL�A�j���[�V����2)

	//���f���֌W
	static constexpr VECTOR  LOCAL_CENTER_POS = { 0.0f,100.0f * CHARACTER_SCALE,0.0f };	//���f���̒��S���W�ւ̑��΍��W

	//�U���֌W
	static constexpr float ALERT_TIME = 1.0f;	//�U���̌x������
	static constexpr float BREAK_TIME = 2.5f;	//�U���̋x�e����

	//�͈͊֌W
	static constexpr float SEARCH_RANGE = 2000.0f * CHARACTER_SCALE;		//���G����̑傫��
	static constexpr float ATK_START_RANGE = 1500.0f * CHARACTER_SCALE;		//�U���J�n����̑傫��

	//�X�L���̓����蔻�蔼�a
	static constexpr float SKILL_ONE_COL_RADIUS = 23.0f;	//�X�L���P�̓����蔻�蔼�a

	//�X�L���̍U����
	static constexpr float SKILL_ONE_POW = 15.0f;			//�X�L���P�̍U����

	//�X�L���̎�������
	static constexpr float SKILL_ONE_DURATION = 3.0f;		//�X�L���P�̎�������

	//�X�L���̌㌄
	static constexpr float SKILL_ONE_BACKLASH = 2.5f;		//�X�L���P�̌㌄

	//�X�L���֌W
	static constexpr ATK SKILL_ONE = { AsoUtility::VECTOR_ZERO	//�X�L���P�̗v�f
		,SKILL_ONE_COL_RADIUS
		,SKILL_ONE_POW
		,SKILL_ONE_DURATION
		,SKILL_ONE_BACKLASH
		,0.0f };

	//�R���X�g���N�^
	EneArcher(const VECTOR& _spawnPos);

private:
	//****************************************************************
	//�ϐ�
	//****************************************************************

	int arrowMdlId_;							//��̃��f��
	std::vector<std::shared_ptr<Arrow>> arrow_;	//�|��
	bool isShotArrow_;							//�����������̔���(true:������)
	int arrowCnt_;								//��̎g�p���J�E���^
	float reloadCnt_;							//��̃����[�h����
	Arrow* lastArrow_;							//�Ō�ɐ������ꂽ��

	//****************************************************************
	//�֐�
	//****************************************************************

	//���
	void Destroy(void)override;

	//�L�����ŗL�ݒ�
	void SetParam(void)override;

	//�A�j���[�V�����ԍ��̏�����
	void InitAnim(void)override;

	//�X�L���̏�����
	void InitSkill(void)override;

	//�x�����Ԓ����ǂ�����Ԃ�
	const bool IsAlertTime(void)const override { return alertCnt_ < ALERT_TIME; }
	//�x�e���Ԓ����ǂ�����Ԃ�
	const bool IsBreak(void)const override { return breakCnt_ < BREAK_TIME; }
	//�����[�h�����ǂ�����Ԃ�
	const bool IsReload(void)const { return arrowCnt_ >= ARROW_SIZE_MAX; }

	//�X�L��1�̌x��
	void AlertSkill_One(void)override;

	//�X�L��1
	void Skill_One(void)override;

	//��̐���
	Arrow& CreateArrow(void);

	//��̃����[�h
	void ReloadArrow(void);

	//�A�j���[�V�����I�����̓���
	void FinishAnim(void)override;

	//�U������̏�����
	void ResetAtkJudge(void)override;

	//��ԑJ��(�x�e)
	void ChangeStateAtk(void)override;

	//�X�V(�U��)
	void Update(void)override;
	//�X�V(�x�e)
	void UpdateBreak(void)override;
	//�`��
	void Draw(void)override;
};

