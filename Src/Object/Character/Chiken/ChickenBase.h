#pragma once
#include <functional>
#include <memory>
#include <map>
#include "../../../Application.h"
#include "../../Common/Transform.h"
#include "../../../Utility/AsoUtility.h"
#include "../../../Manager/Decoration/EffectManager.h"
#include "../../../Manager/Decoration/SoundManager.h"
#include "../../../Manager/Generic/ResourceManager.h"
#include "../../../Manager/Generic/SceneManager.h"
#include "../../../Manager/Generic/InputManager.h"
#include "../../../Manager/Generic/SceneManager.h"
#include "../../System/GameUi/CpuHpBar.h"
#include "../../UnitBase.h"

class CpuHpBar;

class ChickenBase : public UnitBase
{
public:	
	
	//���
	enum class STATE
	{
		NONE,	//�Ȃ�
		ALIVE,	//����
		DAMAGE, //�_���[�W
		DEATH,	//���j
		END		//�I��
	};

	//�������̍s�����
	enum class ALIVE_MOVE
	{
		IDLE,		//�����~�܂�
		ROTATION,	//���̏�𓦂����
		CALL,		//�������Ă�
		MAX		
	};

	//�L�����N�^�[�T�C�Y
	static constexpr VECTOR SCALE = { 1.3f,1.3f, 1.3f };

	//�A�j���[�V�������x
	static constexpr float DEFAULT_SPEED_ANIM = 50.0f;
	
	//�A�j���[�V�����ԍ�
	static constexpr int ANIM_IDLE = 1;
	static constexpr int ANIM_WALK = 10;
	static constexpr int ANIM_DAMAGE = 9;
	static constexpr int ANIM_DEATH =11;
	static constexpr int ANIM_CALL = 6;

	//������Ԏ��̏�Ԏ��
	static constexpr int ALIVE_MOVE_MAX = static_cast<int>(ALIVE_MOVE::MAX);

	//�Փ˔���p���̔��a
	static constexpr float RADIUS = 50.0f;

	//�t�F�[�h����
	static constexpr float TIME_FADE = 4.0f;

	//�t�F�[�h�J�n�E�I���F
	static constexpr COLOR_F FADE_C_FROM = { 1.0f, 1.0f, 1.0f, 1.0f };
	static constexpr COLOR_F FADE_C_TO = { 0.8f, 0.1f, 0.1f, 0.0f };

	//HELP�摜�\������
	static constexpr float IS_HELP_CNT = static_cast<float>(3 * Application::DEFAULT_FPS);

	//HELP���Έʒu
	static constexpr VECTOR LOCAL_HELP_POS = { 0,180,0 };

	//HP�`��
	static constexpr VECTOR LOCAL_HP_POS = { 0, 120, 0 };

	//�����[�J�����W
	static constexpr VECTOR LOCAL_SMOKE_POS = { 0,40,0 };

	//���X�s�[�h
	static constexpr float SMOKE_SPEED = 20.0f;

	//���T�C�Y
	static constexpr float SMOKE_SCALE = 100.0f;

	//���A�j���[�V������
	static constexpr int SMOKE_NUM = ResourceManager::SMOKE_NUM_X * ResourceManager::SMOKE_NUM_Y;

	//�_���[�W�G�t�F�N�g
	static constexpr float DAMAGE_EFE_SCALE = 30.0f;

	//�G�t�F�N�g�̐����͈�
	static constexpr VECTOR EFC_CREATE_RANGE = { 50, 50, 0 };

	//�����S�ʒu
	static constexpr float SMOKE_CENTER_POS = 0.5f;

	//�R���X�g���N�^
	ChickenBase();

	//�f�X�g���N�^
	~ChickenBase();

	/// <summary>
	/// ����
	/// </summary>
	/// <param name="pos"></param>�����ʒu
	virtual void Create(const VECTOR& _pos);

	//�X�V
	virtual void Update()override;

	//�`��
	void Draw()override;
	
	//�摜�\���̐ݒ�
	void SetIsHelp();

	/// <summary>
	/// �^�[�Q�b�g�̍��W���󂯎��
	/// </summary>
	/// <param name="pos"></param>�^�[�Q�b�g�ʒu
	inline void SetTarget(const VECTOR _pos) { targetPos_ = _pos; }

	//��Ԃ�Ԃ�
	inline const STATE GetState() const { return state_; }

protected:

	//�摜
	int imgHelp_;
	int* imgSmoke_;

	//�ړ��X�s�[�h
	//float moveSpeed_;

	//�t�F�[�h�p�X�e�b�v
	float fadeStep_;

	//�摜�̕\��
	bool isHelp_;
	float isHelpCnt_;

	//�^�[�Q�b�g�p���
	VECTOR targetPos_;

	//���
	STATE state_;

	//�������̏��
	ALIVE_MOVE aliveState_;

	//���A�j���[�V����
	int smokeNum_;
	float smokeStep_;
	float efeSpeed_;
	
	// ��ԊǗ�(��ԑJ�ڎ���������)
	std::map<STATE, std::function<void()>> stateChanges_;

	std::map<ALIVE_MOVE, std::function<void()>> aliveChanges_;

	// ��ԊǗ�(�X�V�X�e�b�v)
	std::function<void()> stateUpdate_;

	// ��ԊǗ�(�`��)
	std::function<void()> stateDraw_;

	// ��������ԊǗ�
	std::function<void()> stateAliveUpdate_;

	//UI�C���X�^���X����
	std::unique_ptr<CpuHpBar> hpUi_;

	//���f���ݒ�
	void ModelSet();

	//���\�[�X�̓ǂݍ���
	void Load();

	//�p�����[�^�[�̐ݒ�
	virtual void SetParam();

	//�A�j���[�V�����ԍ��̏�����
	virtual void InitAnimNum(void);

	//UI�ݒ�
	void SetUiParam();

	//��ԕύX
	void ChangeState(const STATE _state);
	void ChangeStateNone();
	void ChangeStateAlive();
	void ChangeStateDamage();
	void ChangeStateDeath();
	void ChangeStateEnd();

	void ChangeAliveState(const ALIVE_MOVE _state);
	void ChangeAliveIdle();
	void ChangeAliveWalk();
	void ChangeAliveCall();

	//��ԕʍX�V
	void UpdateNone();
	void UpdateAlive();
	void UpdateDamage();
	void UpdateDeath();
	void UpdateEnd();

	//��ԕʕ`��
	void DrawNone();
	void DrawAlive();
	void DrawDamage();
	void DrawDeath();
	void DrawEnd();

	//������Ԃł̏�ԕʍX�V
	void AliveIdle();
	void AliveRotation();
	void AliveCall();

	//�^�[�Q�b�g�����鏈��
	void LookTarget();

	//�A�j���[�V�����̏I������
	void FinishAnim() override;

	//�摜�̕\���m�F
	void CheckIsHelp();

	//�w���v�`��
	void DrawHelp();

	//�f�o�b�O
	void DebagUpdate();
	void DebagDraw();
};

