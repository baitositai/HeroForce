#pragma once
#include "GamaUIBase.h"

class Enemy;

class EnemyHpBar : public GamaUIBase
{

public:

	//���΍��W
	static constexpr VECTOR LOCAL_HPBAR_POS = { 0, 30, 0 };

	//�摜�g�嗦
	static constexpr float IMG_RATE = 0.8f;

	//�傫��
	static constexpr VECTOR SIZE = { 58* IMG_RATE - 7, 17* IMG_RATE - 2, 0 };

	//�ʒu�����p
	static constexpr int HP_BAR_OFFSET_TOP = 3;
	static constexpr int HP_BAR_OFFSET_BOTTOM = 2;

	//�R���X�g���N�^
	EnemyHpBar();

	//�f�X�g���N�^
	~EnemyHpBar() = default;

	//���\�[�X�̓ǂݍ���
	void Load()override;

	/// <summary>
	/// �`��
	/// </summary>
	/// <param name="enemy"></param>�G�̏��
	void Draw(Enemy& _enemy);
};

