#pragma once
#include "ChickenBase.h"
class ClearChicken : public ChickenBase
{
public:

	//�����p�̗�
	static constexpr int LIFE = 999;

	//�R���X�g���N�^
	ClearChicken();

	//�f�X�g���N�^
	~ClearChicken() = default;

private:
	
	//�p�����[�^�̐ݒ�
	void SetParam()override;
};

