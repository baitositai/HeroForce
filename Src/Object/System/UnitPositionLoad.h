#pragma once
#include <map>
#include <string>
#include <memory>
#include <fstream>
#include <DxLib.h>

class UnitPositionLoad
{
public:

	//���j�b�g���
	enum class UNIT_TYPE
	{
		PLAYER,
		ENEMY,
		CPU,
		MAX
	};

	//�X�P�[��
	static constexpr float SCALE = 100.0f;

	//�ǂݍ��񂾈ʒu��Y�l������������
	static constexpr float DOWN_POS_Y = -SCALE * 1.5f;

	//��ސ�
	static constexpr int TYPE_MAX = static_cast<int>(UNIT_TYPE::MAX);

	//�R���X�g���N�^
	UnitPositionLoad();

	//�f�X�g���N�^
	~UnitPositionLoad();

	//������
	void Init();

	/// <summary>
	/// �w�肵�����j�b�g�̍��W�z���Ԃ�
	/// </summary>
	/// <param name="_type"></param>���j�b�g�̎��
	/// <returns></returns>�w�肵�����j�b�g�̍��W�z��
	const std::vector<VECTOR>GetPos(const UNIT_TYPE _type);

	/// <summary>
	/// �w�肵�����j�b�g�̊p�x�z���Ԃ�
	/// </summary>
	/// <param name="_type"></param>���j�b�g�̎��
	/// <returns></returns>�w�肵�����j�b�g�̊p�x�z��
	const std::vector<VECTOR>GetRot(const UNIT_TYPE _type);

private:	
	
	//JSON�t�@�C��
	std::string jsonFile_;

	//���W
	std::map < UNIT_TYPE, std::vector<VECTOR>> pos_;

	//�p�x
	std::map < UNIT_TYPE, std::vector<VECTOR>> rot_;	
	
	//JSON�t�@�C���̐ݒ�
	void SetJsonFile();

	//Json�ǂݍ���
	void JsonLoad();

};

