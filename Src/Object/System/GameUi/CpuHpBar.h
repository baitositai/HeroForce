#pragma once
#include "GamaUIBase.h"

class CpuHpBar : public GamaUIBase
{
public:

	//�傫��
	static constexpr VECTOR SIZE = { 58, 17, 0 };

	//���΍��W
	static constexpr VECTOR LOCAL_HPBAR_POS = { 0, 5, 0 };

	//�摜�̊g�嗦
	static constexpr float IMG_RATE = 1.2f;

	//�R���X�g���N�^
	CpuHpBar();

	//�f�X�g���N�^
	~CpuHpBar();

	//�ǂݍ���
	void Load()override;

	//�`��
	void Draw()override;

private:


};

