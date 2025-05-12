#pragma once
#include "../../../UnitBase.h"
#include "OtherScenePlayerBase.h"

class OverPlayers : public OtherScenePlayerBase
{
public:

	//�A�j���[�V�����X�s�[�h
	static constexpr float ANIM_SPEED = 20.0f;

	//�A�C�h���A�j���ԍ�
	static constexpr int IDLE_ANIM = 24;
	static constexpr int DEATH_ANIM = 23;

	//�R���X�g���N�^
	OverPlayers();

	//�f�X�g���N�^
	~OverPlayers() = default;

private:

	//�A�j���[�V����������
	void InitAnim()override;

	//�A�j���[�V�����̏I��
	void FinishAnim()override;
};

