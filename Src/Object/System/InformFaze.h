#pragma once
#include<DxLib.h>
#include<string>
#include"../../Application.h"

class InformFaze
{
public:

	static constexpr int INFORM_HARF = Application::DEFAULT_FPS;
	static constexpr int INFORM_FINISH = Application::DEFAULT_FPS * 2;

	static constexpr int MISSION_SIZE_X = 1270;
	static constexpr int MISSION_SIZE_Y = 720;

	//�\���̏����ʒu�y�эŏI�ʒu(�Ō��-�͔������p)
	static constexpr float STRING_SPOS_Y = Application::SCREEN_SIZE_Y / 2.0f - 60.0f;
	static constexpr float STRING_FPOS_Y = Application::SCREEN_SIZE_Y / 2.0f - 30.0f;
	static constexpr float MISSION_SPOS_Y = Application::SCREEN_SIZE_Y / 2.0f - 30.0f;
	static constexpr float MISSION_FPOS_Y = Application::SCREEN_SIZE_Y / 2.0f;

	InformFaze(void);
	~InformFaze(void);

	bool Update(void);
	void Draw(void);

	/// <summary>
	/// �t�F�[�Y���ݒ�(�������Z�b�g)���t�F�[�Y�\�����s���O�ɕK���Ăяo������
	/// </summary>
	/// <param name="_cnt">�t�F�[�Y��</param>
	void SetFazeCnt(const int _cnt);

private:
	VECTOR informStrPos_;		//�t�F�[�Y���\���ʒu
	VECTOR informImgPos_;		//�ڕW�摜�\���ʒu
	int fazeCnt_;			//�t�F�[�Y���J�E���g
	int informCnt_;			//�o�ߎ��ԃJ�E���g
	int missionImg_[2];		//�~�b�V�����摜
	int missionCnt_;		//��L�z��̎w��
	int font_;	//�t�H���g
};

