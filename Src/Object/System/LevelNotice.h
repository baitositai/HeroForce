#pragma once
#include <string>
#include <memory>
#include <functional>
#include "../../Common/Fader.h"
#include "../../Manager/Decoration/EffectManager.h"
#include "LevelScreenManager.h"

class LevelScreenManager;

class LevelNotice
{
public:

	//�摜�Ɋւ���`��̏��
	enum class STATE
	{
		NONE,
		FADE_IN,	//�t�F�[�h�C��
		MAINTAIN,	//�ێ�
		FIN,		//�I��
	};

	//�g�嗦
	static constexpr float EXPANSION_RATE = 3.0f;

	//�g��ő�
	static constexpr float EXPANSION_MAX = 1.2f;

	//�G�t�F�N�g�g�嗦
	static constexpr float EFFECT_RATE = 1.5f;

	//�ێ��b��
	static constexpr float MAINTAIN_SECONDS =1.5f;

	//�t�H���g�֘A
	static constexpr int FONT_TEXT_SIZE = Application::SCREEN_SIZE_X * 156 / Application::DEFA_SCREEN_SIZE_X;
	static constexpr int FONT_LEVEL_SIZE = Application::SCREEN_SIZE_X * 172 / Application::DEFA_SCREEN_SIZE_X;
	static constexpr int FONT_THICK = 3;

	//���ߊJ�n����(���x���e�L�X�g)
	static constexpr float START_ALPHA_LEVEL =2;

	//�G�t�F�N�g�X�s�[�h
	static constexpr int EFFECT_ANIM_SPPED = 3;
	
	//UI�ʒu
	static constexpr int MES_TEXT_POS_X = FONT_TEXT_SIZE /2;
	static constexpr int MES_TEXT_POS_Y = 100;
	static constexpr int LEVEL_TEXT_POS_X = 8;
	static constexpr int LEVEL_TEXT_POS_Y = 70;
	static constexpr int EFFECT_POS_X = Application::SCREEN_SIZE_X / 2;
	static constexpr int EFFECT_POS_Y = (Application::SCREEN_SIZE_Y - FONT_LEVEL_SIZE) / 2 + LEVEL_TEXT_POS_Y;

	//�R���X�g���N�^
	LevelNotice();

	//�f�X�g���N�^
	~LevelNotice() = default;

	void Init();
	void Update();
	void Draw();
	void Release();
	
	//�ǂݍ���
	void Load();

	//���Z�b�g
	void Reset();

	/// <summary>
	/// ��ԕύX
	/// </summary>
	/// <param name="_state"></param>���
	void ChangeState(const STATE _state);

	/// <summary>
	/// �V�������x���ݒ�
	/// </summary>
	/// <param name="_newLevel"></param>�V�������x��
	void SetNewLevel(const int& _newLevel);

	//��Ԃ�Ԃ�
	inline const STATE GetState() const { return state_; }

private:
	
	//�G�t�F�N�g�摜
	int *imgEfe_;

	//�G�t�F�N�g�X�V�p�X�e�b�v
	int efeStep_;

	//�G�t�F�N�g�̃A�j���[�V�����ԍ�
	int efeAnimNum_;

	//�G�t�F�N�g�̃X�s�[�h
	int efeSpeed_;

	//�G�t�F�N�g�̍Đ�
	bool isEfe_;

	//���
	STATE state_;

	//�J�E���g
	float cnt_;

	//���x��
	int newLevel_;

	//�傫��
	float scl_;
	
	//�A���t�@�l
	float alphaMes_;	//���b�Z�[�W�p
	float alphaLevel_;	//���x���p

	//�t�H���g
	int fontMes_;		//���b�Z�[�W�p�t�H���g
	int fontLevel_;		//���x���p�t�H���g

	//�e�L�X�g
	std::string text_;

	// ��ԊǗ�(��ԑJ�ڎ���������)
	std::map<STATE, std::function<void()>> stateChanges_;

	// ��ԊǗ�
	std::function<void()> stateUpdate_;	//�X�V

	//��ԕύX����
	void ChangeStateNone();
	void ChangeStateFade();
	void ChangeStateMaintain();
	void ChangeStateFin();

	//�X�V����
	void UpdateNone();
	void UpdateFade();
	void UpdateMaintain();
	void UpdateFin();

	//�G�t�F�N�g����
	void EffectUpdate();

	//�`��
	void DrawMessage();
	void DrawLevel();

};

