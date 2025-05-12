#pragma once
#include "OtherScenePlayerBase.h"
#include "../../../UnitBase.h"

class ClearPlayers : public OtherScenePlayerBase
{
public:

	//�A�j���[�V�����X�s�[�h
	static constexpr float ANIM_SPEED = 20.0f;

	//�A�C�h���A�j���ԍ�
	static constexpr int IDLE_ANIM = 61;

	//�R���X�g���N�^
	ClearPlayers();

	//�f�X�g���N�^
	~ClearPlayers() = default;

private:

	void InitAnim() override;

};