#pragma once
#include <map>
#include <string>
#include "Resource.h"

class ResourceManager
{

public:

	//�J�[�\���摜�p�萔
	static constexpr int CARSORS_NUM_X = 4;
	static constexpr int CARSORS_NUM_Y = 1;
	static constexpr int CARSORS_SIZE = 64;

	//�����摜�p�萔
	static constexpr int NUMBERS_NUM_X = 5;
	static constexpr int NUMBERS_NUM_Y = 2;
	static constexpr int NUMBERS_SIZE = 128;

	//���x���A�b�v��ʃG�t�F�N�g�p�萔
	static constexpr int LV_EFE_NUM_X = 10;
	static constexpr int LV_EFE_NUM_Y = 6;
	static constexpr int LV_EFE_SIZE = 256;

	//�L�����N�^�[�p�����[�^�萔
	static constexpr int CHARA_PARAM_NUM_X = 1;
	static constexpr int CHARA_PARAM_NUM_Y = 4;
	static constexpr int CHARA_PARAM_SIZE_X = 640;
	static constexpr int CHARA_PARAM_SIZE_Y = 360;

	//�X�R�A�����N�p�萔
	static constexpr int RANKS_NUM_X = 4;
	static constexpr int RANKS_NUM_Y = 1;
	static constexpr int RANK_SIZE = 300;

	//��
	static constexpr int SMOKE_NUM_X = 16;
	static constexpr int SMOKE_NUM_Y = 1;
	static constexpr int SMOKE_SIZE_X = 1024 / SMOKE_NUM_X;
	static constexpr int SMOKE_SIZE_Y = 77 / SMOKE_NUM_Y;

	//�A�C�R��
	static constexpr int CHARA_ICON_NUM_X = 4;
	static constexpr int CHARA_ICON_NUM_Y = 1;
	static constexpr int CHARA_ICON_SIZE = 128;

	//�L�����l�[��
	static constexpr int NAME_NUM_X = 1;
	static constexpr int NAME_NUM_Y = 4;
	static constexpr int NAME_SIZE_X = 150 / NAME_NUM_X;
	static constexpr int NAME_SIZE_Y = 180 / NAME_NUM_Y;

	//�X�L���A�C�R��
	static constexpr int SKILL_ICON_NUM_X = 1;
	static constexpr int SKILL_ICON_NUM_Y = 2;
	static constexpr int SKILL_ICON_SIZE = 120;

	// ���\�[�X��
	enum class SRC
	{
		TITLE,
		RESTART,
		NEXT,
		//���f��
		STAGE_1,		//�X�e�[�W�@
		PLAYER_MAGE,	//���@�g���i�v���C���[�j
		PLAYER_KNIGHT,	//�R�m�i�v���C���[�j
		PLAYER_ARCHER,	//�|�g���i�v���C���[�j
		PLAYER_AXEMAN,	//���g���i�v���C���[�j
		ENEMY_MAGE,		//���@�g���i�G�j
		ENEMY_BRIGANT,	//�R���i�G�j
		ENEMY_ARCHER,	//�|�g���i�G�j
		ENEMY_AXEMAN,	//���g���i�G�j
		ENEMY_GOLEM,	//�S�[����(�G)
		ARROW,

		//�X�e�[�W
		STAGE_01,
		DECO_01,

		//�Q�[���N���A
		CONGRATULATIONS,
		FIREWORK,
		GAMECLEAR_SE,
		GAMECLEAR_BGM,

		//�Q�[���I�[�o�[
		GAMEOVER,
		GAMEOVER_SE,
		GAMEOVER_BGM,
		GAMEOVER_BACK,

		//UI
		HP_PLAYER,
		HP_ENEMY,
		HP_CPU,
		HP_GAGE,
		CIRCLE_GAGE,
		CIRCLE_EXP_GAGE,
		NUMBERS,
		CHARA_PARAMS,
		TO_TITLE,
		START,
		TIME,
		MISSION_NOMAL,
		MISSION_LAST,
		CHANGE_TITLE_UI,
		CHARA_ICONS,
		HP_BOSS,
		HP_BOSS_GAGE,
		NUMBERS2,
		PARAM_UI,
		ROLE_NAMES,
		TIMER_BACK,

		//���x���ʒm�p
		LEVEL_UP,
		SELECT_UI,
		CARSOLS,
		LEVEL_SCREEN_EFE,
		LEVEL_UP_BACK,


		//����
		ATTACK_UP_UI,
		DEFENCE_UP_UI,
		SPEED_UP_UI,
		LIFE_UP_UI,
		ATTACK_UP_EFE,
		DEFENCE_UP_EFE,
		SPEED_UP_EFE,
		LIFE_UP_EFE,

		//CPU
		CHICKEN,
		HELP,

		//�I����ʗp
		PLAYER_NUM,		//�l���I��
		DISPLAY_NUM,	//�l���I��
		RIGHT_POINT,	//�E���
		LEFT_POINT,		//�����
		READY,			//����
		DEVICE,			//�f�o�C�X
		COMING_SOON,	//ComingSoon
		PLAYER_IMG,
		DISPLAY_IMG,
		OPERATION_IMG,
		ROLE_IMG,
		WAIT_IMG,

		//�^�C�g�����
		TITLE_LOGO,
		PLEASE_KEY,

		//���U���g
		RANKS,
		REZALT_BACK,

		SKY_DOME,
		ROLE,			//��E

		//���U���g
		RESULT,
		RANK_S,
		RANK_A,
		RANK_B,
		RANK_C,

		//BGM
		GAME_NOMAL_BGM,
		GAME_LAST_BGM,
		TITLE_BGM,
		SELECT_BGM,

		//���ʉ�
		HIT_SND,
		PLAYER_DETH_SND,
		ENEMY_DETH_SND,
		CHICKEN_DETH_SND,
		SCENE_CHANGE_SE1,
		CHANGE_SELECT,
		CANCEL_SELECT,
		DECIDE_SELECT,
		FAZE_REZALT_SE,
		POWER_UP_SE,
		
		SKILL_CHANGE,

		ARCHER_SKILL2,	//�A�[�`���[�X�L��2
		BUFF_ARROW_HIT,  //�A�[�`���[�X�L��2���v���C���[�ɓ���������


		//�G�t�F�N�g
		SMOKE,
		DAMAGE_EFE,
		//�G�G�t�F�N�g
		BOSS_PUNCH_EFE,
		BOSS_SHOUT_EFE,
		BOSS_SHOUT_ATK_EFE,
		STATE_DOWN_EFE,

		//�v���C���[�G�t�F�N�g
		ARROW_RED,		//��̃G�t�F�N�g(��)
		ARROW_WHITE,	//��̃G�t�F�N�g(��)

		CHARGE_AXE_HIT, //�A�b�N�X�̗��߃X�L��
		GUARD,			//�K�[�h
		CHARGE_SKILL,	//�`���[�W�G�t�F�N�g
		HIT2,			//�v���C���[����G�ւ̓�����G�t�F�N�g

		CARSOL_SE,
		LEVEL_UP_SE,
		EXPLAN_SWITCH,

		//������
		EXPLAN,
		EXPLAN2,
		EXPLAN3,
		EXPLAN4,
		EXPLAN5,
		EXPLAN6,
		EXPLAN7,

		//�X�L��UI�֌W
		SKILL_UI,
		SKILL_KNIGHT,
		SKILL_AXEMAN,
		SKILL_ARCHER,
	};

	// �����I�ɃC���X�e���X�𐶐�����
	static void CreateInstance(void);

	// �ÓI�C���X�^���X�̎擾
	static ResourceManager& GetInstance(void);

	// ������
	void Init(void);
	//�V�[�����ƂɃf�[�^��ǂݍ��ނ��Ƃɂ���
	void InitTitle(void);
	void InitSelect(void);
	void InitGame(void);
	void InitResult(void);
	void InitGameOver(void);
	void InitGameClear(void);
	void InitExplan(void);

	void ResourcePlayer(void);
	void ResourceEnemy(void);
	void ResourceStage(void);

	// ���(�V�[���ؑ֎��Ɉ�U���)
	void Release(void);

	// ���\�[�X�̊��S�j��
	void Destroy(void);

	// ���\�[�X�̃��[�h
	Resource Load(SRC src);

	// ���\�[�X�̕������[�h(���f���p)
	int LoadModelDuplicate(SRC src);

private:

	// �ÓI�C���X�^���X
	static ResourceManager* instance_;

	// ���\�[�X�Ǘ��̑Ώ�
	std::map<SRC, Resource> resourcesMap_;

	// �ǂݍ��ݍς݃��\�[�X
	std::map<SRC, Resource*> loadedMap_;

	// �f�t�H���g�R���X�g���N�^��private�ɂ��āA
	// �O�����琶���ł��Ȃ��l�ɂ���
	ResourceManager(void);

	// �f�X�g���N�^�����l
	~ResourceManager(void) = default;

	// �������[�h
	Resource* _Load(SRC src);

};

