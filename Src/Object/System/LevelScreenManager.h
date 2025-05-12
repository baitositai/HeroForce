#pragma once
#include <memory>
#include <functional>
#include <vector>
#include "../Character/PlayableChara/PlayerBase.h"
#include "../../Common/Fader.h"

class LevelSelect;
class LevelNotice;
class Fader;

class LevelScreenManager
{
public:

	//��ԊǗ�
	enum class STATE
	{
		NONE,		//�Ȃ�
		NOTICE,		//�ʒm
		SELECT,		//�I��
		END			//�I��
	};	

	//�����v�f�̎��
	enum class TYPE
	{
		ATTACK,		//�U���͂̏㏸
		DEFENSE,	//�h��͂̏㏸
		SPEED,		//�ړ����x�̏㏸
		LIFE,		//�ő�̗͂̏㏸
		MAX
	};

	//�������x��
	static constexpr int DEFAULT_LEVEL = 1;

	//�o���l�l�����x
	static constexpr float EXP_SPEED = 5.0f;
	
	//�Q�[�W�ő�l
	static constexpr float CONSTANT_GAGE = 1000.f;

	//�Q�[�WUI�g�嗦
	static constexpr float GAGE_SCALE_RATE = 1.0f;
	static constexpr float NUM_SCALE_RATE = 0.75f;

	//�Q�[�W�摜�T�C�Y
	static constexpr int GAGE_IMG_SIZE = static_cast<int>(128 * GAGE_SCALE_RATE);

	//�A���t�@�l�ő�
	static constexpr float ALPHA_MAX = 150.0f;

	//���߃X�s�[�h
	static constexpr float ALPHA_SPEED = 3.0f;

	//��ލő�
	static constexpr int TYPE_MAX = static_cast<int>(TYPE::MAX);

	//�G�t�F�N�g�T�C�Y
	static constexpr float EFFECT_SCALE = 30.0f;

	//�Q�[�W���W
	static constexpr int GAGE_POS_X = 20;
	static constexpr int GAGE_POS_Y = 20;

	//���x���A�b�v���̃p�����[�^�[�ǉ���
	static constexpr float ADD_ATK_PER = 30.0f;
	static constexpr float ADD_DEF_PER = 30.0f;
	static constexpr float ADD_SPEED_PER = 30.0f;
	static constexpr int ADD_LIFE = 30;

	//�f�o�b�O�p�o���l
	static constexpr int DEBAG_EXP = 1000;

	//�E�̃��[�J�����WX
	static constexpr int RIGHT_LOCAL_POS_X = static_cast<int>(64 / 1.8f);

	//���̃��[�J�����WX
	static constexpr int LEFT_LOCAL_POS_X = static_cast<int>(32 * 1.2f);

	//���x���v�Z�p
	static constexpr int LEVEL_DIV = 10;

	//�R���X�g���N�^
	LevelScreenManager();

	//�f�X�g���N�^
	~LevelScreenManager();

	void Init();
	void Update();
	void Draw();
	void Release();

	//�ǂݍ���
	void Load();

	//����������
	void Reset();

	/// <summary>
	/// �o���l��������
	/// </summary>
	/// <param name="_value"></param>�ǉ��o���l��
	inline void AddExp(const float _value) { restExp_ += _value; }

	/// <summary>
	/// �o���l�Q�[�W�̐ݒ�
	/// </summary>
	/// <param name="_level"></param>���x��
	void SetGage(const int _level);

	/// <summary>
	/// ���x���A�b�v���ʔ��f
	/// </summary>
	/// <param name="_player"></param>�v���C���[
	/// <param name="_playerNum"></param>�v���C���[�ԍ�
	void Reflection(PlayerBase &_player,const int _playerNum);

	/// <summary>
	/// ��ԕύX
	/// </summary>
	/// <param name="_state"></param>���
	void ChangeState(const STATE _state);
	
	/// <summary>
	/// �G�t�F�N�g��Ǐ]������
	/// </summary>
	/// <param name="_player"></param>�v���C���[
	/// <param name="_playerNum"></param>�v���C���[�ԍ�
	void EffectSyne(PlayerBase& _player, const int _playerNum);

	/// <summary>
	/// �o���l�ʂ�Ԃ�
	/// </summary>
	/// <param name=""></param>
	/// <returns></returns>�o���l
	inline const float GetExp()const { return exp_; };

	/// <summary>
	/// ��Ԃ�Ԃ�
	/// </summary>
	/// <param name=""></param>
	/// <returns></returns>
	inline const STATE GetState()const { return state_; };

	/// <summary>
	/// ��ނ�Ԃ�
	/// </summary>
	/// <param name="_playerNum"></param>�v���C���[�ԍ�
	/// <returns></returns>��ނ�Ԃ�
	inline TYPE GetType(const int _playerNum)const { return selectTypes_[_playerNum]; }

	/// <summary>
	/// �O�̎�ނ�Ԃ�
	/// </summary>
	/// <param name="_playerNum"></param>�v���C���[�ԍ�
	/// <returns></returns>�O�̎��
	inline const TYPE GetPreType(const int _playerNum)const { return preTypeData_[_playerNum]; }

	/// <summary>
	/// ���x���A�b�v����
	/// </summary>
	/// <param name=""></param>
	/// <returns></returns>���x���A�b�v�������s���ꍇ��,true,�����ł͂Ȃ��ꍇ��,false
	inline const bool IsLevelUp()const  { return !(state_ == STATE::NONE || state_ == STATE::END); }

private:	
	
	//�ʒm
	std::unique_ptr<LevelNotice> notice_;

	//�I��
	std::unique_ptr<LevelSelect> select_;

	//�t�F�[�_�[
	std::unique_ptr<Fader> fader_;

	//�摜
	int imgGage_;		//�Q�[�W
	int imgGageExp_;	//�o���l�Q�[�W
	int *imgNumbers_;	//����

	//�v���C���[�l��
	int playerNum_;

	//���
	STATE state_;

	//���
	std::vector<TYPE> selectTypes_;

	//���݂̃��x��
	int nowLevel_;

	//�o���l
	float exp_;
	float restExp_;

	//�Q�[�W(���̃��x���܂ł̌o���l��)
	float gauge_;

	//�A���t�@�l
	float alpha_;

	//�t�F�[�h����
	bool isFader_;

	//�O���
	std::vector<TYPE> preTypeData_;

	// ��ԊǗ�(��ԑJ�ڎ���������)
	std::map<STATE, std::function<void()>> stateChanges_;

	// ��ԊǗ�
	std::function<void()> stateUpdate_;	//�X�V
	std::function<void()> stateDraw_;	//�`��



	//��ԕύX
	void ChangeStateNone();
	void ChangeStateNotice();
	void ChangeStateSelect();
	void ChangeStateEnd();

	//�e��X�V����
	void UpdateNone();
	void UpdateNotice();		//�ʒm
	void UpdateSelect();		//�����I��
	void UpdateEnd();			//�I��

	//�e��`�揈��
	void DrawNone();
	void DrawNotice();
	void DrawSelect();
	void DrawEnd();	
	
	//�o���l�̏�Ԋm�F
	void CheckExp();
						
	//UI�`��
	void DrawLevelUI();

	//�Ó]
	void FaderDraw();

	//�t�F�[�h�̐؂�ւ�����
	void Fade();

	//�X�L�b�v���
	void SkipState(const STATE& _nextState);

	//�f�o�b�O�@�\
	void DebagUpdate();
	void DebagDraw();

};