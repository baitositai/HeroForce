#include<cassert>
#include<random>
#include"../Manager/Generic/SceneManager.h"

#include"EnemySort/Enemy.h"
#include"EnemySort/EneArcher.h"
#include"EnemySort/EneAxe.h"
#include"EnemySort/EneBrig.h"
#include"EnemySort/EneGolem.h"
#include"EnemySort/EneMage.h"

#include "EnemyManager.h"

EnemyManager::EnemyManager(std::vector<VECTOR> _pos) : createPos_(_pos)
{
	for (int i = 0; i < ENEMY_MAX; i++)
	{
		activeEnemys_[i] = nullptr;
		activeEnemysType_[i] = TYPE::MAX;
	}

	for (int a = 0; a < static_cast<int>(TYPE::MAX); a++)
	{
		activeTypeNum_[a] = 0;
	}

	allExp_ = 0.0f;

	activeNum_ = 0;

	createIntCnt_ = 0.0f;
}

void EnemyManager::Init(void)
{
	for (int a = 0; a < static_cast<int>(TYPE::MAX); a++)
	{
		activeTypeNum_[a] = 0;
	}

	dunkCnt_ = 0;

	allExp_ = 0.0f;

	activeNum_ = 0;

	createIntCnt_ = 0.0f;

	ProcessChangePhase(1);
}

void EnemyManager::Update(void)
{
	//�J�E���^
	createIntCnt_ = createIntCnt_ + SceneManager::GetInstance().GetDeltaTime();

	//�G�̐�������
	if (createIntCnt_ >= CREATE_INTERVAL)
	{
		//�Ԋu�J�E���^������
		createIntCnt_ = 0.0f;

		//�G�̐���
		CreateEnemy();
	}

	//�������Ă���G�̏���
	for (int i = 0; i < activeNum_; i++)
	{
		//���S����
		if (!activeEnemys_[i]->IsAlive() 
			&& activeEnemys_[i]->IsFinishAnim(Enemy::ANIM::DEATH))
		{

			//�G�폜
			DeathEnemy(i);
			break;
		}

		//activeEnemys_[i]->SetTargetPos(_target);
		activeEnemys_[i]->Update();
	}
}

void EnemyManager::Draw(void)
{
	for (int i = 0; i < activeNum_; i++)activeEnemys_[i]->Draw();
}

void EnemyManager::Release(void)
{
	
}

void EnemyManager::CollisionStage(const Transform& stageTrans)
{
	auto& col = Collision::GetInstance();

	for (int i = 0; i < activeNum_; i++)
	{
		if (col.IsHitUnitStageObject(stageTrans.modelId, activeEnemys_[i]->GetTransform().pos, activeEnemys_[i]->GetRadius()))
		{
			activeEnemys_[i]->SetPrePos();

			//�ړ����
			activeEnemys_[i]->KeepCollStageDistance();
		}
	}
}



void EnemyManager::CreateEnemy(void)
{
	//�G���ő吔�����琶���������s��Ȃ�
	if (activeNum_ >= ENEMY_MAX)return;

	Enemy* enm = nullptr;

	//�����Ŏ�ތ��߂�
	TYPE type;
	
	//�S�[�����ȊO��
	do
	{
		type = static_cast<TYPE>(GetRand(static_cast<int>(TYPE::MAX) - 1));
	} while (type == TYPE::GOLEM || activeTypeNum_[static_cast<int>(type)] >= ONETYPE_MAX);

	//�������΍��W
	VECTOR createLocalPos = createPos_[GetRand(createPos_.size() - 1)];
	createLocalPos.x += activeNum_ * Enemy::COL_RADIUS;
	createLocalPos.y = 0.0f;

	//�C���X�^���X����
	switch (type)
	{
	case EnemyManager::TYPE::ARCHER:
		enm = new EneArcher(createLocalPos);
		break;
	case EnemyManager::TYPE::AXE:
		enm = new EneAxe(createLocalPos);
		break;
	case EnemyManager::TYPE::BRIG:
		enm = new EneBrig(createLocalPos);
		break;
	case EnemyManager::TYPE::GOLEM:
		//�S�[�����̓{�X�L�����Ȃ̂ł����ł͐������Ȃ�
		return;
		break;
	case EnemyManager::TYPE::MAGE:
		enm = new EneMage(createLocalPos);
		break;
	default:
		return;
		break;
	}

	//�O�̂��߂̃G���[���p
	if (enm == nullptr)assert("�G�̐����Ŗ�肪����܂����B");

	//�G�̏�����
	enm->Init();

	//�G�̍X�V�����|�������Z�b�g
	activeEnemys_[activeNum_] = enm;

	//�G�̎�ނ�ۑ�
	activeEnemysType_[activeNum_] = type;

	//�J�E���^����
	activeTypeNum_[static_cast<int>(activeEnemysType_[activeNum_])]++;
	activeNum_++;
}

void EnemyManager::CreateBoss(void)
{
	//�G���ő吔�����琶���������s��Ȃ�
	if (activeNum_ >= ENEMY_MAX)return;

	//�G�̐���
	Enemy* enm = nullptr;

	//�������΍��W
	VECTOR createLocalPos = createPos_[1];

	//�S�[�����̐���
	enm = new EneGolem(createLocalPos);

	//�O�̂��߂̃G���[���p
	if (enm == nullptr)assert("�G�̐����Ŗ�肪����܂����B");

	//�G�̏�����
	enm->Init();

	//�G�̍X�V�����|�������Z�b�g
	activeEnemys_[activeNum_] = enm;

	//�G�̎�ނ�ۑ�
	activeEnemysType_[activeNum_] = TYPE::GOLEM;

	//�J�E���^����
	activeTypeNum_[static_cast<int>(activeEnemysType_[activeNum_])]++;
	activeNum_++;
}

VECTOR EnemyManager::GetNotOverlappingPos(void)
{
	//�G�̑傫��
	float eneSize;
	eneSize = Enemy::COL_RADIUS;

	//��������
	bool isGenelateEnemy = false;

	//���������
	int challengeCnt = 0;

	//�������W
	VECTOR createPos = AsoUtility::VECTOR_ZERO;//createPos_[GetRand(createPos_.size() - 1)];

	//�������΍��W
	VECTOR ret = AsoUtility::VECTOR_ZERO;

	//�����ł���܂ŌJ��Ԃ�
	while (!isGenelateEnemy)
	{
 		if (challengeCnt == ENEMY_CREATE_CHALLENGE_LIMIT)
			assert("�G�̐�������񐔏���ɒB�������߃t���[�Y�Ɣ��f���܂���");

		//�J�E���^
		challengeCnt++;

		//�������W
		VECTOR createPos = createPos_[GetRand(createPos_.size() - 1)];

		//�~�͈͂̒��̈�_�����
		GetRandomPointInCircle(createPos, GENELATE_RADIUS, ret);

		//�����ꏊ������Ă��Ȃ���
		if (!IsOverlap(ret, eneSize * 2))
		{
			//����Ă��Ȃ�����

			//��������
			isGenelateEnemy = true;
		}
	}

	return ret;
}

void EnemyManager::GetRandomPointInCircle(VECTOR _myPos, const int _r, VECTOR& _tPos)
{
	// �����_���G���W���𐶐�
	std::random_device rd;
	std::mt19937 gen(rd());

	//�����͈̔�(0 �` 2��)
	std::uniform_real_distribution<> dis(0, 2 * DX_PI_F);

	//�����͈̔�(0 �` 1)
	std::uniform_int_distribution<> create(0, createPos_.size() - 1);

	// �����_���Ȋp�xtheta
	float theta = dis(gen);

	// �����_���Ȕ��ar' (0 �` r) �ŁA�ϓ��ɕ��z����悤�� sqrt ���g��
	float radius = sqrt(static_cast<float>(rand()) / RAND_MAX) * _r;

	// �~���̓_���v�Z
	_tPos.x = static_cast<int>(_myPos.x + radius * cos(theta));
	_tPos.z = static_cast<int>(_myPos.z + radius * sin(theta));

	//�o���ʒu�̎w�肵�Ȃ���
	_myPos = createPos_[create(gen)];
}

bool EnemyManager::IsOverlap(VECTOR& _tPos, float _minDist)
{
	//1�̂�����Ă��Ȃ��Ȃ画�肵�Ȃ�
	if (activeNum_ <= 0)
		return false;

	for (const auto& enemy : activeEnemys_) {
		if (enemy == nullptr)
			continue;

		int dx = _tPos.x - enemy->GetPos().x;
		int dz = _tPos.z - enemy->GetPos().z;
		double distance = std::sqrt(dx * dx + dz * dz);
		if (distance < _minDist) {
			return true; // �d�Ȃ��Ă���ꍇ
		}
	}
	return false; // �d�Ȃ��Ă��Ȃ��ꍇ
}

void EnemyManager::DeleteAllEnemy(void)
{
	for (int i = activeNum_ - 1; i >= 0;i--) {
		if (activeEnemys_[i] == nullptr)
			continue;

		//�G�����ׂč폜
		activeEnemys_[i]->Destroy();
		delete activeEnemys_[i];
		activeEnemys_[i] = nullptr;
		activeTypeNum_[static_cast<int>(activeEnemysType_[i])]--;
		activeEnemysType_[i] = TYPE::MAX;
		activeNum_--;
	}
}

void EnemyManager::DeathEnemy(int _num)
{
	//�|���ꂽ�G�̌o���l��ۑ�
	allExp_ += activeEnemys_[_num]->GetExp();
	dunkCnt_++;

	//�|���ꂽ�G�̏���
	activeEnemys_[_num]->Destroy();
	delete activeEnemys_[_num];

	//�����̌���
	//���̎��_��activeNum_�͔z��̖����̔ԍ��������悤�ɂȂ�B
	activeNum_--;
	activeTypeNum_[static_cast<int>(activeEnemysType_[_num])]--;

	//�L���ȓG�̎�ނ̏�����
	activeEnemysType_[_num] = TYPE::MAX;

	//�z��̋󂫂𖄂߂邽�߂̃\�[�g
	//�z��̖����̕����󂫂Ɉړ�������
	//�|���ꂽ�G�������̕��Ȃ珈���͂��Ȃ�
	if (_num == activeNum_)return;

	//�}������
	//delete������ƈڍs���ꂽ���̏���������̂�nullptr�ݒ�̂݁@�ړ����@��std::move�ł�����
	activeEnemys_[_num] = activeEnemys_[activeNum_];
	activeEnemys_[activeNum_] = nullptr;
	activeEnemysType_[_num] = activeEnemysType_[activeNum_];
}

void EnemyManager::ProcessChangePhase(const int _phase)
{
	//�G�̍폜
	DeleteAllEnemy();
	dunkCnt_ = 0;

	//�G�쐬��
	int createNum = 0;

	//�t�F�[�Y�ɂ���č쐬����G�̐���ς���
	if (_phase == 1)
		createNum = PHASE_ONE_INIT_CREATE_ENEMY;
	else if (_phase == 2)
		createNum = PHASE_TWO_INIT_CREATE_ENEMY;
	else if (_phase == 3)
	{
		//�{�X�̍쐬
		CreateBoss();
		return;
	}
	else 
		return;

	for (int i = 0; i < createNum; i++)
	{
		//�G�̏�������
		CreateEnemy();
	}
 }