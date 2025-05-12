#include <DxLib.h>
#include "../../Application.h"
#include "Resource.h"
#include "ResourceManager.h"

ResourceManager* ResourceManager::instance_ = nullptr;

void ResourceManager::CreateInstance(void)
{
	if (instance_ == nullptr)
	{
		instance_ = new ResourceManager();
	}
	instance_->Init();
}

ResourceManager& ResourceManager::GetInstance(void)
{
	return *instance_;
}

void ResourceManager::Init(void)
{

}

void ResourceManager::InitTitle(void)
{
	Resource res;

	//�X�J�C�h�[��
	res = Resource(Resource::TYPE::MODEL, Application::PATH_SKYDOME + "SkyDome.mv1");
	resourcesMap_.emplace(SRC::SKY_DOME, res);

	//�^�C�g�����S
	res = Resource(Resource::TYPE::IMG, Application::PATH_IMAGE + "TitleLogo.png");
	resourcesMap_.emplace(SRC::TITLE_LOGO, res);

	//�L�[�w�����b�Z�[�W
	res = Resource(Resource::TYPE::IMG, Application::PATH_IMAGE + "PleaseKey.png");
	resourcesMap_.emplace(SRC::PLEASE_KEY, res);

	//�^�C�g��BGM
	res = Resource(Resource::TYPE::SOUND, Application::PATH_BGM + "TitleBGM.m4a");
	resourcesMap_.emplace(SRC::TITLE_BGM, res);

	//�V�[���`�F���W
	res = Resource(Resource::TYPE::SOUND, Application::PATH_WAVE + "SceneChange.mp3");
	resourcesMap_.emplace(SRC::SCENE_CHANGE_SE1, res);

}

void ResourceManager::InitSelect(void)
{
	Resource res;

	//�l���I���摜
	res = Resource(Resource::TYPE::IMGS, Application::PATH_IMAGE + "PlayerNumber.png", 4, 1, 300, 300);
	resourcesMap_.emplace(SRC::PLAYER_NUM, res);
	
	//��ʐ��I���摜
	res = Resource(Resource::TYPE::IMGS, Application::PATH_IMAGE + "DisPlayNumber.png", 4, 1, 300, 300);
	resourcesMap_.emplace(SRC::DISPLAY_NUM, res);

	//�E���
	res = Resource(Resource::TYPE::IMGS, Application::PATH_IMAGE + "Right_point.png", 1, 1, 52, 52);
	resourcesMap_.emplace(SRC::RIGHT_POINT, res);

	//�����
	res = Resource(Resource::TYPE::IMGS, Application::PATH_IMAGE + "Left_point.png", 1, 1, 52, 52);
	resourcesMap_.emplace(SRC::LEFT_POINT, res);

	//����
	res = Resource(Resource::TYPE::IMGS, Application::PATH_IMAGE + "Ready.png", 1, 1, 500, 200);
	resourcesMap_.emplace(SRC::READY, res);

	//��E
	res = Resource(Resource::TYPE::IMGS, Application::PATH_IMAGE + "RoleNum.png", 1, 1, 300, 300);
	resourcesMap_.emplace(SRC::ROLE, res);

	//�f�o�C�X
	res = Resource(Resource::TYPE::IMGS, Application::PATH_IMAGE + "Device.png", 1, 1, 300, 300);
	resourcesMap_.emplace(SRC::DEVICE, res);

	//ComingSoon
	res = Resource(Resource::TYPE::IMGS, Application::PATH_IMAGE + "Coming Soon.png", 1, 1, 410, 200);
	resourcesMap_.emplace(SRC::COMING_SOON, res);

	//�L�����N�^�[�p�����[�^���X�g
	res = Resource(Resource::TYPE::IMGS, Application::PATH_IMAGE + "CharaParams.png", CHARA_PARAM_NUM_X, CHARA_PARAM_NUM_Y, CHARA_PARAM_SIZE_X, CHARA_PARAM_SIZE_Y);
	resourcesMap_.emplace(SRC::CHARA_PARAMS, res);

	//�`�L��
	res = Resource(Resource::TYPE::MODEL, Application::PATH_MODEL + "Chicken/Chicken.mv1");
	resourcesMap_.emplace(SRC::CHICKEN, res);

	//BGM
	res = Resource(Resource::TYPE::SOUND, Application::PATH_BGM + "SelectBGM.mp3");
	resourcesMap_.emplace(SRC::SELECT_BGM, res);

	//��������
	res = Resource(Resource::TYPE::SOUND, Application::PATH_WAVE + "SceneChange.mp3");
	resourcesMap_.emplace(SRC::SCENE_CHANGE_SE1, res);

	//�L�����Z��
	res = Resource(Resource::TYPE::SOUND, Application::PATH_WAVE + "Select_Cancel.mp3");
	resourcesMap_.emplace(SRC::CANCEL_SELECT, res);

	//�I��ύX
	res = Resource(Resource::TYPE::SOUND, Application::PATH_WAVE + "Change_Object.mp3");
	resourcesMap_.emplace(SRC::CHANGE_SELECT, res);

	//�I������
	res = Resource(Resource::TYPE::SOUND, Application::PATH_WAVE + "Select_Decide.mp3");
	resourcesMap_.emplace(SRC::DECIDE_SELECT, res);

	//�����p�摜
	res = Resource(Resource::TYPE::IMG, Application::PATH_IMAGE + "Display_Select.png");
	resourcesMap_.emplace(SRC::DISPLAY_IMG, res);
	//�����p�摜
	res = Resource(Resource::TYPE::IMG, Application::PATH_IMAGE + "Player_Select.png");
	resourcesMap_.emplace(SRC::PLAYER_IMG, res);
	//�����p�摜
	res = Resource(Resource::TYPE::IMG, Application::PATH_IMAGE + "Operation_Select.png");
	resourcesMap_.emplace(SRC::OPERATION_IMG, res);
	//�����p�摜
	res = Resource(Resource::TYPE::IMG, Application::PATH_IMAGE + "Role_Select.png");
	resourcesMap_.emplace(SRC::ROLE_IMG, res);
	//�����p�摜
	res = Resource(Resource::TYPE::IMG, Application::PATH_IMAGE + "Waiting.png");
	resourcesMap_.emplace(SRC::WAIT_IMG, res);
	ResourcePlayer();
	ResourceEnemy();
	ResourceStage();
}

void ResourceManager::InitGame(void)
{
	Resource res;

	//���G�t�F�N�g
	res = Resource(Resource::TYPE::IMGS, Application::PATH_IMAGE + "Smokes.png", SMOKE_NUM_X, SMOKE_NUM_Y, SMOKE_SIZE_X, SMOKE_SIZE_Y);
	resourcesMap_.emplace(SRC::SMOKE, res);

	//�_���[�W�G�t�F�N�g
	res = Resource(Resource::TYPE::EFFEKSEER, Application::PATH_EFFECT + "Damage.efkefc");
	resourcesMap_.emplace(SRC::DAMAGE_EFE, res);

	//�Q�[��UI�p�����摜2
	res = Resource(Resource::TYPE::IMGS, Application::PATH_IMAGE + "Numbers2.png", NUMBERS_NUM_X, NUMBERS_NUM_Y, NUMBERS_SIZE, NUMBERS_SIZE);
	resourcesMap_.emplace(SRC::NUMBERS2, res);

	//�p�����[�^�[UI
	res = Resource(Resource::TYPE::IMG, Application::PATH_IMAGE + "ParamUI.png");
	resourcesMap_.emplace(SRC::PARAM_UI, res);

	//�{�XHP
	res = Resource(Resource::TYPE::IMG, Application::PATH_IMAGE + "BossHpBar.png");
	resourcesMap_.emplace(SRC::HP_BOSS, res);

	//�{�XHPBar
	res = Resource(Resource::TYPE::IMG, Application::PATH_IMAGE + "BossHpGage.png");
	resourcesMap_.emplace(SRC::HP_BOSS_GAGE, res);

	//�L�����A�C�R��
	res = Resource(Resource::TYPE::IMGS, Application::PATH_IMAGE + "CharaIcons.png", CHARA_ICON_NUM_X, CHARA_ICON_NUM_Y, CHARA_ICON_SIZE, CHARA_ICON_SIZE);
	resourcesMap_.emplace(SRC::CHARA_ICONS, res);

	//�L�������O
	res = Resource(Resource::TYPE::IMGS, Application::PATH_IMAGE + "RoleNames.png", NAME_NUM_X, NAME_NUM_Y, NAME_SIZE_X, NAME_SIZE_Y);
	resourcesMap_.emplace(SRC::ROLE_NAMES, res);

	//�^�C�}�[�w�i
	res = Resource(Resource::TYPE::IMG, Application::PATH_IMAGE + "TimerBack.png");
	resourcesMap_.emplace(SRC::TIMER_BACK, res);

	//BGM
	//�Q�[���m�[�}��
	res = Resource(Resource::TYPE::SOUND, Application::PATH_BGM + "Game_Nomal.mp3");
	resourcesMap_.emplace(SRC::GAME_NOMAL_BGM, res);
	//�Q�[���{�X
	res = Resource(Resource::TYPE::SOUND, Application::PATH_BGM + "Game_Boss.mp3");
	resourcesMap_.emplace(SRC::GAME_LAST_BGM, res);

	//���ʉ�
	//�U����炢
	res = Resource(Resource::TYPE::SOUND, Application::PATH_WAVE + "hit.mp3");
	resourcesMap_.emplace(SRC::HIT_SND, res);
	//���S(�v���C���[)
	res = Resource(Resource::TYPE::SOUND, Application::PATH_WAVE + "PlayerDeth.mp3");
	resourcesMap_.emplace(SRC::PLAYER_DETH_SND, res);
	//���S(�G)
	res = Resource(Resource::TYPE::SOUND, Application::PATH_WAVE + "EnemyDeth.mp3");
	resourcesMap_.emplace(SRC::ENEMY_DETH_SND, res);
	//���S(�j���g��)
	res = Resource(Resource::TYPE::SOUND, Application::PATH_WAVE + "ChickenDeth.mp3");
	resourcesMap_.emplace(SRC::CHICKEN_DETH_SND, res);

	//�����I��pUI�摜
	res = Resource(Resource::TYPE::IMG, Application::PATH_IMAGE + "SelectUI.png");
	resourcesMap_.emplace(SRC::SELECT_UI, res);

	//�J�[�\���܂Ƃ߉摜(�����I��p)
	res = Resource(Resource::TYPE::IMGS, Application::PATH_IMAGE + "Carsors.png", CARSORS_NUM_X, CARSORS_NUM_Y, CARSORS_SIZE, CARSORS_SIZE);
	resourcesMap_.emplace(SRC::CARSOLS, res);

	//�o���l�Q�[�W
	res = Resource(Resource::TYPE::IMG, Application::PATH_IMAGE + "CircleGage.png");
	resourcesMap_.emplace(SRC::CIRCLE_GAGE, res);

	//�o���l�o���l�Q�[�W
	res = Resource(Resource::TYPE::IMG, Application::PATH_IMAGE + "CircleExpGage.png");
	resourcesMap_.emplace(SRC::CIRCLE_EXP_GAGE, res);

	//�~�b�V�����@
	res = Resource(Resource::TYPE::IMG, Application::PATH_IMAGE + "Mission_Nomal.png");
	resourcesMap_.emplace(SRC::MISSION_NOMAL, res);

	//�~�b�V�����A
	res = Resource(Resource::TYPE::IMG, Application::PATH_IMAGE + "Mission_Last.png");
	resourcesMap_.emplace(SRC::MISSION_LAST, res);


	//�Q�[��UI�p�����摜
	res = Resource(Resource::TYPE::IMGS, Application::PATH_IMAGE + "Numbers.png", NUMBERS_NUM_X,NUMBERS_NUM_Y, NUMBERS_SIZE, NUMBERS_SIZE);
	resourcesMap_.emplace(SRC::NUMBERS, res);

	//���x����ʗp�G�t�F�N�g
	res = Resource(Resource::TYPE::IMGS, Application::PATH_IMAGE + "LevelUpEffect.png", LV_EFE_NUM_X, LV_EFE_NUM_Y, LV_EFE_SIZE, LV_EFE_SIZE);
	resourcesMap_.emplace(SRC::LEVEL_SCREEN_EFE, res);

	//���x���A�b�v�w�i
	res = Resource(Resource::TYPE::IMG, Application::PATH_IMAGE + "LevelUpBack.png");
	resourcesMap_.emplace(SRC::LEVEL_UP_BACK, res);

	//�����n�G�t�F�N�g
	res = Resource(Resource::TYPE::EFFEKSEER, Application::PATH_EFFECT + "LifeUp.efkefc");
	resourcesMap_.emplace(SRC::LIFE_UP_EFE, res);
	res = Resource(Resource::TYPE::EFFEKSEER, Application::PATH_EFFECT + "AttackUp.efkefc");
	resourcesMap_.emplace(SRC::ATTACK_UP_EFE, res);
	res = Resource(Resource::TYPE::EFFEKSEER, Application::PATH_EFFECT + "DefenceUp.efkefc");
	resourcesMap_.emplace(SRC::DEFENCE_UP_EFE, res);
	res = Resource(Resource::TYPE::EFFEKSEER, Application::PATH_EFFECT + "SpeedUp.efkefc");
	resourcesMap_.emplace(SRC::SPEED_UP_EFE, res);

	//����UI
	res = Resource(Resource::TYPE::IMG, Application::PATH_IMAGE + "AttackUp.png");
	resourcesMap_.emplace(SRC::ATTACK_UP_UI, res);
	res = Resource(Resource::TYPE::IMG, Application::PATH_IMAGE + "DefenseUp.png");
	resourcesMap_.emplace(SRC::DEFENCE_UP_UI, res);
	res = Resource(Resource::TYPE::IMG, Application::PATH_IMAGE + "SpeedUp.png");
	resourcesMap_.emplace(SRC::SPEED_UP_UI, res);
	res = Resource(Resource::TYPE::IMG, Application::PATH_IMAGE + "LifeUp.png");
	resourcesMap_.emplace(SRC::LIFE_UP_UI, res);

	//HP�֌WUI
	res = Resource(Resource::TYPE::IMG, Application::PATH_IMAGE + "HpGage.png");
	resourcesMap_.emplace(SRC::HP_GAGE, res);
	res = Resource(Resource::TYPE::IMG, Application::PATH_IMAGE + "HpBarPlayer.png");
	resourcesMap_.emplace(SRC::HP_PLAYER, res);
	res = Resource(Resource::TYPE::IMG, Application::PATH_IMAGE + "HpBarEnemy.jpg");
	resourcesMap_.emplace(SRC::HP_ENEMY, res);
	res = Resource(Resource::TYPE::IMG, Application::PATH_IMAGE + "HpBarCpu.jpg");
	resourcesMap_.emplace(SRC::HP_CPU, res);

	//����
	res = Resource(Resource::TYPE::IMG, Application::PATH_IMAGE + "time.png");
	resourcesMap_.emplace(SRC::TIME, res);


	ResourcePlayer();

	//�G�֘A�̃G�t�F�N�g
	res = Resource(Resource::TYPE::EFFEKSEER, Application::PATH_EFFECT + "EnemyEffect/StateDownSpell.efkefc");
	resourcesMap_.emplace(SRC::STATE_DOWN_EFE, res);
	res = Resource(Resource::TYPE::EFFEKSEER, Application::PATH_EFFECT + "EnemyEffect/BossPunch.efkefc");
	resourcesMap_.emplace(SRC::BOSS_PUNCH_EFE, res);
	res = Resource(Resource::TYPE::EFFEKSEER, Application::PATH_EFFECT + "EnemyEffect/BossShout.efkefc");
	resourcesMap_.emplace(SRC::BOSS_SHOUT_EFE, res);
	res = Resource(Resource::TYPE::EFFEKSEER, Application::PATH_EFFECT + "EnemyEffect/BossShoutAtk.efkefc");
	resourcesMap_.emplace(SRC::BOSS_SHOUT_ATK_EFE, res);

	ResourceEnemy();
	ResourceStage();

	//�`�L��
	res = Resource(Resource::TYPE::MODEL, Application::PATH_MODEL + "Chicken/Chicken.mv1");
	resourcesMap_.emplace(SRC::CHICKEN, res);

	//�w���v�摜
	res = Resource(Resource::TYPE::IMG, Application::PATH_IMAGE + "Help!.png");
	resourcesMap_.emplace(SRC::HELP, res);

	//�����N���X�g
	res = Resource(Resource::TYPE::IMGS, Application::PATH_IMAGE + "Ranks.png", RANKS_NUM_X, RANKS_NUM_Y, RANK_SIZE, RANK_SIZE);
	resourcesMap_.emplace(SRC::RANKS, res);

	//���U���g�w�i
	res = Resource(Resource::TYPE::IMG, Application::PATH_IMAGE + "FazeRezaltBack.jpg");
	resourcesMap_.emplace(SRC::REZALT_BACK, res);

	//���G�t�F�N�g
	res = Resource(Resource::TYPE::IMGS, Application::PATH_IMAGE + "Smokes.png", SMOKE_NUM_X, SMOKE_NUM_Y, SMOKE_SIZE_X, SMOKE_SIZE_Y);
	resourcesMap_.emplace(SRC::SMOKE, res);

	//�_���[�W�G�t�F�N�g
	res = Resource(Resource::TYPE::EFFEKSEER, Application::PATH_EFFECT + "Damage.efkefc");
	resourcesMap_.emplace(SRC::DAMAGE_EFE, res);

	//�Q�[��UI�p�����摜2
	res = Resource(Resource::TYPE::IMGS, Application::PATH_IMAGE + "Numbers2.png", NUMBERS_NUM_X, NUMBERS_NUM_Y, NUMBERS_SIZE, NUMBERS_SIZE);
	resourcesMap_.emplace(SRC::NUMBERS2, res);

	//�p�����[�^�[UI
	res = Resource(Resource::TYPE::IMG, Application::PATH_IMAGE + "ParamUI.png");
	resourcesMap_.emplace(SRC::PARAM_UI, res);

	//�{�XHP
	res = Resource(Resource::TYPE::IMG, Application::PATH_IMAGE + "BossHpBar.png");
	resourcesMap_.emplace(SRC::HP_BOSS, res);

	//�{�XHPBar
	res = Resource(Resource::TYPE::IMG, Application::PATH_IMAGE + "BossHpGage.png");
	resourcesMap_.emplace(SRC::HP_BOSS_GAGE, res);

	//�L�����A�C�R��
	res = Resource(Resource::TYPE::IMGS, Application::PATH_IMAGE + "CharaIcons.png", CHARA_ICON_NUM_X, CHARA_ICON_NUM_Y, CHARA_ICON_SIZE, CHARA_ICON_SIZE);
	resourcesMap_.emplace(SRC::CHARA_ICONS, res);

	//�L�������O
	res = Resource(Resource::TYPE::IMGS, Application::PATH_IMAGE + "RoleNames.png", NAME_NUM_X, NAME_NUM_Y, NAME_SIZE_X, NAME_SIZE_Y);
	resourcesMap_.emplace(SRC::ROLE_NAMES, res);

	//�^�C�}�[�w�i
	res = Resource(Resource::TYPE::IMG, Application::PATH_IMAGE + "TimerBack.png");
	resourcesMap_.emplace(SRC::TIMER_BACK, res);

	//�J�[�\��SE
	res = Resource(Resource::TYPE::SOUND, Application::PATH_WAVE + "Carsol.mp3");
	resourcesMap_.emplace(SRC::CARSOL_SE, res);

	//���x���A�b�vSE
	res = Resource(Resource::TYPE::SOUND, Application::PATH_WAVE + "LevelUp.mp3");
	resourcesMap_.emplace(SRC::LEVEL_UP_SE, res);
	//����SE
	res = Resource(Resource::TYPE::SOUND, Application::PATH_WAVE + "PowerUp.mp3");
	resourcesMap_.emplace(SRC::POWER_UP_SE, res);

	//�t�F�[�Y���U���gSE
	res = Resource(Resource::TYPE::SOUND, Application::PATH_WAVE + "FazeRezalt.mp3");
	resourcesMap_.emplace(SRC::FAZE_REZALT_SE, res);

	//�X�L��UI�֘A
	res = Resource(Resource::TYPE::IMG, Application::PATH_IMAGE + "skill.png");
	resourcesMap_.emplace(SRC::SKILL_UI, res);
	res = Resource(Resource::TYPE::IMGS, Application::PATH_IMAGE + "KnightSkills.png", SKILL_ICON_NUM_X, SKILL_ICON_NUM_Y, SKILL_ICON_SIZE, SKILL_ICON_SIZE);
	resourcesMap_.emplace(SRC::SKILL_KNIGHT, res);
	res = Resource(Resource::TYPE::IMGS, Application::PATH_IMAGE + "AxemanSkills.png", SKILL_ICON_NUM_X, SKILL_ICON_NUM_Y, SKILL_ICON_SIZE, SKILL_ICON_SIZE);
	resourcesMap_.emplace(SRC::SKILL_AXEMAN, res);
	res = Resource(Resource::TYPE::IMGS, Application::PATH_IMAGE + "ArcherSkills.png", SKILL_ICON_NUM_X, SKILL_ICON_NUM_Y, SKILL_ICON_SIZE, SKILL_ICON_SIZE);
	resourcesMap_.emplace(SRC::SKILL_ARCHER, res);

	//�A�[�`���[�X�L��2�����G�t�F�N�g
	res = Resource(Resource::TYPE::EFFEKSEER, Application::PATH_EFFECT + "Undine4.efkefc");
	resourcesMap_.emplace(SRC::ARCHER_SKILL2, res);

	//�A�[�`���[�X�L��2���v���C���[�Ƀq�b�g�����Ƃ��̃G�t�F�N�g
	res = Resource(Resource::TYPE::EFFEKSEER, Application::PATH_EFFECT + "Salamander.efkefc");
	resourcesMap_.emplace(SRC::BUFF_ARROW_HIT, res);
}

void ResourceManager::InitResult(void)
{
	Resource res;
	//�����N���X�g
	res = Resource(Resource::TYPE::IMGS, Application::PATH_IMAGE + "Ranks.png", RANKS_NUM_X, RANKS_NUM_Y, RANK_SIZE, RANK_SIZE);
	resourcesMap_.emplace(SRC::RANKS, res);

	//���U���g�w�i
	res = Resource(Resource::TYPE::IMG, Application::PATH_IMAGE + "FazeRezaltBack.jpg");
	resourcesMap_.emplace(SRC::REZALT_BACK, res);
}

void ResourceManager::InitGameOver(void)
{
	Resource res;

	//�Q�[���I�[�o�[UI
	res = Resource(Resource::TYPE::IMG, Application::PATH_IMAGE + "GameOver.png");
	resourcesMap_.emplace(SRC::GAMEOVER, res);

	//�Q�[���I�[�o�[�w�i
	res = Resource(Resource::TYPE::IMG, Application::PATH_IMAGE + "GameOverBack.png");
	resourcesMap_.emplace(SRC::GAMEOVER_BACK, res);

	//�^�C�g���J�ڃ��b�Z�[�W
	res = Resource(Resource::TYPE::IMG, Application::PATH_IMAGE + "ChangeTitleMes.png");
	resourcesMap_.emplace(SRC::CHANGE_TITLE_UI, res);

	//SE
	res = Resource(Resource::TYPE::SOUND, Application::PATH_WAVE + "GameOverSE.wav");
	resourcesMap_.emplace(SRC::GAMEOVER_SE, res);

	//BGM
	res = Resource(Resource::TYPE::SOUND, Application::PATH_BGM + "GameOverBGM.mp3");
	resourcesMap_.emplace(SRC::GAMEOVER_BGM, res);

	ResourcePlayer();
}

void ResourceManager::InitGameClear(void)
{
	Resource res;

	//�j�����b�Z�[�W
	res = Resource(Resource::TYPE::IMG, Application::PATH_IMAGE + "Congratulations.png");
	resourcesMap_.emplace(SRC::CONGRATULATIONS, res);

	//�^�C�g���J�ڃ��b�Z�[�W
	res = Resource(Resource::TYPE::IMG, Application::PATH_IMAGE + "ChangeTitleMes.png");
	resourcesMap_.emplace(SRC::CHANGE_TITLE_UI, res);

	//�ԉ�
	res = Resource(Resource::TYPE::EFFEKSEER, Application::PATH_EFFECT + "Fireworks.efkefc");
	resourcesMap_.emplace(SRC::FIREWORK, res);

	//SE
	res = Resource(Resource::TYPE::SOUND, Application::PATH_WAVE + "GameClearSE.mp3");
	resourcesMap_.emplace(SRC::GAMECLEAR_SE, res);

	//BGM
	res = Resource(Resource::TYPE::SOUND, Application::PATH_BGM + "GameClearBGM.mp3");
	resourcesMap_.emplace(SRC::GAMECLEAR_BGM, res);

	ResourcePlayer();
	ResourceStage();

	//�`�L��
	res = Resource(Resource::TYPE::MODEL, Application::PATH_MODEL + "Chicken/Chicken.mv1");
	resourcesMap_.emplace(SRC::CHICKEN, res);
}

void ResourceManager::InitExplan(void)
{
	Resource res;

	//������
	res = Resource(Resource::TYPE::IMG, Application::PATH_IMAGE + "explan.png");
	resourcesMap_.emplace(SRC::EXPLAN, res);
	res = Resource(Resource::TYPE::IMG, Application::PATH_IMAGE + "explan2.png");
	resourcesMap_.emplace(SRC::EXPLAN2, res);
	res = Resource(Resource::TYPE::IMG, Application::PATH_IMAGE + "explan3.png");
	resourcesMap_.emplace(SRC::EXPLAN3, res);
	res = Resource(Resource::TYPE::IMG, Application::PATH_IMAGE + "explan4.png");
	resourcesMap_.emplace(SRC::EXPLAN4, res);
	res = Resource(Resource::TYPE::IMG, Application::PATH_IMAGE + "explan5.png");
	resourcesMap_.emplace(SRC::EXPLAN5, res);
	res = Resource(Resource::TYPE::IMG, Application::PATH_IMAGE + "explan6.png");
	resourcesMap_.emplace(SRC::EXPLAN6, res);
	res = Resource(Resource::TYPE::IMG, Application::PATH_IMAGE + "explan7.png");
	resourcesMap_.emplace(SRC::EXPLAN7, res);

	//�����
	res = Resource(Resource::TYPE::IMGS, Application::PATH_IMAGE + "Left_point.png", 1, 1, 52, 52);
	resourcesMap_.emplace(SRC::LEFT_POINT, res);

	//�����؂�ւ����ʉ�
	res = Resource(Resource::TYPE::SOUND, Application::PATH_WAVE + "ExplanSwitch.mp3");
	resourcesMap_.emplace(SRC::EXPLAN_SWITCH, res);

	//�V�[���`�F���W
	res = Resource(Resource::TYPE::SOUND, Application::PATH_WAVE + "SceneChange.mp3");
	resourcesMap_.emplace(SRC::SCENE_CHANGE_SE1, res);
}

//�v���C���[
//*************************************************************************************************************
void ResourceManager::ResourcePlayer(void)
{
	Resource res;

	// �G�t�F�N�g
		// ********************************************************************

	//���g���̃X�L��1���ߍU��
	res = Resource(Resource::TYPE::EFFEKSEER, Application::PATH_EFFECT + "fire.efkproj");
	resourcesMap_.emplace(SRC::CHARGE_AXE_HIT, res);

	//�R�m�̃K�[�h
	res = Resource(Resource::TYPE::EFFEKSEER, Application::PATH_EFFECT + "Falloff.efkefc");
	resourcesMap_.emplace(SRC::GUARD, res);

	//�A�[�`���[�̖�̃G�t�F�N�g(��)
	res = Resource(Resource::TYPE::EFFEKSEER, Application::PATH_EFFECT + "Spear1.efkproj");
	resourcesMap_.emplace(SRC::ARROW_RED, res);

	//��̃G�t�F�N�g(��)
	res = Resource(Resource::TYPE::EFFEKSEER, Application::PATH_EFFECT + "Spear2.efkproj");
	resourcesMap_.emplace(SRC::ARROW_WHITE, res);

	//�`���[�W�G�t�F�N�g
	//res = Resource(Resource::TYPE::EFFEKSEER, Application::PATH_EFFECT + "Rock1.efkproj");
	res = Resource(Resource::TYPE::EFFEKSEER, Application::PATH_EFFECT + "Rock1.efkefc");
	resourcesMap_.emplace(SRC::CHARGE_SKILL, res);

	//������G�t�F�N�g
	res = Resource(Resource::TYPE::EFFEKSEER, Application::PATH_EFFECT + "hit_eff.efkproj");
	resourcesMap_.emplace(SRC::HIT2, res);


	//���f��
	//********************************************************************
	//���@�g���i�v���C���[�j
	res = Resource(Resource::TYPE::MODEL, Application::PATH_PLAYER + "Mage.mv1");
	resourcesMap_.emplace(SRC::PLAYER_MAGE, res);

	//�R�m�i�v���C���[�j
	res = Resource(Resource::TYPE::MODEL, Application::PATH_PLAYER + "Knight.mv1");
	resourcesMap_.emplace(SRC::PLAYER_KNIGHT, res);

	//�|�g���i�v���C���[�j
	res = Resource(Resource::TYPE::MODEL, Application::PATH_PLAYER + "Archer.mv1");
	resourcesMap_.emplace(SRC::PLAYER_ARCHER, res);

	//���g���i�v���C���[�j
	res = Resource(Resource::TYPE::MODEL, Application::PATH_PLAYER + "Axeman.mv1");
	resourcesMap_.emplace(SRC::PLAYER_AXEMAN, res);

	//���ʉ�
	//**********************************************************************

	//�X�L���ύX
	res = Resource(Resource::TYPE::SOUND, Application::PATH_WAVE + "SkillSwap.mp3");
	resourcesMap_.emplace(SRC::SKILL_CHANGE, res);
}

//�G
//*************************************************************************************************************
void ResourceManager::ResourceEnemy(void)
{
	Resource res;

	//���@�g���i�G�j
	res = Resource(Resource::TYPE::MODEL, Application::PATH_ENEMY + "Enemy_Mage.mv1");
	resourcesMap_.emplace(SRC::ENEMY_MAGE, res);

	//�R���i�G�j
	res = Resource(Resource::TYPE::MODEL, Application::PATH_ENEMY + "Enemy_Brigant.mv1");
	resourcesMap_.emplace(SRC::ENEMY_BRIGANT, res);

	//�|�g���i�G�j
	res = Resource(Resource::TYPE::MODEL, Application::PATH_ENEMY + "Enemy_Archer.mv1");
	resourcesMap_.emplace(SRC::ENEMY_ARCHER, res);

	//���g���i�G�j
	res = Resource(Resource::TYPE::MODEL, Application::PATH_ENEMY + "Enemy_Axeman.mv1");
	resourcesMap_.emplace(SRC::ENEMY_AXEMAN, res);

	//�S�[�����i�G�j
	res = Resource(Resource::TYPE::MODEL, Application::PATH_ENEMY + "Boss_Golem.mv1");
	resourcesMap_.emplace(SRC::ENEMY_GOLEM, res);

	//��
	res = Resource(Resource::TYPE::MODEL, Application::PATH_ARROW + "Arrow.mv1");
	resourcesMap_.emplace(SRC::ARROW, res);
}

//�X�e�[�W
//*************************************************************************************************************
void ResourceManager::ResourceStage(void)
{
	Resource res;

	//�X�e�[�W
	res = Resource(Resource::TYPE::MODEL, Application::PATH_STAGE + "Stage1.mv1");
	resourcesMap_.emplace(SRC::STAGE_01, res);

	//����
	res = Resource(Resource::TYPE::MODEL, Application::PATH_STAGE + "Deco1.mv1");
	resourcesMap_.emplace(SRC::DECO_01, res);

	//�X�J�C�h�[��
	res = Resource(Resource::TYPE::MODEL, Application::PATH_SKYDOME + "SkyDome.mv1");
	resourcesMap_.emplace(SRC::SKY_DOME, res);
}

void ResourceManager::Release(void)
{
	for (auto& p : loadedMap_)
	{
		p.second->Release();
		delete p.second;
	}

	loadedMap_.clear();
	resourcesMap_.clear();
}

void ResourceManager::Destroy(void)
{
	Release();
	delete instance_;
}

Resource ResourceManager::Load(SRC src)
{
	Resource* res = _Load(src);
	if (res == nullptr)
	{
		return Resource();
	}
	Resource ret = *res;
	return *res;
}

int ResourceManager::LoadModelDuplicate(SRC src)
{
	Resource* res = _Load(src);
	if (res == nullptr)
	{
		return -1;
	}

	int duId = MV1DuplicateModel(res->handleId_);
	res->duplicateModelIds_.push_back(duId);

	return duId;
}

ResourceManager::ResourceManager(void)
{
}

Resource* ResourceManager::_Load(SRC src)
{
	const auto& lPair = loadedMap_.find(src);
	if (lPair != loadedMap_.end())
	{
		return lPair->second;
	}

	const auto& rPair = resourcesMap_.find(src);
	if (rPair == resourcesMap_.end())
	{
		// �o�^����Ă��Ȃ�
		return nullptr;
	}

	rPair->second.Load();

	// �O�̂��߃R�s�[�R���X�g���N�^
	Resource* ret = new Resource(rPair->second);
	loadedMap_.emplace(src, ret);

	return ret;
}
