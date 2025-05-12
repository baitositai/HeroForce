#pragma once
#include <map>
#include <string>
#include <memory>
#include <iostream>
#include <fstream>

class CharacterParamData
{
public:

	//�L�����N�^�[���
	enum class UNIT_TYPE
	{
		KNIGHT,
		AXEMAN,
		MAGE,
		ARCHER,
		E_KNIGHT,
		E_AXEMAN,
		E_MAGE,
		E_ARCHER,
		BOSS,
		CHICKEN,
		MAX
	};
	
	//�ǂݍ��݃X�L����
	static constexpr int TYPE_MAX = static_cast<int>(UNIT_TYPE::MAX);
	static constexpr int SKILLS = 4;

	//�p�����[�^�[�v�f
	struct PARAM
	{
		float atk_;
		float def_;
		float speed_;
		int hp_;
		int skills_[SKILLS];
		float radius_;
		float exp_;
	};

	//�C���X�^���X�̐���
	static void CreateInstance();

	/// <summary>
	/// �C���X�^���X��Ԃ�
	/// </summary>
	/// <returns></returns>�C���X�^���X
	static CharacterParamData& GetInstance();

	//������
	void Init();

	/// <summary>
	/// ���̎󂯎��
	/// </summary>
	/// <param name="type"></param>
	/// <returns></returns>
	inline const PARAM GetParamData(const UNIT_TYPE type)const { return charParam_.at(type); }

private:

	//�ÓI�C���X�^���X
	static CharacterParamData* instance_;

	//�󂯎��p�e��ϐ�
	std::map < UNIT_TYPE, PARAM> charParam_;

	CharacterParamData();
	~CharacterParamData() = default;
};

