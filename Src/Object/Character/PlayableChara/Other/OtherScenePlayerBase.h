#pragma once
#include <Dxlib.h>
#include "../../../../Manager/Generic/SceneManager.h"
#include "../../../UnitBase.h"

class OtherScenePlayerBase : public UnitBase
{
public:

	//�A�j���[�V�����X�s�[�h
	static constexpr float ANIM_SPEED = 20.0f;

	//�A�C�h���A�j���ԍ�
	static constexpr int IDLE_ANIM = 61;

	//�X�P�[��
	static constexpr VECTOR SCALE = { 0.5f,0.5f,0.5f };

	//�v���C���[��
	static constexpr int PLAYERS = SceneManager::PLAYER_NUM;

	//�ʒu
	static constexpr VECTOR PLAYER_POS[PLAYERS] = {
		{-240.0f, -40.0f, -50.0f},
		{ -50.0f, -50.0f, -10.0f },
		{ 50.0f, -50.0f, -10.0f },
		{ 240.0f, -50.0f, -50.0f } };

	//����
	static constexpr float PLAYER_ROT[PLAYERS] = {
		-30.0f,
		-15.0f,
		15.0f,
		30.0f };

	//�R���X�g���N�^
	OtherScenePlayerBase();

	//�f�X�g���N�^
	~OtherScenePlayerBase() = default;

	void Init()override;
	void Update()override;
	void Draw()override;

protected:

	/// <summary>
	/// �A�j���[�V�����̏�����
	/// </summary>
	/// <param name="_anim"></param>�A�j���[�V�������
	/// <param name="_speed"></param>�A�j���[�V�����X�s�[�h
	void ResetAnim(const ANIM _anim, const float _speed) override;

private:

	//�L�������Ƃ̃g�����X�t�H�[��
	Transform trans_[PLAYERS];

	//��E
	SceneManager::ROLE role_[PLAYERS];

	//�A�j���[�V�����̏�����
	virtual void InitAnim() = 0;

	//3D���f��������
	void Init3DModel();

	//�A�j���[�V��������
	void Animation();
};