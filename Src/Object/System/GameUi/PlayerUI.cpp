#include "PlayerUI.h"

PlayerUI::PlayerUI()
{
	imgParam_ = -1;
	imgSkill_ = -1;
	iconNum_ = -1;
	uiScreen_ = -1;
	atk_ = -1;
	def_ = -1;
	barLength_ = -1.0f;
	skill_ = -1;

	int i = -1;
	imgIcon_ = &i;
	imgNumber_ = &i;
	imgName_ = &i;
	imgSkillIcon_ = &i;

	for (int i = 0; i < PARAM_TYPE_MAX; i++) {
		for (int j = 0; j < PARAM_DIG_CNT; j++) {
			nums_[i][j] = -1;
		}
	}
}

PlayerUI::~PlayerUI()
{
}

void PlayerUI::Init(PlayerBase& _player,const DataBank::PLAYER_INFO& _info)
{
	//�ǂݍ���
	Load();

	switch (_info.role_)
	{
	case SceneManager::ROLE::KNIGHT:
		iconNum_ = ICON_CHARA_NUM_KN;
		imgSkillIcon_ = ResourceManager::GetInstance().Load(ResourceManager::SRC::SKILL_KNIGHT).handleIds_;
		break;
	case SceneManager::ROLE::AXEMAN:
		iconNum_ = ICON_CHARA_NUM_AX;
		imgSkillIcon_ = ResourceManager::GetInstance().Load(ResourceManager::SRC::SKILL_AXEMAN).handleIds_;
		break;
	case SceneManager::ROLE::ARCHER:
		iconNum_ = ICON_CHARA_NUM_AC;
		imgSkillIcon_ = ResourceManager::GetInstance().Load(ResourceManager::SRC::SKILL_ARCHER).handleIds_;
		break;
	case SceneManager::ROLE::MAGE:
		iconNum_ = ICON_CHARA_NUM_MA;
		imgSkillIcon_ = ResourceManager::GetInstance().Load(ResourceManager::SRC::SKILL_KNIGHT).handleIds_;
		break;
	default:
		break;
	}

	atk_ = static_cast<int>(_player.GetAtkPow());
	def_ = static_cast<int>(_player.GetDef());
	skill_ = static_cast<int>(_player.GetSkillNo());
	barLength_ = HP_BAR_LENGTH;
}

void PlayerUI::Load()
{
	auto& res = ResourceManager::GetInstance();
	GamaUIBase::Load();

	//�摜�ǂݍ���
	imgHpBar_ = res.Load(ResourceManager::SRC::HP_PLAYER).handleId_;
	imgParam_ = res.Load(ResourceManager::SRC::PARAM_UI).handleId_;
	imgIcon_ = res.Load(ResourceManager::SRC::CHARA_ICONS).handleIds_;
	imgNumber_ = res.Load(ResourceManager::SRC::NUMBERS2).handleIds_;
	imgName_ = res.Load(ResourceManager::SRC::ROLE_NAMES).handleIds_;
	imgSkill_ = res.Load(ResourceManager::SRC::SKILL_UI).handleId_;
}

void PlayerUI::Update(PlayerBase& _player)
{	
	//���̍X�V
	hp_ = _player.GetHp();
	atk_ = static_cast<int>(_player.GetAtkPow());
	def_ = static_cast<int>(_player.GetDef());
	skill_ = static_cast<int>(_player.GetSkillNo());

	//�����̐ݒ�
	nums_[static_cast<int>(PARAM_TYPE::ATK)][0] = atk_ / 100 % 10;	//3����
	nums_[static_cast<int>(PARAM_TYPE::ATK)][1] = atk_ / 10 % 10;	//2����
	nums_[static_cast<int>(PARAM_TYPE::ATK)][2] = atk_ % 10;		//1����

	nums_[static_cast<int>(PARAM_TYPE::DEF)][0] = def_ / 100 % 10;
	nums_[static_cast<int>(PARAM_TYPE::DEF)][1] = def_ / 10 % 10;
	nums_[static_cast<int>(PARAM_TYPE::DEF)][2] = def_ % 10;

	//HPBar�̒����X�V
	int hpMax = _player.GetHpMax();
	float divSize = static_cast<float>(HP_BAR_LENGTH / hpMax);
	barLength_ = divSize * hp_;
}

void PlayerUI::Draw()
{
	//�����̕`��
	SetDrawBlendMode(DX_BLENDMODE_ALPHA, BOX_ALPHA);
	DrawBox(
		BOX_POS_X, BOX_POS_Y,
		BOX_POS_X2, BOX_POS_Y2,
		0x000000, true);
	SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);

	//�A�C�R���̕`��
	DrawRotaGraph(
		ICON_POS_X,ICON_POS_Y,
		ICON_RATE,
		0.0f,
		imgIcon_[iconNum_],
		true);

	//HP�Q�[�W�̕`��
	DrawRotaGraph(
		HPGAGE_POS_X,HPGAGE_POS_Y,
		ICON_RATE,
		0.0f,
		imgHpGage_,
		true);

	//���O�̕`��
	DrawRotaGraph(
		NAME_POS_X,
		NAME_POS_Y,
		NAME_RATE,
		0.0f,
		imgName_[iconNum_],
		true);

	//HP�̕`��
	if (hp_ > 0) {
		DrawExtendGraph(
			HP_POS_X, HP_POS_Y,
			HP_POS_X + barLength_, HP_POS_Y2,
			imgHpBar_,
			true);
	}

	//�p�����[�^�֘A�̕`��
	DrawParam();

	//�X�L���֘A�̕`��
	DrawSkill();
}

void PlayerUI::DrawParam()
{
	//�p�����[�^�[
	DrawRotaGraph(
		PARAM_POS_X, PARAM_POS_Y,
		PARAM_RATE,
		0.0f,
		imgParam_,
		true);

	//�����̕`��
	for (int i = 0; i < PARAM_TYPE_MAX; i++) {
		for (int j = 0; j < PARAM_DIG_CNT; j++) {
			DrawRotaGraph(
				NUM_POS_X[i][j],
				NUM_POS_Y,
				NUMBER_RATE,
				0.0f,
				imgNumber_[nums_[i][j]],
				true);
		}
	}
}

void PlayerUI::DrawSkill()
{
	//�g�p�X�L��
	DrawRotaGraph(
		SKILL_ICON_POS_X, SKILL_ICON_POS_Y,
		SKILL_ICON_RATE,
		0.0f,
		imgSkillIcon_[skill_ - 1],
		true);

	//�X�L��
	DrawRotaGraph(
		SKILL_POS_X, SKILL_POS_Y,
		SKILL_RATE,
		0.0f,
		imgSkill_,
		true);
}
