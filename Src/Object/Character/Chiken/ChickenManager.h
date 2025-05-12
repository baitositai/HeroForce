#pragma once
#include <vector>
#include <random>
#include <DxLib.h>
#include "../../Common/Transform.h"
#include "../../../Utility/AsoUtility.h"
#include "../PlayableChara/PlayerBase.h"
#include "ChickenBase.h"


class ChickenBase;

class ChickenManager
{
public:

	//�`�L��������
	static constexpr int CREATE_COUNT = 10;

	/// <summary>
	/// �R���X�g���N�^
	/// </summary>
	/// <param name="pos"></param>�z�u���W�z��
	/// <param name="stageTrans"></param>�X�e�[�W�̏��
	/// <param name="playerTrans"></param>�v���C���[���(1p�̂�)
	ChickenManager(
		std::vector<VECTOR> pos,
		const Transform& stageTrans,
		const Transform& playerTrans);

	/// <summary>
	/// �f�X�g���N�^
	/// </summary>
	~ChickenManager() = default;

	/// <summary>
	/// ������
	/// </summary>
	void Init();

	/// <summary>
	/// �X�V
	/// </summary>
	void Update();

	/// <summary>
	/// �`��
	/// </summary>
	void Draw();

	/// <summary>
	/// �v���C���[�̍��W���󂯎��
	/// </summary>
	/// <param name="pos"></param>1P�̃v���C���[���W
	//void SetTargetPos(const VECTOR pos);

	/// <summary>
	/// �`�L��������
	/// </summary>
	/// <param name=""></param>
	/// <returns></returns>������
	const int GetAliveNum()const;

	/// <summary>
	/// �`�L����Ԃ�
	/// </summary>
	/// <param name="_num"></param>�`�L���̔ԍ�
	/// <returns></returns>�w�肵���ԍ��̃`�L��
	std::shared_ptr<ChickenBase> GetChicken(const int _num);

	/// <summary>
	/// �`�L���z��̗v�f����Ԃ�
	/// </summary>
	/// <param name=""></param>
	/// <returns></returns>�z��̗v�f��
	const int GetChickenAllNum()const;

private:

	//���X�|�[���ʒu�p���W�z��
	std::vector<VECTOR> pos_;

	//�^�[�Q�b�g�ʒu
	VECTOR targetPos_;

	//�v���C���[�p�g�����X�t�H�[��
	const Transform& playerTrans_;

	//�X�e�[�W�p�g�����X�t�H�[��
	const Transform& stageTrans_;

	//�`�L���C���X�^���X
	std::vector<std::shared_ptr<ChickenBase>> chickens_;	
	
	//�����ʒu�̏����V���b�t������
	void ShufflePos();	
	
	//�Փ˔���
	void CollisionStage(const Transform& stageTrans, std::shared_ptr<ChickenBase> cheken);
};

