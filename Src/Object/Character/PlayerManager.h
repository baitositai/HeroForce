#pragma once
#include"../../Manager/GameSystem/DataBank.h"
#include"../../Manager/Generic/SceneManager.h"

class PlayerBase;
class JobManagerBase;

class PlayerManager
{
public:

	//�v���C���[�ɂ��Ă̍\����
	struct PLAYER {
		JobManagerBase* ins;	//�������L�����N�^�[�̃C���X�^���X
		DataBank::PLAYER_INFO info;	//���̑����(�ڍׂ�DataBank�Q��)
	};

	//�v���C���[�̐�
	static constexpr int PLAYER_NUM = 4;

	void Init(void);
	void Update(void);
	void Draw(void);
	void Release(void);

	/// <summary>
	/// ����̃L�����N�^�̏��𓾂�
	/// </summary>
	/// <param name="_num">��P��</param>
	/// <returns>���</returns>
	PlayerBase* GetPlayer(const int _num);

	//�Փ˔���
	void CollisionStage(const Transform& stageTrans);
	void ResetFaze(void);

private:
	//�v���C���[�̊Ǘ�
	PLAYER players_[PLAYER_NUM];

	//�p�b�h�̎g�p�䐔���Ǘ�
	int padNum_;
	
	JobManagerBase* CreateKeybordPlayer(const SceneManager::ROLE _role)const;
	JobManagerBase* CreatePadPlayer(const SceneManager::ROLE _role);
	JobManagerBase* CreateCpuPlayer(const SceneManager::ROLE _role);
};

