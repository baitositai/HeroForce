#pragma once
#include "PlayerBase.h"
class AxeMan :
    public PlayerBase
{
public:
#ifdef DEBUG_ON
    void DrawDebug(void)override;
#endif // DEBUG_ON

#ifdef DEBUG_COOL
    //�N�[���^�C��
    static constexpr float ATK_COOLTIME = 1.0f;
    static constexpr float SKILL_ONE_COOLTIME = 1.0f;
    static constexpr float SKILL_TWO_COOLTIME = 1.0f;
#else // DEBUG_COOL
    //�N�[���^�C��
    static constexpr float ATK_COOLTIME = 1.5f;
    static constexpr float SKILL_ONE_COOLTIME = 6.0f;
    static constexpr float SKILL_TWO_COOLTIME = 3.5f;
#endif // DEBUG_COOL

    //�X�e�[�^�X
    static constexpr float POW_ATK = 160.0f;
    static constexpr int DEF_MAX = 110;
    static constexpr int HP_MAX = 265;
    static constexpr float SPEED = PlayerBase::MOVE_SPEED_SLOW;

    //�U�������̎���
    static constexpr float ATK_START = 0.2f;
    static constexpr float SKILL_ONE_START = 10.0f;
    static constexpr float SKILL_TWO_START = 0.2f;

    //�e�U���̎�������
    static constexpr float FRAME_ATK_DURATION = 0.5f - ATK_START;
    static constexpr float FRAME_SKILL1_DURATION = 0.7f;
    static constexpr float FRAME_SKILL2_DURATION = 3.0f - SKILL_TWO_START;
    static constexpr float SKILL2_CHANGE_ANIM_TIME = 0.25f;

    //�㌄
    static constexpr float FRAME_ATK_BACKRASH = 0.1f;
    static constexpr float FRAME_SKILL1_BACKRASH = 0.2f;
    static constexpr float FRAME_SKILL2_BACKRASH = 0.2f;

    //�e�U���̍��W
    static constexpr VECTOR SKILL1_COL_LOCAL_POS = { 0.0f,100.0f,100.0f };
    static constexpr VECTOR ATK_COL_LOCAL_POS = { 0.0f,100.0f,100.0f };
    static constexpr VECTOR SKILL2_COL_LOCAL_POS = { 0.0f,100.0f,0.0f };

    //�U���͈�
    static constexpr float COL_ATK = CHARACTER_SCALE * 100.0f;
    static constexpr float COL_SKILL1 = CHARACTER_SCALE * 150.0f;
    static constexpr float COL_SKILL2 = CHARACTER_SCALE * 200.0f;

    //�U���З�
    static constexpr float NMLATK_POW = 12.0f;
    static constexpr float SKILL_ONE_POW = 25.0f;
    static constexpr float SKILL_ONE_CHARGE_POW = 50.0f;
    static constexpr float SKILL_TWO_POW = 8.0f;

    //���߃��[�V����
    static constexpr float SKILL_CHARGE_STEPANIM = 16.9f;

    //�G�t�F�N�g�T�C�Y
    //----------------------------------------------------
    //�X�L���Q�U���̏u��
    static constexpr float CHARGE_AXE_EFF_SIZE = 20.0f;

    //�X�L���`���[�W
    static constexpr float CHARGE_SKILL_EFF_SIZE = 50.0f;



    //�ʏ�U���̍ő�l
    static constexpr ATK ATK_MAX{ ATK_COL_LOCAL_POS,COL_ATK,NMLATK_POW,FRAME_ATK_DURATION,FRAME_ATK_BACKRASH,0.0f,false };

    //�X�L���P
    static constexpr ATK SKILL_ONE_MAX{ SKILL1_COL_LOCAL_POS ,COL_SKILL1 ,SKILL_ONE_POW,FRAME_SKILL1_DURATION ,FRAME_SKILL1_BACKRASH ,0.0f,false };

    //�X�L���Q
    static constexpr ATK SKILL_TWO_MAX{ SKILL2_COL_LOCAL_POS ,COL_SKILL2 ,SKILL_TWO_POW,FRAME_SKILL2_DURATION ,FRAME_SKILL2_BACKRASH ,0.0f,false };




    //�ŗL�A�j���[�V����
    static constexpr int ATK_NUM = 9;
    static constexpr int SKILL_ONE_NUM = 8;
    static constexpr int SKILL_TWO_NUM = 9;
    static constexpr int SPIN_NUM = 11;

    //�X�L���Q�̉�]�U���̑��i�q�b�g�C���^�[�o��
    static constexpr float MULTIHIT_INTERVAL = 0.5f;
    static constexpr float ATK_START_RANGE = 250.0f * CHARACTER_SCALE;	//�U���J�n����̑傫��

    AxeMan(void);
    ~AxeMan(void) = default;
    void SetParam(void)override;
protected:

    void AtkFunc(void)override;
    void Skill1Func(void)override;
    void Skill2Func(void)override;
    void InitAct(void)override;
    void InitSkill(ATK_ACT _act);

    void Draw(void)override;

    //�ŗL�A�j���[�V�����ԍ��̏�����
    void InitCharaAnim(void)override;

    //void ChargeAct(void)override;
    //�U������
    void NmlAtkInit(void)override;
    //�X�L������
    void SkillOneInit(void)override;
    void SkillTwoInit(void)override;
    //void ResetParam(ATK_ACT _act)override;
};

