#pragma once
#include "SceneBase.h"

class ExplanScene : public SceneBase
{
public:

	//����������
	static constexpr int EXPLAN_CNT = 7;

	//�傫��
	static constexpr float EXP_RATE = 0.6f;
	static constexpr int POINT_SIZE = 54;

	//�t�H���g�T�C�Y
	static constexpr int FONT_SIZE = 36;

	//�A���t�@�l�ő�
	static constexpr int ALPHA_MAX = 255;
	static constexpr int ALPHA_MIN = 130;

	//���b�Z�[�W�ʒu
	static constexpr int MES_POS_X = Application::SCREEN_SIZE_X / 2;
	static constexpr int MES_POS_Y = Application::SCREEN_SIZE_Y - FONT_SIZE - 15;

	//�R���X�g���N�^
	ExplanScene();

	//�f�X�g���N�^
	~ExplanScene() = default;

	void Init() override;
	void Update() override;
	void Draw() override;
	void Release() override;

private:

	//�摜
	int imgExp_[EXPLAN_CNT];	//�����摜
	int *imgPoint_;				//�|�C���g�摜

	//�A���t�@�l
	float alpha_;

	//�A���t�@�ǉ��l
	float alphaAdd_;

	//�X�V�p�X�e�b�v
	float step_;

	//�t�H���g
	int font_;

	//�������ԍ�
	int cntExp_;

	//�X�e�B�b�N����
	bool isStick_;

};

