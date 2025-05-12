#pragma once
#include <vector>
#include <map>
#include <string>
#include <memory>
#include <fstream>
#include"../Common/Transform.h"


class StageObject;

class StageManager
{
public:

	//���f�����
	enum class MODEL_TYPE
	{
		STAGE,	//�{��
		DECO,	//����(�Փ˔���͍s��Ȃ�)
		MAX
	};

	//�傫��
	static constexpr VECTOR SCALE = { 1.0f,1.0f,1.0f };

	//�����ʒu
	static constexpr VECTOR DEFAULT_POS = { 0.0f,-120.0f,0.0f };
	
	// �X�e�[�W���f����
	static constexpr int MODELS = static_cast<int>(MODEL_TYPE::MAX);

	//�R���X�g���N�^
	StageManager(void);

	//�f�X�g���N�^
	~StageManager(void);

	//���
	virtual void Destroy(void);

	//��{�����̂S�͉��z�֐�������̂ł�������I�[�o�[���C�h����悤��
	virtual void Init(void);	//������
	virtual void Update(void);	//�X�V
	virtual void Draw(void);	//�`��

	//�X�e�[�W�̃g�����X�t�H�[����Ԃ�
	const Transform &GetTtans() const{ return trans_[static_cast<int>(MODEL_TYPE::STAGE)]; }	

protected:

	//�g�����X�t�H�[��
	Transform trans_[MODELS];
	
	//�f�o�b�O�p�`��
	void DebugDraw();

};

