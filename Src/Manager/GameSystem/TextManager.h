#pragma once
#include <windows.h>
#include <iostream>
#include <fstream>
#include <string>
#include <unordered_map>

class TextManager
{
public:

	//���t�H���g�����͊ecpp�ōs���Ă�������
	//�@���̍�DeleteFontToHandle��Y�ꂸ�ɁI

	//�t�H���g���
	enum class FONT_TYPE
	{
		NORMAR,
		DOT,
		LOGO,
		BOKUTACHI,
		HANAZOME,
		BANANA,
		PENGS,
		KAKUMEI,
		MAX
	};

	//�e�L�X�g�Ăяo���p
	enum class TEXTS		//MES---���b�Z�[�W�̗�
	{
		LEVEL_UP,			//�����֌W�̃e�L�X�g
		LV_UP_SELECT_MES,
		LV_ATK_MES,
		LV_DEF_MES,
		LV_SPEED_MES,
		LV_LIFE_MES,
		MAX
	};

	//�e�L�X�g���������p(�e�L�X�g�̒��� �� �萔)
	static constexpr int CENTER_TEXT = 2;
	static constexpr int CENTER_TEXT_H = 4;	//�p���Ȃǂ̔��p�݂̂̃e�L�X�g�͂���Ŋ���

	//�C���X�^���X�̐���
	//�V���O���g����
	static void CreateInstance();
	static TextManager& GetInstance();

	//������
	void Init();

	/// <summary>
	/// �e�L�X�g��ǂ݂���
	/// </summary>
	/// <param name="_text"></param>�e�L�X�g���
	/// <returns></returns>�e�L�X�g
	std::string TextLoad(const TEXTS _text);

	/// <summary>
	/// �t�H���g�̖��O��Ԃ�
	/// </summary>
	/// <param name="_type"></param>�t�H���g���
	/// <returns></returns>���O
	inline std::string GetFontName(const FONT_TYPE _type) const { return fontName_[static_cast<int>(_type)]; }

	/// <summary>
	/// �e�L�X�g�T�C�Y��Ԃ�
	/// </summary>
	/// <param name="_txt"></param>�e�L�X�g
	/// <param name="_num"></param>�i���o�[
	/// <returns></returns>�e�L�X�g�T�C�Y
	int GetTextSize(const std::string _txt,const int _num)const;

	//�������
	void Destroy();

private:

	//�t�H���g���
	std::string fontPath_[static_cast<int>(FONT_TYPE::MAX)];	//�p�X
	std::string fontName_[static_cast<int>(FONT_TYPE::MAX)];	//���O

	//�f�[�^
	std::unordered_map<TextManager::TEXTS, std::string> textDatas_;

	//�t�H���g������
	void InitFont();

	//�e�L�X�g��ێ�����}�b�v
	std::unordered_map<TEXTS, std::string> LoadDialogues(const std::string& _filename);

	//�e�L�X�g�̓ǂݍ���
	std::string* _Load(const TEXTS _text);

	// �f�t�H���g�R���X�g���N�^��private�ɂ��āA
	// �O�����琶���ł��Ȃ��l�ɂ���
	TextManager();
	~TextManager() = default;

	//�V���O���g����
	static TextManager* instance_;
};
