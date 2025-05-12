#pragma once
#include "SceneBase.h"
#include"../Common/Fader.h"
#include"../Object/Character/PlayerManager.h"
#include<vector>
#include<memory>

class Grid;
class PlayerManager;
class PlayerBase;
class Enemy;
class EnemyManager;
class StageManager;
class SkyDome;
class LevelScreenManager;
class UnitPositionLoad;
class ChickenManager;
class FazeResult;
class InformFaze;
class PlayerUI;


class GameScene : public SceneBase
{
public:
	//�f�o�b�O�p
	//#define _DEBUG_COL	//�e�X�g�p�̓G�ƃv���C���[����̂������i���̍s���R�����g�A�E�g����Ə�����j

	//�萔
	static constexpr int PHASE_TIME = 180;	//�t�F�[�Y�؂�ւ��̎��ԁi���j

	static constexpr int LAST_FAZE = 3;

	//�q�b�g�G�t�F�N�g�T�C�Y
	static constexpr float HIT_EFFECT_SIZE = 20.0f;
	//�o�t�A���[�̃q�b�g�G�t�F�N�g�T�C�Y
	static constexpr float BUFF_ARROW_EFFECT_SIZE = 40.0f;

	// �R���X�g���N�^
	GameScene(void);

	// �f�X�g���N�^
	~GameScene(void);

	void Init(void) override;
	void Update(void) override;
	void Draw(void) override;
	void Release(void) override;

	

private:

	//�X�e�[�W
	std::unique_ptr<StageManager> stage_;

	//�X�J�C�h�[��
	std::unique_ptr<SkyDome> sky_;

	//�t�F�[�_�[
	std::unique_ptr<Fader>fader_;
	bool isFadeInFinish_;

	//�Q�[���V�[���̃t�F�[�Y�J�ڒ�����
	bool isPhaseChanging_;
	int phaseCnt_;


	//�t�F�[�Y���U���g
	std::unique_ptr<FazeResult>fazeResult_;
	//���U���g����
	bool isFazeRezult_;

	//�v���C���[
	std::unique_ptr<PlayerManager>playerMng_;
	std::unique_ptr<PlayerUI> uis_[PlayerManager::PLAYER_NUM];

	//�G
	std::unique_ptr<EnemyManager>enmMng_;

	//�t�F�[�Y���J�E���g
	int fazeCnt_;

	//�t�F�[�Y���ʒm�֘A
	bool isInformFaze_;		//�ʒm������true/false
	std::unique_ptr<InformFaze>inform_;

	//�`�L��
	std::unique_ptr<ChickenManager> chicken_;

	std::unique_ptr<LevelScreenManager> level_;

	std::unique_ptr<UnitPositionLoad> unitLoad_;

	//���֘A������
	void SoundInit(void);

	//�����蔻��i�����ڂɊ�������̂̂݁j
	void Collision(void);
	void CollisionEnemy(void);	//�G�֘A�̓����蔻��
	void CollisionPlayer(void);	//�v���C���[�֘A�̓����蔻��
	void CollisionChicken(void);//�j���g�������蔻��
	//void CollisionPlayerCPU(PlayerBase& _player,const VECTOR& _pPos);	//�v���C���[(CPU)�֘A�̓����蔻��

	void CollisionPlayerArrow(int _p1Num);	//�������֌W�̓����蔻��
	void CollisionPlayerSerch(int _p1Num);	//�A�[�`���[�p�̍U���^�[�Q�b�g�擾�\

	//�t�F�[�h
	void Fade(void);

	//�t�F�[�Y�J��
	void ChangePhase(void);
	//�t�F�[�Y�X�V(���S�Ó]��)
	void UpdatePhase(void);
	//�t�F�[�Y�`��
	void DrawPhase(void);

	//�����v�f���f
	void LevelUpReflection();

	//���U���g
	void FazeResultUpdate(void);
	void InformFazeNum(void);

	void SetIsInform(const bool _flag);	//�t�F�[�Y���\�����邩�ǂ����̃t���O

	//�Q�[���I�[�o�[����
	bool IsGameOver(void);
};

