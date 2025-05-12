#include "../Manager/Generic/SceneManager.h"
#include "../Manager/Generic/Camera.h"
#include "../Manager/GameSystem/Collision.h"
#include"../Manager/GameSystem/Timer.h"
#include"../Manager/Decoration/SoundManager.h"
#include "../Manager/Generic/InputManager.h"
#include"../Object/Character/EnemySort/Enemy.h"
#include"../Object/Character/EnemyManager.h"
#include"../Object/Character/Chiken/ChickenManager.h"
#include "../Object/Common/Transform.h"
#include "../Object/Stage/StageManager.h"
#include "../Object/Stage/SkyDome.h"
#include "../Object/System/LevelScreenManager.h"
#include "../Object/System/UnitPositionLoad.h"
#include "../Object/System/InformFaze.h"
#include "../Object/System/GameUi/PlayerUI.h"
#include "../Manager/GameSystem/DataBank.h"
#include"../Object/Character/PlayerDodge.h"
#include "../Object/System/GameUi/PlayerUI.h"
#include"FazeResult.h"
#include "GameScene.h"




GameScene::GameScene(void)
{
	stage_ = nullptr;
	sky_ = nullptr;
	fader_ = nullptr;
	level_ = nullptr;
	chicken_ = nullptr;
	unitLoad_ = nullptr;
	fazeResult_ = nullptr;

	isPhaseChanging_ = false;
	fazeCnt_ = 0;
}

GameScene::~GameScene(void)
{
}

void GameScene::Init(void)
{
	//�^�C�}�[�̐���
	Timer::CreateInstance();

	unitLoad_ = std::make_unique<UnitPositionLoad>();
	unitLoad_->Init();

	//�X�e�[�W
	stage_ = std::make_unique<StageManager>();
	stage_->Init();
	//�X�J�C�h�[��
	sky_ = std::make_unique<SkyDome>();
	sky_->Init();
	//���x���֌W
	level_ = std::make_unique<LevelScreenManager>();
	level_->Init();

	//�v���C���[�ݒ�
	playerMng_ = std::make_unique<PlayerManager>();
	playerMng_->Init();
	//UI
	for (int i = 0; i < PlayerManager::PLAYER_NUM; i++)
	{
		uis_[i] = std::make_unique<PlayerUI>();
		uis_[i]->Init(*playerMng_->GetPlayer(i), DataBank::GetInstance().Output(i + 1));
	}


	//�G�}�l�[�W���̐���
	enmMng_ = std::make_unique<EnemyManager>(unitLoad_->GetPos(UnitPositionLoad::UNIT_TYPE::ENEMY));
	enmMng_->Init();
	//�j���g���̐���
	chicken_ = std::make_unique<ChickenManager>(unitLoad_->GetPos(UnitPositionLoad::UNIT_TYPE::CPU),
		stage_->GetTtans(),
		playerMng_->GetPlayer(0)->GetTransform());
	chicken_->Init();

	//UI
	for (int i = 0; i < PlayerManager::PLAYER_NUM; i++)
	{
		uis_[i] = std::make_unique<PlayerUI>();
		uis_[i]->Init(*playerMng_->GetPlayer(i), DataBank::GetInstance().Output(i + 1));
	}

	//�J�����̐ݒ�
	auto cameras = SceneManager::GetInstance().GetCameras();
	for (int i = 0; i < cameras.size(); i++)
	{
		cameras[i]->SetFollow(&playerMng_->GetPlayer(i)->GetTransform());
		cameras[i]->ChangeMode(Camera::MODE::FOLLOW_SPRING);
	}

	//�t�F�[�_�[�̍쐬
	fader_ = std::make_unique<Fader>();
	fader_->Init();
	isFadeInFinish_ = true;

	//�t�F�[�Y���U���g�̍쐬
	fazeResult_ = std::make_unique<FazeResult>();
	fazeResult_->Init();

	//�t�F�[�Y���J�E���g
	inform_ = std::make_unique<InformFaze>();
	SetIsInform(true);


	//�����֌W�ݒ�
	SoundInit();

	//�G�t�F�N�g
	EffectManager::GetInstance().Add(
		EffectManager::EFFECT::HIT2, 
		ResourceManager::GetInstance().Load(ResourceManager::SRC::HIT2).handleId_);
	//�v���C���[�A�[�`���[�̃o�t������������̃G�t�F�N�g
	EffectManager::GetInstance().Add(
		EffectManager::EFFECT::BUFF_ARROW_HIT,
		ResourceManager::GetInstance().Load(ResourceManager::SRC::BUFF_ARROW_HIT).handleId_);

}

void GameScene::Update(void)
{
	//�Q�[���I�[�o�[����
	if(IsGameOver())SceneManager::GetInstance().ChangeScene(SceneManager::SCENE_ID::GAMEOVER);
	if (!playerMng_->GetPlayer(0)->IsAlive())SceneManager::GetInstance().ChangeScene(SceneManager::SCENE_ID::GAMEOVER);

	//�t�F�[�Y���U���g
	if (isFazeRezult_)
	{
		FazeResultUpdate();
		return;
	}

	//�t�F�[�Y�J�ڒ�
	if (isPhaseChanging_)
	{
		fader_->Update();
		//�t�F�[�h��������
		Fade();
		return;
	}
	//�t�F�[�Y���̒m�点(�t�F�[�h�I����ɍX�V��������)
	if (isInformFaze_&& isFadeInFinish_) {
		InformFazeNum();
		return;
	}

	//���x������
	level_->Update();
	//���x���A�b�v�����̑��̍X�V�͂����Ȃ�
	if (level_->IsLevelUp())return;

	//�^�C�}�[�X�V
	Timer::GetInstance().Update();
	//�^�C�}�[���I��������
	if (Timer::GetInstance().IsEnd())ChangePhase();
	

	//�v���C���[�̍X�V
	playerMng_->Update();

	//UI�X�V
	for (int i = 0; i < DataBank::GetInstance().Output(DataBank::INFO::USER_NUM); i++)
	{
		uis_[i]->Update(*playerMng_->GetPlayer(i));
	}

	//�G�̍X�V
	enmMng_->Update();
	//�G�̌o���l�擾����
	if (enmMng_->GetAllExp() != 0.0f)
	{
		//�o���l�擾
		level_->AddExp(enmMng_->GetAllExp());
		//�G����̌o���l������
		enmMng_->ClearAllExp();
	}

	
	//�`�L���̍X�V
	//chicken_->SetTargetPos(playerMng_->GetPlayer(0)->GetPos());
	chicken_->Update();

	for (int i = 0; i < DataBank::GetInstance().Output(DataBank::INFO::USER_NUM); i++)
	{
		uis_[i]->Update(*playerMng_->GetPlayer(i));
	}

	//�����蔻��
	Collision();

	//�����v�f�̔��f
	LevelUpReflection();



	//���������
	auto& ins = InputManager::GetInstance();
	auto& mng = SceneManager::GetInstance();
	////�X�y�[�X��������^�C�g���ɖ߂�
	/*if (ins.IsTrgDown(KEY_INPUT_SPACE))
	{
		mng.ChangeScene(SceneManager::SCENE_ID::GAMECLEAR);
	}*/

	/*if (ins.IsTrgDown(KEY_INPUT_RETURN))
	{
		ChangePhase();
	}*/

	//if (ins.IsTrgDown(KEY_INPUT_K))
	//	playerMng_->GetPlayer(0)->SetDamage(20, 20);
}

void GameScene::Draw(void)
{
	if (isFazeRezult_)
	{
		fazeResult_->Draw();
		return;
	}
	//�X�J�C�h�[��
	sky_->Draw();
	//�X�e�[�W
	stage_->Draw();
	////�v���C���[
	//playerMng_->Draw();

	//�G
	enmMng_->Draw();

	//�`�L��
	chicken_->Draw();

	//�v���C���[
	playerMng_->Draw();

	//���x��
	level_->Draw();

	//���x���A�b�v���^�C�}�[�y��UI�͕\�����Ȃ�
	if (level_->IsLevelUp())return;
	
	//UI�̕`��
	uis_[SceneManager::GetInstance().GetNowWindow()]->Draw();

	//��������
	Timer::GetInstance().Draw();


	if (isInformFaze_) {
		inform_->Draw();
	}

	fader_->Draw();

	if (fader_->GetState() == Fader::STATE::FADE_KEEP)
	{
		DrawPhase();
	}

}

void GameScene::Release(void)
{
	level_->Release();
	stage_->Destroy();

	/*SceneManager::GetInstance().ResetCameras();
	SceneManager::GetInstance().ReturnSolo();*/
	Timer::GetInstance().Reset();
	Timer::GetInstance().Release();

	if (fazeCnt_ == LAST_FAZE)SoundManager::GetInstance().Stop(SoundManager::SOUND::GAME_LAST);
	else SoundManager::GetInstance().Stop(SoundManager::SOUND::GAME_NOMAL);

	EffectManager::GetInstance().Stop(EffectManager::EFFECT::HIT2);
	EffectManager::GetInstance().Stop(EffectManager::EFFECT::BUFF_ARROW_HIT);
	playerMng_->Release();

	enmMng_->Release();
}


void GameScene::SoundInit(void)
{
	//BGM�̏�����
	auto& snd = SoundManager::GetInstance();

	//�ʏ�
	snd.Add(SoundManager::TYPE::BGM, SoundManager::SOUND::GAME_NOMAL,
		ResourceManager::GetInstance().Load(ResourceManager::SRC::GAME_NOMAL_BGM).handleId_);
	//�{�X��
	snd.Add(SoundManager::TYPE::BGM, SoundManager::SOUND::GAME_LAST,
		ResourceManager::GetInstance().Load(ResourceManager::SRC::GAME_LAST_BGM).handleId_);
	//���ʐݒ�
	snd.AdjustVolume(SoundManager::SOUND::GAME_LAST, 150);

	//�Q�[���V�[���J�n���̓m�[�}����BGM���Đ�
	snd.Play(SoundManager::SOUND::GAME_NOMAL);

	//���ʉ��ݒ�
	//�v���C���[���S
	snd.Add(SoundManager::TYPE::SE, SoundManager::SOUND::DETH_PLAYER,
		ResourceManager::GetInstance().Load(ResourceManager::SRC::PLAYER_DETH_SND).handleId_);
	//�G���S
	snd.Add(SoundManager::TYPE::SE, SoundManager::SOUND::DETH_ENEMY,
		ResourceManager::GetInstance().Load(ResourceManager::SRC::ENEMY_DETH_SND).handleId_);
	//�j���g�����S
	snd.Add(SoundManager::TYPE::SE, SoundManager::SOUND::DETH_CHICKEN,
		ResourceManager::GetInstance().Load(ResourceManager::SRC::CHICKEN_DETH_SND).handleId_);
	//�U����炢��
	snd.Add(SoundManager::TYPE::SE, SoundManager::SOUND::HIT,
		ResourceManager::GetInstance().Load(ResourceManager::SRC::HIT_SND).handleId_);
}

//�����蔻��i�����ڂɊ�������̂̂݁j
//�����蔻�葍��
//�����蔻��i�����ڂɊ�������̂̂݁j
//�����蔻�葍��
void GameScene::Collision(void)
{
	auto& col = Collision::GetInstance();

	CollisionEnemy();
	CollisionPlayer();
}


//�G�֌W�̓����蔻��
void GameScene::CollisionEnemy(void)
{
	//�Փ˔���}�l�[�W���擾
	auto& col = Collision::GetInstance();
	//�G�̑����擾
	int maxCnt = enmMng_->GetActiveNum();

	enmMng_->CollisionStage(stage_->GetTtans());

	//�����蔻��(��ɍ��G)
	for (int i = 0; i < maxCnt; i++)
	{
		//�G�̎擾
		Enemy* e = enmMng_->GetActiveEnemy(i);

		//�G�l�̈ʒu�ƍU�����擾
		VECTOR ePos = e->GetPos();

		//�G�l�̍��G����
		bool isPlayerFound = false;

		for (int i = 0; i < PlayerManager::PLAYER_NUM; i++)
		{
			PlayerBase* p = playerMng_->GetPlayer(i);
			//�v���C���[�������ĂȂ������珈�����΂�
			if (!p->IsAlive())
				continue;
			VECTOR pPos = p->GetPos();

			//�͈͓��ɓ����Ă���Ƃ�

			//�ʏ��Ԏ� && �U���͈͓��Ƀv���C���[����������U�����J�n
			if (col.Search(ePos, pPos, e->GetAtkStartRange()) && e->GetState() == Enemy::STATE::NORMAL) {
				//��Ԃ�ύX
				e->ChangeState(Enemy::STATE::ALERT);
			}

			if (col.Search(ePos, pPos, e->GetSearchRange())) {
				//�v���C���[��_��
				e->ChangeSearchState(Enemy::SEARCH_STATE::PLAYER_FOUND);
				e->SetTargetPos(pPos);

				//������
				isPlayerFound = true;
			}
			else if (!isPlayerFound && e->GetSearchState() != Enemy::SEARCH_STATE::CHICKEN_FOUND) {
				//�N���_���Ă��Ȃ�
				e->ChangeSearchState(Enemy::SEARCH_STATE::CHICKEN_SEARCH);
			}

			//�U������
			for (int a = 0; a < e->GetAtks().size(); a++)
			{
				//�U�������Z�b�g
				e->SetAtk(e->GetAtks()[a]);

				//�Z�b�g���Ă��������Ƃ��Ă���
				UnitBase::ATK eAtk = e->GetAtk();

				//�A�^�b�N�� && �U�����肪�I�����Ă��Ȃ��Ƃ�������������B����ȊO�͂��Ȃ��̂Ŗ߂�
				if (eAtk.IsAttack() && !eAtk.isHit_) {


					//�U����������͈� && �v���C���[��������Ă��Ȃ��Ƃ�
					if (col.IsHitAtk(*e, *p)/* && !p->GetDodge()->IsDodge()*/)
					{
						//�_���[�W
						p->SetDamage(e->GetAtkPow(), eAtk.pow_);
						//�g�p�����U���𔻒�I����
						e->SetAtksIsHit(a, true);
					}
				}
			}
		}
	}

}

void GameScene::CollisionPlayer(void)
{
	//�Փ˔���}�l�[�W���擾
	auto& col = Collision::GetInstance();
	//�G�̑����擾
	int maxCnt = enmMng_->GetActiveNum();

	//�X�e�[�W�Ƃ̔���
	playerMng_->CollisionStage(stage_->GetTtans());

	for (int i = 0; i < PlayerManager::PLAYER_NUM; i++)
	{
		PlayerBase* p = playerMng_->GetPlayer(i);

		auto pPos = p->GetPos();
		auto pAtk = p->GetAtk();

		//�ύX�ӏ�
		//--------------------------------
		auto pRole = p->GetRole();
		if (pRole == SceneManager::ROLE::ARCHER)
		{
			CollisionPlayerArrow(i);
			CollisionPlayerSerch(i);
		}
		//--------------------------------
		// 
		// 
		//�v���C���[��CPU�̎������T�[�`������
		//if (p->GetPlayMode() == SceneManager::PLAY_MODE::CPU)CollisionPlayerCPU(*p, pPos);

		//�v���C���[�U������
		//�U�����Ă��Ȃ� || �U�������łɓ������Ă���
		if (!pAtk.IsAttack() || pAtk.isHit_)continue;

		for (int i = 0; i < maxCnt; i++)
		{
			//�G�̎擾
			Enemy* e = enmMng_->GetActiveEnemy(i);
			VECTOR ePos = e->GetPos();
			//�����蔻��
			if (col.IsHitAtk(*p, *e)) {
				//��e
				e->SetDamage(p->GetAtkPow(), p->GetAtk().pow_);
				EffectManager::GetInstance().Play(EffectManager::EFFECT::HIT2, ePos, Quaternion(), HIT_EFFECT_SIZE, SoundManager::SOUND::NONE);
				//�U������̏I��
				p->SetIsHit(true);
			}
		}
	}

}

void GameScene::CollisionPlayerArrow(int _p1Num)
{
	//�Փ˔���}�l�[�W���擾
	auto& col = Collision::GetInstance();
	//�G�̑����擾
	int maxCnt = enmMng_->GetActiveNum();



	PlayerBase* p = playerMng_->GetPlayer(_p1Num);

	auto pPos = p->GetPos();
	auto pAtk = p->GetAtk();

	//�v���C���[��CPU�̎������T�[�`������
	//if (p->GetPlayMode() == SceneManager::PLAY_MODE::CPU)CollisionPlayerCPU(*p, pPos);

	//�v���C���[�U������
	//�U�����Ă��Ȃ� || �U�������łɓ������Ă���

	for (int enemy = 0; enemy < maxCnt; enemy++)
	{
		int pArrowCnt = p->GetArrowCnt(static_cast<int>(PlayerBase::ATK_TYPE::ATTACK));
		for (int arrowCnt = 0; arrowCnt < pArrowCnt; arrowCnt++)
		{
			if (p->GetAtks(PlayerBase::ATK_TYPE::ATTACK).empty())continue;
			auto arrow = p->GetArrowAtk(PlayerBase::ATK_TYPE::ATTACK, arrowCnt);

			if (!arrow.IsAttack() || arrow.isHit_)continue;
			p->SetAtk(arrow);
			//�G�̎擾
			Enemy* e = enmMng_->GetActiveEnemy(enemy);
			//�����蔻��
			//if (col.IsHitArrowAtk(p, *e, arrowCnt)) {
			if (col.IsHitAtk(*p, *e)) {
				//��e
				e->SetDamage(p->GetAtkPow(), p->GetAtk().pow_);
				EffectManager::GetInstance().Play(EffectManager::EFFECT::HIT2, e->GetPos(), Quaternion(), HIT_EFFECT_SIZE, SoundManager::SOUND::NONE);
				//�U������̏I��
				p->SetIsArrowHit(PlayerBase::ATK_TYPE::ATTACK, true, arrowCnt);
			}
		}

	}


	//�A�[�`���[�̃o�t�����������o�t��������
	for (int pl = 0; pl < PlayerManager::PLAYER_NUM; pl++)
	{
		//�����蔻�肷��҂��������g�������ꍇ��������
		if (_p1Num == pl)continue;
		PlayerBase* p2 = playerMng_->GetPlayer(pl);


		int pArrowCnt = p->GetArrowCnt(static_cast<int>(PlayerBase::ATK_TYPE::BUFF));
		for (int arrowCnt = 0; arrowCnt < pArrowCnt; arrowCnt++)
		{
			auto arrow = p->GetArrowAtk(PlayerBase::ATK_TYPE::BUFF, arrowCnt);
			if (!arrow.IsAttack() || arrow.isHit_)continue;
			p->SetAtk(arrow);
			if (col.IsHitAtk(*p, *p2) && !p2->GetIsBuff(PlayerBase::SKILL_BUFF::BUFF_ARROW))
			{
				//�A�[�`���[�̋|�����������瓖�������v���C���[�̔\�͂��グ��
				p->Buff(*p2);
				p2->SetIsBuff(PlayerBase::SKILL_BUFF::BUFF_ARROW, true);
				EffectManager::GetInstance().Play(
					EffectManager::EFFECT::BUFF_ARROW_HIT,
					p2->GetPos(),
					Quaternion(), 
					BUFF_ARROW_EFFECT_SIZE,
					SoundManager::SOUND::NONE);
				//�U������̏I��
				p->SetIsArrowHit(PlayerBase::ATK_TYPE::BUFF, true, arrowCnt);

			}
		}
	}

}

void GameScene::CollisionPlayerSerch(int _p1Num)
{
	//��ԋ߂��G��T��
	//�G�̐����擾
	int enmCnt = enmMng_->GetActiveNum();
	float e2pVecSize[EnemyManager::ENEMY_MAX];
	VECTOR minE2PVec;
	float min = FLT_MAX;
	auto& col = Collision::GetInstance();

		VECTOR pPos = playerMng_->GetPlayer(_p1Num)->GetPos();
		for (int ecnt = 0; ecnt < enmCnt; ecnt++)
		{
			VECTOR ePos1 = enmMng_->GetActiveEnemy(ecnt)->GetPos();

			//�v���C���[�ƓG�Ƃ̋��������߂�
			float e2p = sqrt((ePos1.x - pPos.x) * (ePos1.x - pPos.x) + (ePos1.z - pPos.z) * (ePos1.z - pPos.z));
			e2pVecSize[ecnt] = e2p;
			if (e2pVecSize[ecnt] < min)
			{
				min = e2pVecSize[ecnt];
				minE2PVec = ePos1;
				playerMng_->GetPlayer(_p1Num)->SetTargetPos(minE2PVec);
				if (col.Search(pPos, ePos1, PlayerBase::ARCHER_SEARCH_RANGE))
				{
					playerMng_->GetPlayer(_p1Num)->SetIsSerchArcher(true);
				}
				else
				{
					playerMng_->GetPlayer(_p1Num)->SetIsSerchArcher(false);
				}
			}
		}
	
}

void GameScene::CollisionChicken(void)
{
	auto& col = Collision::GetInstance();

	int chickenNum = chicken_->GetChickenAllNum();

	for (int i = 0; i < chickenNum; i++) {
		auto c = chicken_->GetChicken(i);
		//�j���g��������ł����玟��
		if (!c->IsAlive())continue;

		//�G�̑����擾
		int maxCnt = enmMng_->GetActiveNum();
		//�U������

		for (int r = 0; r < maxCnt; r++)
		{

			//�G�̎擾
			Enemy* e = enmMng_->GetActiveEnemy(r);

			//�G�l�̈ʒu�ƍU�����擾
			VECTOR ePos = e->GetPos();

			//���G
			//�͈͓��ɓ����Ă���Ƃ�
			if (col.Search(ePos, c->GetPos(), e->GetSearchRange())) {
				//�ړ����J�n

				//�{��_��
				e->ChangeSearchState(Enemy::SEARCH_STATE::CHICKEN_FOUND);
				e->SetTargetPos(c->GetPos());
			}
			else if (e->GetSearchState() != Enemy::SEARCH_STATE::PLAYER_FOUND) {
				//�ړ����J�n

				//�܂��T����
				e->ChangeSearchState(Enemy::SEARCH_STATE::CHICKEN_SEARCH);
				e->SetPreTargetPos(c->GetPos());
			}

			//�ʏ��Ԏ� && �U���͈͓��Ƀv���C���[����������U�����J�n
			if (col.Search(ePos, c->GetPos(), e->GetAtkStartRange()) && e->GetState() == Enemy::STATE::NORMAL) {
				//��Ԃ�ύX
				e->ChangeState(Enemy::STATE::ALERT);
			}

			//�U������
			for (int a = 0; a < e->GetAtks().size(); a++)
			{
				//�U�������Z�b�g
				e->SetAtk(e->GetAtks()[a]);

				//�Z�b�g���Ă��������Ƃ��Ă���
				UnitBase::ATK eAtk = e->GetAtk();

				//�A�^�b�N�� && �U�����肪�I�����Ă��Ȃ��Ƃ�������������B����ȊO�͂��Ȃ��̂Ŗ߂�
				if (!(eAtk.IsAttack() && !eAtk.isHit_))continue;

				//�U����������͈� && �v���C���[��������Ă��Ȃ��Ƃ�
				if (col.IsHitAtk(*e, *c))
				{
					//�_���[�W
					c->SetDamage(e->GetAtkPow(),e->GetAtk().pow_);
					//�g�p�����U���𔻒�I����
					e->SetAtksIsHit(a, true);
				}
			}
		}
	}

}

//void GameScene::CollisionPlayerCPU(PlayerBase& _player, const VECTOR& _pPos)
//{
//	//�Փ˔���}�l�[�W���擾
//	auto& col = Collision::GetInstance();
//	//�G�̑����擾
//	int maxCnt = enmMng_->GetActiveNum();
//
//	//�G���T�[�`������
//	_player.SetisEnemySerch(false);
//
//
//	//�G�̌̕��s��
//	for (int i = 0; i < maxCnt; i++)
//	{
//		//�G�̎擾
//		Enemy* e = enmMng_->GetActiveEnemy(i);
//
//		//�G�����S���Ă����珈�����Ȃ�
//		if (!e->IsAlive())continue;
//
//		//�G�l�̈ʒu�ƍU�����擾
//		VECTOR ePos = e->GetPos();
//
//		//�v���C���[�����G
//		if (col.Search(_pPos, ePos, _player.GetSearchRange())
//			&& !_player.GetIsCalledPlayer())
//		{
//			//�G���T�[�`��������Ԃ�
//			_player.SetisEnemySerch(true);
//			_player.SetTargetPos(ePos);
//		}
//
//		if (col.Search(_player.GetPos(), ePos, _player.GetAtkStartRange())
//			&& _player.GetState() == PlayerBase::CPU_STATE::NORMAL
//			&& !_player.GetIsCalledPlayer())
//		{
//			//��Ԃ�ύX
//			_player.ChangeState(PlayerBase::CPU_STATE::ATTACK);
//		}
//	}
//}

void GameScene::Fade(void)
{

	Fader::STATE fState = fader_->GetState();
	switch (fState)
	{
	case Fader::STATE::FADE_IN:
		// ���]��
		if (fader_->IsEnd())	//���]�I��
		{
			// ���]���I��������A�t�F�[�h�����I��
 			fader_->SetFade(Fader::STATE::NONE);
			isFadeInFinish_ = true;
			isPhaseChanging_ = false;
		}
		break;
	case Fader::STATE::FADE_OUT:
		// �Ó]��
		if (fader_->IsEnd())	//�Ó]�I��
		{
			
			isFadeInFinish_ = false;
			//�����̏������t�F�[�Y�J�ڂ��킩��₷���悤�Ȃ�n���ɕς���B
			// �Ó]���疾�]��
			//������x���S�Ó]�̎��Ԃ�p��
			fader_->SetFade(Fader::STATE::FADE_KEEP);

			//�t�F�[�Y���U���g�̏�Ԃ�
			isFazeRezult_ = true;
		}
		break;
	case Fader::STATE::FADE_KEEP:
		UpdatePhase();
		break;
	default:
		break;
	}
}

//�t�F�[�Y�֌W
//*********************************************************
void GameScene::ChangePhase(void)
{
	//BGM�̒�~
	if (fazeCnt_ == LAST_FAZE)SoundManager::GetInstance().Stop(SoundManager::SOUND::GAME_LAST);
	else SoundManager::GetInstance().Stop(SoundManager::SOUND::GAME_NOMAL);

	//���U���g�Ɋ֌W����f�[�^�����
	DataBank& data = DataBank::GetInstance();

	data.Input(DataBank::INFO::FAZE_DUNK_ENEMY, enmMng_->GetDunkCnt());	//�|�����G��
	data.Input(DataBank::INFO::ALIVE_CHICKEN, chicken_->GetAliveNum());		//�j���g��������

	//���U���g�Ŏ擾
	fazeResult_->SetResult();
	if (fazeCnt_ == LAST_FAZE)fazeResult_->SetLast();

	isPhaseChanging_ = true;
	fader_->SetFade(Fader::STATE::FADE_OUT);
}

void GameScene::UpdatePhase(void)
{
	/*phaseCnt_++;
	if (phaseCnt_ > PHASE_TIME)
	{
		fader_->SetFade(Fader::STATE::FADE_IN);
		phaseCnt_ = 0;
	}*/
}

void GameScene::DrawPhase(void)
{
	DrawString(0, 0, "�t�F�[�Y�J�ڒ�", 0xffffff, true);
}
void GameScene::LevelUpReflection()
{
	//�v���C���[���Ƃɋ������f
	int plNum = DataBank::GetInstance().Output(DataBank::INFO::USER_NUM);
	for (int i = 0; i < plNum; i++)
	{
		if (level_->GetPreType(i) != LevelScreenManager::TYPE::MAX)
		{
			level_->EffectSyne(*playerMng_->GetPlayer(i), i);
		}
	}
	//�X�e�[�g�m�F
	if (level_->GetState() != LevelScreenManager::STATE::NONE)
	{
		//�ʏ펞�ȊO�͏������Ȃ�
		return;
	}

	for (int i = 0; i < plNum; i++)
	{
		level_->Reflection(*playerMng_->GetPlayer(i), i);
	}
}

void GameScene::FazeResultUpdate(void)
{
	fazeResult_->Update();

	//���U���g���I�������Ƃ�
	if (fazeResult_->IsEnd())
	{
		//�J�E���g��ŏI�t�F�[�Y�����傫���Ȃ�����N���A�V�[����
		if (fazeCnt_ >= LAST_FAZE) {
			SceneManager::GetInstance().ChangeScene(SceneManager::SCENE_ID::GAMECLEAR);
		}

		level_->AddExp(fazeResult_->GetExp());
		

		//�t�F�[�Y���U���g���I�������̂Ŗ��]�y�у��U���g���Z�b�g�E�^�C�}�[�������EBGM�̍Đ�
		fader_->SetFade(Fader::STATE::FADE_IN);
		Timer::GetInstance().Reset();
		fazeResult_->Reset();
		isFazeRezult_ = false;
		//�ʒm�@�\��ON��
		SetIsInform(true);
		
		//�G�̓���ւ�
		enmMng_->ProcessChangePhase(fazeCnt_);

		//�v���C���[�̃��Z�b�g
		playerMng_->ResetFaze();

		if (fazeCnt_ <= LAST_FAZE)
		{
			if (fazeCnt_ == LAST_FAZE)SoundManager::GetInstance().Play(SoundManager::SOUND::GAME_LAST);
			else SoundManager::GetInstance().Play(SoundManager::SOUND::GAME_NOMAL);
		}
	}
}

void GameScene::InformFazeNum(void)
{
	//�m�点�X�V
	if (!inform_->Update()) {
		SetIsInform(false);
	}

	//�m�点�֌W�̕`�揈�����
}

void GameScene::SetIsInform(const bool _flag)
{
	isInformFaze_ = _flag;
	if (_flag == true)
	{
		fazeCnt_++;		//true=�t�F�[�Y���X�V���̂��߃J�E���g�𑝉�
		inform_->SetFazeCnt(fazeCnt_);
	}
}

bool GameScene::IsGameOver(void)
{
	for (int i = 0; i < PlayerManager::PLAYER_NUM; i++) {
		PlayerBase* p = playerMng_->GetPlayer(i);

		if (p->IsAlive())return false;
	}
	return true;
}