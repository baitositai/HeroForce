#pragma once
#include "PlayerBase.h"
class Arrow;
class Knight :
    public PlayerBase
{
public:
    //�X�e�[�^�X
    static constexpr float POW_ATK = 120.0f;
    static constexpr float MAX_DEF = 150.0f;
    static constexpr float MAX_HP = 250;
    static constexpr float SPEED = PlayerBase::MOVE_SPEED_NORMAL;



    static constexpr float ATK_START_RANGE = 250.0f * CHARACTER_SCALE;	//�U���J�n����̑傫��
    //�X�L���g�p�J�n���ɃN�[���^�C���𑝂₷����
    static constexpr float SKILL_TWO_START_COOLTIME = 3.0f;

    //�K�[�h�\�ɂȂ�c��N�[���^�C��(�K�[�h���ԍŒ�1�b�m�ۂ���������)
    static constexpr float GUARD_STARTABLE_COOL = 4.0f;

    //�K�[�h�G�t�F�N�g�T�C�Y
    static constexpr float GUARD_EFFECT_SIZE = 20.0f;
#ifdef DEBUG_COOL
    //�N�[���^�C��
    static constexpr float ATK_COOLTIME = 1.0f;
    static constexpr float SKILL_ONE_COOLTIME = 1.0f;
    static constexpr float SKILL_TWO_COOLTIME = 10.0f;

   

#else // DEBUG_COOL
#ifdef DEBUG_ON
    void DrawDebug(void)override;
#endif // DEBUG_ON

    //�N�[���^�C��
    static constexpr float ATK_COOLTIME = 0.75f;
    static constexpr float SKILL_ONE_COOLTIME = 3.3f;
    static constexpr float SKILL_TWO_COOLTIME = 10.0f;
#endif // DEBUG_COOL
    //�ʏ�U���̃X�e�[�^�X
    //------------------------------------------------------------------
    static constexpr float ATK_START = 0.2f;                               //�O��
    static constexpr float FRAME_ATK_DURATION = 0.4f - ATK_START;          //��������
    static constexpr float FRAME_ATK_BACKRASH = 0.1f;                      //�㌄
    static constexpr VECTOR ATK_COL_LOCAL_POS = { 0.0f,100.0f,100.0f };    //���[�J�����W
    static constexpr float COL_ATK = CHARACTER_SCALE * 100.0f;             //�����蔻��̔��a
    static constexpr float NMLATK_POW = 9.0f;          //�ʏ�U��             //�Z�З�
    //------------------------------------------------------------------

    //�X�L��1�̃X�e�[�^�X
    //------------------------------------------------------------------
    //�ߐ�
    static constexpr float SKILL_ONE_START = 0.2f;
    static constexpr float FRAME_SKILL1_DURATION = 0.3f;
    static constexpr float FRAME_SKILL1_BACKRASH = 0.2f;
    static constexpr float COL_SKILL1 = CHARACTER_SCALE * 150.0f;
    static constexpr VECTOR SKILL1_COL_LOCAL_POS = { 0.0f,100.0f,100.0f };
    static constexpr float SKILL_ONE_POW = 30.0f;   

    //�a��
    static constexpr float COL_SLASH = CHARACTER_SCALE * 100.0f;
    static constexpr VECTOR SLASH_LOCAL_POS = { 0.0f,100.0f,100.0f };
    static constexpr float FRAME_SLASH_DURATION = 2.0f;
    static constexpr float SLASH_POW = 30.0f;       //�X�L��1�̎a��
    static constexpr float SLASH_SPEED = 15.0f;



    //------------------------------------------------------------------

    //�X�L��2�̃X�e�[�^�X
    //------------------------------------------------------------------
    //ATK�̃X�e�[�^�X
    static constexpr float SKILL_TWO_START = 0.3f;
    static constexpr float FRAME_SKILL2_DURATION = 3.0f;
    static constexpr float FRAME_SKILL2_BACKRASH = 0.0f;
    //static constexpr float COL_SKILL2 = CHARACTER_SCALE * 400.0f;
    static constexpr float COL_SKILL2 = CHARACTER_SCALE * 0.0f;
    static constexpr VECTOR SKILL2_COL_LOCAL_POS = { 0.0f,100.0f,0.0f };
    static constexpr float SKILL_TWO_POW = 0.0f;

    //�K�[�h���̃X�e�b�v
    static constexpr float SKILL2_CHANGE_ANIM_TIME = 0.7f;
    //------------------------------------------------------------------
    //�ʏ�U���̍ő�l
    static constexpr ATK ATK_MAX{ ATK_COL_LOCAL_POS
        ,COL_ATK
        ,NMLATK_POW
        ,FRAME_ATK_DURATION
        ,FRAME_ATK_BACKRASH
        ,0.0f,false };

    //�X�L���P
    static constexpr ATK SKILL_ONE_MAX{ SKILL1_COL_LOCAL_POS
        ,COL_SKILL1
        ,SKILL_ONE_POW
        ,FRAME_SKILL1_DURATION
        ,FRAME_SKILL1_BACKRASH
        ,0.0f,false };

    //�a��
    static constexpr ATK SLASH_MAX{ SLASH_LOCAL_POS
        ,COL_SLASH
        ,SLASH_POW
        ,FRAME_SLASH_DURATION
        ,0.0f
        ,0.0f
        ,false
    };

    //�X�L���Q
    static constexpr ATK SKILL_TWO_MAX{ SKILL2_COL_LOCAL_POS
        ,COL_SKILL2
        ,SKILL_TWO_POW
        ,FRAME_SKILL2_DURATION
        ,FRAME_SKILL2_BACKRASH
        ,0.0f,false };

    //�ŗL�A�j���[�V����
    static constexpr int ATK_NUM = 1;
    static constexpr int SKILL_ONE_NUM = 2;
    static constexpr int SKILL_TWO_NUM = 18;
    static constexpr int BLOCKING_NUM = 21;
    Knight(void);
    ~Knight(void) = default;
    void SetParam(void)override;
    void Update(void)override;
    void Draw(void)override;



protected:
    //�U���A�N�V�����̃p�����[�^�̏�����
    void InitAct(void)override;
    void AtkFunc(void)override;

    //void ResetGuardCnt(void)override;

      //�U������
    void NmlAtkInit(void)override;
    //�X�L������
    void SkillOneInit(void)override;
    void SkillTwoInit(void)override;

    //�e�X�L���̏���
    void InitSkill(ATK_ACT _act);
    void Skill1Func(void)override;
    void Skill2Func(void)override;

    //�ŗL�A�j���[�V�����ԍ��̏�����
    void InitCharaAnim(void)override;

private:
    //�����o�[�֐�
    void InitSlashAtk(ATK & arrowAtk);
    void CreateSlash(void);
    void CreateSlashAtk(void);




    //�����o�ϐ�
    float guardCnt_;        //�K�[�h����
    //std::vector<ATK>slashArrow_;
    ATK slashArrow_;                            //�a��(������������Ȃ����ߕ��ʂ̕ϐ��ɂ���)
    int arrowMdlId_;							//��̃��f��
    std::vector<std::shared_ptr<Arrow>> arrow_;	//�|��
    bool isShotArrow_;							//�����������̔���(true:������)
    int arrowCnt_;								//��̎g�p���J�E���^
    float reloadCnt_;							//��̃����[�h����

    float atkAbleCnt_;                          //��̔��ˉ\�J�E���g
};

