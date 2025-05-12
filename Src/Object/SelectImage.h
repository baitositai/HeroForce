#pragma once
#include <functional>
#include"../Utility/AsoUtility.h"
#include"Common/Transform.h"
#include "../Common/Vector2.h"
#include "./UnitBase.h"
#include "../Scene/SelectScene.h"

//class SelectScene;
class SelectPlayer;

class SelectImage : public UnitBase
{
public:
	//#define DRAW_DEBUG

	//�摜�֘A
	static constexpr float POINT_SCALE = 52.0f;			//���摜�̑傫��
	static constexpr float IMAGE_SCALE = 50.0f;			//�摜�̑傫��

	//���_�֘A�i�S�_)--------------------------------------------------------------

	//�l���I���Ȃǂ̕\���Ɏg�����b�V��
	static constexpr int VERTEX_NUM = 4;			//���_��
	static constexpr float VERTEX_ROTSPEED = 1.0f;	//���_����]�����鑬�x

	static constexpr float VERTEX_LEFT_X = -25.0f;	//�摜(���_)����X���W
	static constexpr float VERTEX_RIGHT_X = 25.0f;	//�摜(���_)�E��X���W
	
	static constexpr float VERTEX_TOP_Y = 150.0f;	//�摜���Y���W	
	static constexpr float VERTEX_UNDER_Y = 90.0f;	//�摜����Y���W 

	static constexpr float VERTEX_Z = -400.0f;		//�摜���Z���W
	static constexpr float VERTEX_UNDER_Z = -392.0f;//�摜����Z���W

	//���(���̂ق��͂��̂܂܂ŉE�̏ꍇ��LEFT��RIGHT�����ւ��ă}�C�i�X�l��������)
	static constexpr float POINT_LEFT_X = -50.0f;	//�摜(���_)����X���W
	static constexpr float POINT_RIGHT_X = -28.0f;	//�摜(���_)�E��X���W
	static constexpr float POINT_TOP_Y = 130.0f;	//�摜(���_)����Y���W
	static constexpr float POINT_UNDER_Y = 110.0f;	//�摜(���_)���Y���W
													
	static constexpr float POINT_TOP_Z = VERTEX_Z;	//�摜���Z���W
	static constexpr float POINT_UNDER_Z = -402.0f;	//�摜����Z���W

	static constexpr float ROLE_MESH_LEFT_X = -55.0f;
	static constexpr float ROLE_MESH_RIGHT_X = 15.0f;

	static constexpr float ROLE_MESH_TOP_Z = 170.0f;
	static constexpr float ROLE_MESH_UNDER_Z = 100.0f;

	static constexpr int BLEND_PARAM = 128;			//�u�����h���[�h�̋���

	//�L�[���͊֘A
	static constexpr float SELECT_TIME = 1.0f;		//�L�[�����o�ߎ���
	static constexpr float INTERVAL_TIME = 0.6f;	//�C���^�[�o�����

	//�\����-----------------------------------------------------------------------
	
	//���b�V��
	struct Mesh {
		VERTEX3D vertex_[VERTEX_NUM];	//���_���

		//������
		Mesh() : vertex_(){}

		/// <summary>
		/// ���b�V����`�悷��
		/// </summary>
		/// <param name="handle">�摜�n���h��</param>
		void DrawTwoMesh(int handle);		
	};

	//���
	struct Point {
		bool isToggle_;	//�I���A�I�t�̐؂�ւ��p
		Mesh mesh_;		//���p�̃��b�V��

		//������
		Point() : isToggle_(false),mesh_() {}
		Point(bool isT,Mesh& mesh) :
			 isToggle_(isT) , mesh_(mesh)  {}
	};

	//------------------------------------------------------------------------------

	//�R���X�g���N�^
	SelectImage(SelectScene& select, std::shared_ptr<SelectPlayer> player);

	//�f�X�g���N�^
	~SelectImage(void) = default;

	//���
	virtual void Destroy(void);

	//������
	virtual void Init(void)override;
	//�X�V
	virtual void Update(void)override;
	//�`��
	virtual void Draw(void)override;

	//��E�I���̍ۂɕ\�����郁�b�V���̍��W�ֈړ�������
	void MoveVertexPos(void);
	void ReductinVertexPos(void);	//ComingSoon�p�̏k���p

	/// <summary>
	/// �I�����Ă���I�u�W�F�N�g��ύX����
	/// </summary>
	/// <param name="input">���̓f�o�C�X</param>
	/// <param name="i">���̓f�o�C�X�ɉ������v���C���[�i���o�[</param>
	void ChangeObject(SelectScene::Device& input,int i);

	/// <summary>
	/// ��ԑJ��
	/// </summary>
	/// <param name="_state">�J�ڂ�����</param>
	void ChangeSelect(const SelectScene::SELECT _state);

	//�Q�b�^�[�@---------------------------------------------------

	//�I��ł����E���擾
	int GetRole(void) { return role_; };	

	/// <summary>
	/// ���b�V���̒��_�����擾
	/// </summary>
	/// <param name="i">vertex�z��̎w��</param>
	/// <returns>�w�肵��vertex�̒��_���</returns>
	VERTEX3D GetMeshVertex(int i){ return mesh_.vertex_[i]; };
	VERTEX3D GetReadyMeshVertex(int i) { return readyMesh_.vertex_[i]; };
	VERTEX3D GetPointLMeshVertex(int i){ return pointL_.mesh_.vertex_[i]; };
	VERTEX3D GetPointRMeshVertex(int i){ return pointR_.mesh_.vertex_[i]; };

	//�����������ǂ�������
	bool GetReady(void) { return isReady_; };

	// �Z�b�^�[�@--------------------------------------------------

	/// <summary>
	/// ���b�V�����W��ݒ肷��
	/// </summary>
	/// <param name="pos">�ݒ肷����W</param>
	/// <param name="i">�ݒ肷��vertex�z��̎w��</param>
	void RotMeshPos(VECTOR pos, int i) { mesh_.vertex_[i].pos = pos; }
	void RotReadyMeshPos(VECTOR pos, int i) { readyMesh_.vertex_[i].pos = pos; }
	void RotPointLMeshPos(VECTOR pos, int i) { pointL_.mesh_.vertex_[i].pos = pos; }
	void RotPointRMeshPos(VECTOR pos, int i) { pointR_.mesh_.vertex_[i].pos = pos; }

private:

	//��ԊǗ�(�X�V�X�e�b�v)
	std::function<void(void)> stateUpdate_;
	//��ԊǗ�(��ԑJ�ڎ���������)
	std::map<SelectScene::SELECT, std::function<void(void)>> stateChanges_;

	//�I�𒆂̎��
	SelectScene::SELECT state_;

	//���b�V��
	Mesh mesh_;			//�l���I���Ƒ���I���̎��̃��b�V��
	Mesh readyMesh_;	//���������̎��̃��b�V��
	
	//�摜�n���h��
	int* imgPlayerNum_;		//�l���I���摜
	int* imgDisplayNum_;	//�f�B�X�v���C���I���摜
	int* imgLeftPoint_;		//�������̖��摜
	int* imgRightPoint_;	//�E�����̖��摜
	int* imgReady_;			//���������摜
	int* imgRoleNum_;		//��E�̉摜
	int* imgDeviceNum_;		//��E�̉摜
	int* imgComingSoon_;	//ComingSoon�̉摜

	//���b�V���̒��_���W�p�i4�̒��_�j
	VECTOR leftTop_;		//����
	VECTOR leftBottom_;		//����
	VECTOR rightTop_;		//�E��	
	VECTOR rightBottom_;	//�E��
	float angle_;		

	//���̍\����
	Point pointL_;		//��
	Point pointR_;		//�E

	//�f�B�X�v���C��
	int displayNum_;

	//�v���C���[�l��
	int playerNum_;

	//1P�̓��̓^�C�v
	bool isPad_;	

	//�E��
	int role_;

	//�����I�b�P�[���ǂ���
	bool isReady_;

	//�L�[�����b�����Ă��邩
	float keyPressTime_;
	
	//�L�[�̔�����P�񂾂��擾����p
	bool press_;

	//�l�������Ԋu�ŉ��Z���Ă������߂̃C���^�[�o���p����(���Z���Ď����Z����܂ł̊�)
	float interval_;

	//�C���X�^���X
	SelectScene& selectScene_;				//�Z���N�g�V�[��
	std::shared_ptr<SelectPlayer> player_;	//�Z���N�g�V�[���p�̃v���C���[

	//�J�����p�̃^�[�Q�b�g���W
	VECTOR target_[SceneManager::PLAYER_NUM];

	//�֐�-------------------------------------------------------------------------------------

	//�ǂݍ��ݗp
	void Load(void);	

	//���_���W������
	void InitVertex(void);

	//���_���W��]�p
	VECTOR RotateVertex(VECTOR pos, VECTOR center, float angle);

	//��ԑJ��
	void ChangeStateDisplay(void);
	void ChangeStateNumber(void);
	void ChangeStateOperation(void);
	void ChangeStateRole(void);

	//�X�V�����֘A-----------------------------------------------

	void DisplayUpdate(void);		//�f�B�X�v���C���I�𒆂̏���

	void NumberUpdate(void);		//�l���I�𒆂̏���

	void OperationUpdate(void);		//������@�I�𒆂̏���(1P�̂�)

	void RoleUpdate(void);			//��E�I�𒆂̏���

	//�`�揈���֘A--------------------------------------------

	void DisplayDraw(void);			//�f�B�X�v���C���I�𒆂̏���

	void NumberDraw(void);			//�l���I�𒆂̏���

	void OperationDraw(void);		//������@�I�𒆂̏���(1P�̂�)

	void RoleDraw(void);			//��E�I�𒆂̏���

	void PointsDraw(void);			//���i�Q�Ƃ��j�`��

	//-----------------------------------------------------------
};