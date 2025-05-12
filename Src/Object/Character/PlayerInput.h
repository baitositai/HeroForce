#pragma once
#include<DxLib.h>
#include"../../Manager/Generic/SceneManager.h"
#include"../../Manager/Generic/InputManager.h"
class PlayerBase;
class PlayerInput
{
public:
    //*************************************************
//�e�A�N�V��������{�^��
//*************************************************
//�ړ�
    static constexpr int MOVE_FRONT_KEY = KEY_INPUT_W;
    static constexpr int MOVE_LEFT_KEY = KEY_INPUT_A;
    static constexpr int MOVE_BACK_KEY = KEY_INPUT_S;
    static constexpr int MOVE_RIGHT_KEY = KEY_INPUT_D;

    //�U��
    static constexpr int ATK_KEY = KEY_INPUT_E;
    static constexpr InputManager::JOYPAD_BTN ATK_BTN = InputManager::JOYPAD_BTN::RIGHT;

    //�X�L��
    static constexpr int SKILL_KEY = KEY_INPUT_Q;
    static constexpr InputManager::JOYPAD_BTN SKILL_BTN = InputManager::JOYPAD_BTN::TOP;

    //�X�L���ύX�L�[
    static constexpr int SKILL_CHANGE_KEY = KEY_INPUT_J;
    static constexpr InputManager::JOYPAD_BTN SKILL_CHANGE_BTN = InputManager::JOYPAD_BTN::R_BUTTON;

    //���
    static constexpr int DODGE_KEY = KEY_INPUT_N;
    static constexpr InputManager::JOYPAD_BTN DODGE_BTN = InputManager::JOYPAD_BTN::LEFT;

    //�A�N�V�����{�^���̎��
    enum class ACT_CNTL
    {
        NONE    //�������Ă��Ȃ��Ƃ�
        , MOVE   //�ړ�
        , DODGE             //���
        , NMLATK            //�ʏ�U��    
        , CHANGE_SKILL      //�X�L���؂�ւ�
        , SKILL_DOWN        //�Z�����X�L��
        , SKILL_KEEP        //�������X�L��(�������ςȂ�)
        , SKILL_UP          //�������X�L��(����)
    };

    //�C���X�^���X�̐���
    static void CreateInstance(void);
    static PlayerInput& GetInstance(void);

    
    void Update(PlayerBase* _player,InputManager::JOYPAD_NO _padNum,SceneManager::CNTL _cntl);




    //�R���g���[������
    bool CheckAct(ACT_CNTL _actCntl) { return actCntl_ == _actCntl ? true : false; }

    //�Q�b�^
    //----------------------------------------------------------------
    ACT_CNTL GetAct(void) { return actCntl_; }
    float GetStickDeg(void){ return stickDeg_; }
    float GetMoveDeg(void) { return moveDeg_; }

private:

    //�V���O���g�������邽�߂�
	PlayerInput(void);
	~PlayerInput(void)=default;

    //�����o�֐�
    void InputKeyBoard(PlayerBase* _player);
    void InputPad(PlayerBase* _player, InputManager::JOYPAD_NO _padNum);

    //�����o�ϐ�
    //-----------------------------------------------------------------------
    //����Ǘ��p
    ACT_CNTL actCntl_;

    float leftStickX_;
    float leftStickY_;
    float stickDeg_;            //�p�b�h�̃X�e�B�b�N�̊p�x
    float moveDeg_;             //�ړ�����
};

