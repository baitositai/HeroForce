#include "ClearPlayers.h"

using S_ROLE = SceneManager::ROLE;

ClearPlayers::ClearPlayers()
{
}

void ClearPlayers::InitAnim()
{	
	//�A�j���[�V�����̐ݒ�
	animNum_.emplace(ANIM::IDLE, IDLE_ANIM);
	ResetAnim(ANIM::IDLE, ANIM_SPEED);
}