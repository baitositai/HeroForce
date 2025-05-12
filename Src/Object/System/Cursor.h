#pragma once
#include "../../Application.h"
#include "../../Manager/Generic/InputManager.h"
#include "../../Common/Vector2.h"

class Cursor
{
public:

	//�����ʒu
	static constexpr int DEFAULT_POS_X = 150;
	static constexpr int DEFAULT_POS_Y = 150;

	//�v���C���[�ő�l��
	static constexpr int NUM_MAX = 4;

	//�ړ���
	static constexpr int MOVE_POW = 14;

	//���a
	static constexpr int RADIUS = 32;

	//�p�x
	static constexpr float ANGLE = 315.0f;

	//�J�[�\���T�C�Y
	static constexpr float SCALE_RATE = Application::SCREEN_SIZE_X * 1.5f / Application::DEFA_SCREEN_SIZE_X;

	//�L�[����
	struct Key
	{
		int right_;
		int left_;
		int up_;
		int down_;

		int stickX_;
		int stickY_;
		int decide_;
	};

	//�R���X�g���N�^
	Cursor();

	//�f�X�g���N�^
	~Cursor() = default;

	/// <summary>
	/// ������
	/// </summary>
	/// <param name="plNum"></param>�v���C���[�i���o�[
	/// <param name="img"></param>�摜
	void Init(const int _plNum,const int _img);

	//�X�V
	void Update();

	//�`��
	void Draw();

	//�������Z�b�g����
	void Reset();

	//����֘A�̐ݒ�
	void SetControllKey(const int _right, const int _left, const int _up, const int _down, const int _decide);

	//����̐ݒ�
	void SetDecide(const bool _value);

	//���W�Ԃ�
	inline const Vector2 GetPos() const { return pos_; }

	//����ς݂��m�F
	inline const bool IsDecide() const { return decide_; }

private:

	//�v���C���[�i���o�[
	int playerNum_;

	//�摜
	int img_;

	//�g�嗦
	float rate_;

	//���W
	Vector2 pos_;	

	//����
	bool decide_;	

	//�L�[�Ǘ�
	Key key_;	

	//�p�b�h���
	InputManager::JOYPAD_NO pad_;
};