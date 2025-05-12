#pragma once
#include <functional>
#include "SceneBase.h"
#include "../Object/Common/Transform.h"
#include "../Manager/Generic/SceneManager.h"
#include "../Common/Vector2.h"

class SkyDome;
class StageManager;
class SelectPlayer;
class SelectEnemy;
class SelectImage;

class SelectScene :public SceneBase
{
public:

	//#define DEBUG_RECT

	//�O�p�`�p�̒萔
	static constexpr int TRI_SCALE = 150;	//�傫��
	static constexpr int TRI_POS_X = Application::SCREEN_SIZE_X / 2;	//X���W
	static constexpr int TRI_POS_Y = Application::SCREEN_SIZE_Y / 2;	//Y���W

	//�l�p�`�̑傫��
	static constexpr int RECT_SCALE = 300;

	//�}�`���m�̊Ԋu
	static constexpr int PRI_SPACE = 100;

	static constexpr int PLAYER_NUM = SceneManager::PLAYER_NUM;
	static constexpr int ROLE_NUM = SceneManager::PLAYER_NUM;

	//�J�����֘A
	static constexpr VECTOR DEFAULT_CAMERA_POS = { 0.0f, 100.0f, -500.0f };		//�J�����̍��W
	static constexpr VECTOR DEFAULT_TARGET_POS = { 0.0f, 150.0f, -100.0f };		//�J�����̒����_���W

	static constexpr VECTOR DEFAULT_TARGET_TWO = { 400.0f, 150.0f, 0.0f };		//�J�����̒����_���W
	static constexpr VECTOR DEFAULT_TARGET_THREE = { 0.0f, 150.0f, 100.0f };	//�J�����̒����_���W
	static constexpr VECTOR DEFAULT_TARGET_FOUR = { -100.0f, 150.0f, -100.0f };	//�J�����̒����_���W

	static constexpr float CHARACTER_SCALE = 0.5f;

	//�I�����Ă�����
	enum class SELECT 
	{
		DISPLAY,		//�f�B�X�v���C��
		NUMBER,		//�l��
		OPERATION,	//1P���L�[�{�[�h����ɂ��邩�ǂ���
		ROLE,		//��E
		MAX
	};

	// �L�[�R���t�B�O
	enum class  KEY_CONFIG
	{
		NONE,
		UP,
		DOWN,
		LEFT,
		RIGHT,

		UP_TRG,
		DOWN_TRG,
		LEFT_TRG,
		RIGHT_TRG,

		DECIDE,
		CANCEL
	};

	//�f�o�C�X���
	struct Device
	{
		SceneManager::CNTL cntl_;	//���͂���f�o�C�X
		KEY_CONFIG config_;			//�L�[�R���t�B�O
	};	

	// �R���X�g���N�^
	SelectScene(void);

	// �f�X�g���N�^
	~SelectScene(void);

	virtual void Init(void) override;
	virtual void Update(void) override;
	virtual void Draw(void) override;
	virtual void Release(void) override;

	/// <summary>
	/// ��ԑJ��
	/// </summary>
	/// <param name="_state">�J�ڂ�����</param>
	void ChangeSelect(const SELECT _state);

	////�I���������(�l��or��E)�̎�ނ�ς���
	//void ChangeSelect(SELECT select);

	//�L�[���͂ƃR���g���[�����͂����ʉ�
	void KeyConfigSetting(void);

	//�L�[�{�[�h����
	void KeyBoardProcess(void);
	//�p�b�h����
	void PadProcess(void);

	//���̓f�o�C�X�ύX(����������Ƃ����������@�����肻��)
	void Change1PDevice(SceneManager::CNTL cntl);

	//�L�[���͂ƃp�b�h���͂̐���
	void ControllDevice(void);

	//�Q�b�^�[	----------------------------------------------------------

	SceneManager::CNTL Get1PDevice(void) { return input_[0].cntl_; };		//1P�̓��̓f�o�C�X���擾����

	KEY_CONFIG GetConfig(void);					//���̓L�[���擾

	SELECT GetSelect(void) { return select_; }	//���݂̑I���t�F�[�Y���擾

	bool IsAllReady(void);

	//�Z�b�^�[ -----------------------------------------------------------

	/// <summary>
	/// 1P�̓��̓f�o�C�X��ݒ肷��
	/// </summary>
	/// <param name="cntl">�f�o�C�X�̎��</param>
	void Set1PDevice(SceneManager::CNTL cntl){ input_[0].cntl_ = cntl; };

	//�f�o�b�O�֘A--------------------------------------------------------

	void DrawDebug(void);	//�f�o�b�O�`��

	//--------------------------------------------------------------------

private:

	//��ԊǗ�(�X�V�X�e�b�v)
	std::function<void(void)> stateUpdate_;
	//��ԊǗ�(��ԑJ�ڎ���������)
	std::map<SELECT, std::function<void(void)>> stateChanges_;

	//�X�J�C�h�[��
	std::unique_ptr<SkyDome> skyDome_;

	//�v���C���[
	std::shared_ptr<SelectPlayer>players_[SceneManager::PLAYER_NUM];

	// �摜
	std::unique_ptr<SelectImage>images_[SceneManager::PLAYER_NUM];

	//�v���C���[
	std::shared_ptr<SelectEnemy>enemys_[SceneManager::PLAYER_NUM];

	//�w�i�̃X�e�[�W
	StageManager* stage_;

	//�I�𒆂̎��
	SELECT select_;

	//�L�[�R���t�B�O
	KEY_CONFIG key_;

	//���ꂼ��̃v���C���[�̃f�o�C�X�Ɠ���
	Device input_[SceneManager::PLAYER_NUM];

	int imgDisplay_;
	int imgPlayer_;
	int imgOperation_;
	int imgRole_;
	int imgWait_;

	//��ԑJ��
	void ChangeStateDisplay(void);
	void ChangeStateNumber(void);
	void ChangeStateOperation(void);
	void ChangeStateRole(void);
	void ChangeStateMax(void);

	//�X�V�����֘A-----------------------------------------------

	void DisplayUpdate(void);		//�f�B�X�v���C���I�𒆂̏���

	void NumberUpdate(void);		//�l���I�𒆂̏���

	void OperationUpdate(void);		//������@�I�𒆂̏���(1P�̂�)

	void RoleUpdate(void);			//��E�I�𒆂̏���

	void MaxUpdate(void);			//��

	//�`�揈���֘A-----------------------------------------------

	void DisplayDraw(void);			//�f�B�X�v���C���I�𒆂̏���

	void NumberDraw(void);			//�l���I�𒆂̏���

	void OperationDraw(void);		//������@�I�𒆂̏���(1P�̂�)

	void RoleDraw(void);			//��E�I�𒆂̏���

	void MaxDraw(void);				//��

	//-----------------------------------------------------------

	//�f�o�b�O�֘A-------------------------------
	int readyNum;
	int okNum;
	bool isOk_[SceneManager::PLAYER_NUM];
};

