#include "../../../Manager/Generic/ResourceManager.h"
#include "../../../Manager/GameSystem/CharacterParamData.h"
#include "../../Character/EnemySort/Enemy.h"
#include "EnemyHpBar.h"

EnemyHpBar::EnemyHpBar()
{
}

void EnemyHpBar::Load()
{
	//�ǂݍ���
	GamaUIBase::Load();

	//�摜
	imgHpBar_ = ResourceManager::GetInstance().Load(ResourceManager::SRC::HP_ENEMY).handleId_;
}

void EnemyHpBar::Draw(Enemy& _enemy)
{
	//hp�̔��f
	hp_ = _enemy.GetHp();

	//hp��0�ȉ��̏ꍇ�ȉ��̏������s��Ȃ�
	if (hp_ <= 0) { return; }

	//�f�o�b�O�֘A�ϐ�
	VECTOR pos = ConvWorldPosToScreenPos(_enemy.GetPos());
	pos = VAdd(pos, LOCAL_HPBAR_POS);

	//hp�o�[�̒������v�Z
	int hpMax = _enemy.GetHpMax();
	float divSize = SIZE.x * 2 / hpMax;
	int barLength = static_cast<int>(divSize * hp_);

	//�Q�[�W�̕`��
	DrawRotaGraph(
		pos.x, pos.y,
		IMG_RATE,
		0.0f,
		imgHpGage_,
		true,
		false);

	//�o�[�̕`��
	DrawExtendGraph(
		pos.x - SIZE.x, 
		pos.y - SIZE.y + HP_BAR_OFFSET_TOP,
		pos.x - SIZE.x + barLength, 
		pos.y + SIZE.y + HP_BAR_OFFSET_BOTTOM,
		imgHpBar_,
		true);
}
