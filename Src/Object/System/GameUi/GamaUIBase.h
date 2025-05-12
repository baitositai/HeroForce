#pragma once
#include <DxLib.h>

class GamaUIBase
{
public:

	//HP�o�[�̎��
	enum class HPBAR_TYPE
	{
		PLAYER,
		ENEMY,
		CPU,
		MAX
	};

	//HP�o�[��ޗ�
	static constexpr int HP_BAR_MAX = static_cast<int>(HPBAR_TYPE::MAX);

	//HP�o�[�T�C�Y
	static constexpr int HP_BAR_SIZE_X = 60;
	static constexpr int HP_BAR_SIZE_Y = 17;

	//HP�Q�[�W�T�C�Y
	static constexpr int HP_GAGE_SIZE_X = 120;
	static constexpr int HP_GAGE_SIZE_Y = 45;

	//�R���X�g���N�^
	GamaUIBase();

	//�f�X�g���N�^
	~GamaUIBase() = default;

	//������
	virtual void Init();

	//�`��
	virtual void Draw();

	/// <summary>
	/// ���W�ݒ�
	/// </summary>
	/// <param name="_pos"></param>���W
	inline void SetPos(const VECTOR _pos) { pos_ = _pos; }

	/// <summary>
	/// �̗͐ݒ�
	/// </summary>
	/// <param name="_hp"></param>�̗�
	inline void SetHP(const int _hp) { hp_ = _hp; if (hp_ <= 0) { hp_ = 0; }}

protected:

	//�摜
	int imgHpGage_;
	int imgHpBar_;

	//HP�o�[���
	HPBAR_TYPE typeHpBar_;

	//���W
	VECTOR pos_;

	//HP
	int hp_;	
	
	//�ǂݍ���
	virtual void Load();

};

