#include "PlayerInput.h"
#include"PlayableChara/PlayerBase.h"
#include"../../Utility/AsoUtility.h"

PlayerInput* playerInput_ = nullptr;
void PlayerInput::CreateInstance(void)
{
	if (playerInput_ == nullptr)
	{
		playerInput_ = new PlayerInput();
	}
	//actCntl_ = SceneManager::CNTL::NONE;
}

PlayerInput& PlayerInput::GetInstance(void)
{
	return *playerInput_;
}

void PlayerInput::Update(PlayerBase* _player, InputManager::JOYPAD_NO _padNum,SceneManager::CNTL _cntl)
{
	switch (_cntl)
	{
	case SceneManager::CNTL::NONE:
		break;
	case SceneManager::CNTL::KEYBOARD:
		InputKeyBoard(_player);
		break;
	case SceneManager::CNTL::PAD:
		InputPad(_player, _padNum);
		break;
	default:
		break;
	}
}

void PlayerInput::InputKeyBoard(PlayerBase* _player)
{
	auto& ins = InputManager::GetInstance();
	using ATK_ACT = PlayerBase::ATK_ACT;
	actCntl_ = ACT_CNTL::NONE;
	//�ړ�(�����ꂩ�̈ړ��L�[�������Ă�����)
	if (ins.IsNew(MOVE_FRONT_KEY) || ins.IsNew(MOVE_BACK_KEY) || ins.IsNew(MOVE_LEFT_KEY) || ins.IsNew(MOVE_RIGHT_KEY)) { actCntl_ = ACT_CNTL::MOVE; }

	//�ړ��p�x�����߂�
	if (ins.IsNew(PlayerInput::MOVE_FRONT_KEY)) { moveDeg_ = 0.0f; }
	else if (ins.IsNew(PlayerInput::MOVE_LEFT_KEY)) { moveDeg_ = 270.0f; }
	else if (ins.IsNew(PlayerInput::MOVE_BACK_KEY)) { moveDeg_ = 180.0f; }
	else if (ins.IsNew(PlayerInput::MOVE_RIGHT_KEY)) { moveDeg_ = 90.0f; }

	//�ʏ�U��
	if (ins.IsTrgDown(ATK_KEY) && !_player->GetIsCool(ATK_ACT::ATK) && !_player->GetIsAtk() && !_player->GetIsSkill()) { actCntl_ = ACT_CNTL::NMLATK; }

	//�X�L���Z����
	if (ins.IsTrgDown(SKILL_KEY) && !_player->GetIsAtk() && !_player->GetIsSkill()) { actCntl_ = ACT_CNTL::SKILL_DOWN; }

	//������
	else if (ins.IsNew(SKILL_KEY)&& _player->GetIsSkill()) { actCntl_ = ACT_CNTL::SKILL_KEEP; }

	//�{�^�������ꂽ��
	if (ins.IsTrgUp(SKILL_KEY)&& _player->GetIsSkill()) { actCntl_ = ACT_CNTL::SKILL_UP; }

	//���
	if (ins.IsTrgDown(DODGE_KEY)&&!_player->GetIsAtk() && !_player->GetIsSkill()) { actCntl_ = ACT_CNTL::DODGE; }

	//�X�L���ύX�L�[
	if (ins.IsTrgDown(SKILL_CHANGE_KEY)&&!_player->GetIsSkill()) { actCntl_ = ACT_CNTL::CHANGE_SKILL; }
}

void PlayerInput::InputPad(PlayerBase* _player, InputManager::JOYPAD_NO _padNum)
{
	auto& ins = InputManager::GetInstance();
	using ATK_ACT = PlayerBase::ATK_ACT;
	actCntl_ = ACT_CNTL::NONE;
	// ���X�e�B�b�N�̉���
	leftStickX_ = ins.GetJPadInputState(_padNum).AKeyLX;
	//�c��
	leftStickY_ = ins.GetJPadInputState(_padNum).AKeyLY;
	auto stickRad = static_cast<float>(atan2(static_cast<double>(leftStickY_), static_cast<double>(leftStickX_)));
	stickDeg_ = static_cast<float>(AsoUtility::DegIn360(AsoUtility::Rad2DegF(stickRad) + 90.0f));


	if (leftStickX_ != 0.0f || leftStickY_ != 0.0f)
	{ 
		actCntl_ = ACT_CNTL::MOVE; }

	//�X�e�B�b�N�̊p�x�ɂ���Ĉړ����������߂�
	moveDeg_ = stickDeg_;


	if (ins.IsPadBtnTrgDown(_padNum, ATK_BTN) && !_player->GetIsCool(ATK_ACT::ATK) &&!_player->GetIsAtk() && !_player->GetIsSkill()) { actCntl_ = ACT_CNTL::NMLATK; }
	if(ins.IsPadBtnTrgDown(_padNum, SKILL_BTN) && !_player->GetIsAtk() && !_player->GetIsSkill()){ actCntl_ = ACT_CNTL::SKILL_DOWN; }
	else if(ins.IsPadBtnNew(_padNum, SKILL_BTN) && _player->GetIsSkill()&& !_player->GetIsAtk()){ actCntl_ = ACT_CNTL::SKILL_KEEP; }
	if( ins.IsPadBtnTrgUp(_padNum, SKILL_BTN) && _player->GetIsSkill()&&!_player->GetIsAtk()){ actCntl_ = ACT_CNTL::SKILL_UP; }
	if(ins.IsPadBtnTrgDown(_padNum, DODGE_BTN) && !_player->GetIsAtk() && !_player->GetIsSkill()){ actCntl_ = ACT_CNTL::DODGE; }
	if(ins.IsPadBtnTrgDown(_padNum, SKILL_CHANGE_BTN) && !_player->GetIsSkill()){ actCntl_ = ACT_CNTL::CHANGE_SKILL; }
}

PlayerInput::PlayerInput(void)
{
	actCntl_ = ACT_CNTL::NONE;
	leftStickX_ = -1;
	leftStickY_ = -1;
	stickDeg_ = -1;
}