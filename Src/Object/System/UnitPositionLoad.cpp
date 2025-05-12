#include "../../Application.h"
#include "../../Lib/nlohmann/json.hpp"
#include "UnitPositionLoad.h"

// �����̂�namespace�̏ȗ�
using json = nlohmann::json;

UnitPositionLoad::UnitPositionLoad()
{
}

UnitPositionLoad::~UnitPositionLoad()
{
}

void UnitPositionLoad::Init()
{
	//JSON�t�@�C���ݒ�
	SetJsonFile();
	
	//JSON�ǂݍ���
	JsonLoad();
}

const std::vector<VECTOR> UnitPositionLoad::GetPos(const UNIT_TYPE _type)
{
	return pos_[_type];
}

const std::vector<VECTOR> UnitPositionLoad::GetRot(const UNIT_TYPE _type)
{
	return rot_[_type];
}

void UnitPositionLoad::SetJsonFile()
{
	//JSON�t�@�C���l�[��
	jsonFile_ = Application::PATH_JSON + "CharacterPosData.json";
}

void UnitPositionLoad::JsonLoad()
{
	//JSON�t�@�C�����J��
	std::ifstream ifs(jsonFile_);
	json posData;
	if (ifs) { ifs >> posData; }	//�J����������擾
	else 
	{ 
		return; 
	}						//�J���Ȃ��ꍇ�������I��

	//���O�̓o�^
	std::string names[TYPE_MAX];
	names[static_cast<int>(UNIT_TYPE::PLAYER)] = "player";
	names[static_cast<int>(UNIT_TYPE::ENEMY)] = "enemy";
	names[static_cast<int>(UNIT_TYPE::CPU)] = "cpu";

	for (int i = 0; i< TYPE_MAX; i++)
	{
		//��ނ̐ݒ�
		UNIT_TYPE type = static_cast<UNIT_TYPE>(i);

		//���o���f�[�^�����߂�
		const auto& datas = posData[names[i]];

		//json�i�[�p�̕ϐ�
		std::vector<VECTOR> pos;	//���W
		std::vector<VECTOR> rot;	//�p�x

		for (const json& data : datas)
		{
			//���W�̎擾
			VECTOR ret = {
				data["pos"]["x"] * SCALE,
				data["pos"]["y"] * SCALE + DOWN_POS_Y,
				data["pos"]["z"] * SCALE };
			pos.push_back(ret);

			//�p�x�̎󂯎��
			ret = {
				data["rot"]["x"],
				data["rot"]["y"],
				data["rot"]["z"] };
			rot.push_back(ret);
		}
		pos_.emplace(type, pos);
		pos_.emplace(type, rot);
	}
	ifs.close();
}