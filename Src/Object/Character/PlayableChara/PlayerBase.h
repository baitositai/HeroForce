#pragma once
#include<map>
#include<functional>
#include"../Utility/AsoUtility.h"
#include"../../../Manager/Generic/SceneManager.h"
#include "../../UnitBase.h"
#include"../../../Manager/Generic/InputManager.h"
#include"../../../Manager/Decoration/EffectManager.h"
#include"../PlayerManager.h"
#include"../PlayerInput.h"
#include "../../UnitBase.h"

//�f�o�b�O
//#define DEBUG_ON
//#define INPUT_DEBUG_ON
//#define DEBUG_COOL
class PlayerDodge;
class PlayerBase :
    public UnitBase
{
public:
#ifdef DEBUG_ON
    void InitDebug(void);
    unsigned int color_Col_;
    unsigned int color_Atk_;
    unsigned int color_skl1_;
    unsigned int color_skl2_;

    //�f�o�b�O�p�֐�
    virtual void DrawDebug(void);

#endif // DEBUG_ON
    //�f�o�b�O�p
      //�G�t�F�N�g���ŏ���1�t���[���̎��ɍĐ�������p��FPS
    static constexpr float DELTATIME = 1.0f / 60.0f;


    //�e�A�j���[�V�����ԍ�
    static constexpr int T_POSE_NUM = 64;
    static constexpr int IDLE_NUM = 36;
    static constexpr int WALK_NUM = 72;
    static constexpr int RUN_NUM = 49;
    static constexpr int DODGE_NUM = 28;
    static constexpr int DAMAGE_NUM = 35;
    static constexpr int DEATH_NUM = 23;

    //�A�j���[�V�����X�s�[�h
    static constexpr float SPEED_ANIM_IDLE = 20.0f;
    static constexpr float SPEED_ANIM_RUN = 80.0f;
    static constexpr float SPEED_ANIM_DODGE = 30.0f;
    static constexpr float SPEED_ANIM_ATK = 50.0f;

    //���ɃA�j���[�V�����̎~�߂�J�E���g
    static constexpr float DEATH_STEP_ANIM = 22.7;

    //�U���̎�ނ̐�
    static constexpr int ATK_TOTAL = 3;

    // �ړ��X�s�[�h
    static constexpr float SPEED_MOVE = 5.0f;
    static constexpr float SPEED_DODGE = 15.0f;
    static constexpr float MOVE_SPEED_SLOW = 3.0f;
    static constexpr float MOVE_SPEED_NORMAL = 5.0f;
    static constexpr float MOVE_SPEED_FAST = 8.0f;

    static constexpr float FRAME_DODGE_MAX = 1.0f * CHARACTER_SCALE;
    static constexpr float DODGE_CDT_MAX = 0.5f;

    //�v���C���[���g�̓����蔻��
    static constexpr VECTOR PLAYER_COL_LOCAL_POS = { 0.0f,100.0f,0.0f };

    //���g�̓����蔻�蔼�a
    static constexpr float MY_COL_RADIUS = 66.0f * CHARACTER_SCALE;

    //�A�[�`���[�̍��G(�G�ɖ��������͈�)
    static constexpr float ARCHER_SEARCH_RANGE = CHARACTER_SCALE * 1300.0f;

    //�t�F�[�Y�ς��Ƃ��Ƀv���C���[������ł�����hp��
    static constexpr float CHANGEFAZE_RECOVERY_PER = 0.2f;

    //�v���C���[�̏������W
    static constexpr VECTOR PLAYER1_POS = { -200.0f,0.0F,0.0f };
    static constexpr VECTOR PLAYER2_POS = { -100.0f,0.0F,0.0f };
    static constexpr VECTOR PLAYER3_POS = { 0.0F,0.0F,0.0f };
    static constexpr VECTOR PLAYER4_POS = { 100.0F,0.0F,0.0f };

    //����
    static constexpr int SKILL_CHANGE_SE_VOL = 80;

    //*************************************************
    // �񋓌^
    //*************************************************

    //�U�����
    enum class ATK_ACT
    {
        ATK
        , SKILL1
        , SKILL2
        , MAX
    };

    //�X�L���ύX�p
    enum class SKILL_NUM
    {
        ONE = 1
        , TWO = 2
        , MAX
    };

    enum class ATK_TYPE
    {
        ATTACK
        ,BUFF
        ,MAX
    };

    enum class STATUSBUFF_TYPE
    {
        ATK_BUFF
        ,DEF_BUFF
        ,SPD_BUFF
        ,MAX
    };

    //�o�t����X�L��
    enum class SKILL_BUFF
    {
        BUFF_ARROW  //�A�[�`���[�̃X�L��2
        ,GUARD      //�i�C�g�̃K�[�h
        ,MAX
    };

    struct BUFF
    {
        float cnt;
        float percent[static_cast<int>(STATUSBUFF_TYPE::MAX)];
        bool isBuff;            //�o�t�������ǂ���(�o�t�̏d�ˊ|����h��)
        bool isBuffing;         //�o�t�����ǂ���  (���t���[���o�t����̂�h��)
    };

    PlayerBase(void);
    ~PlayerBase(void) = default;
    void Destroy(void)override;
    virtual void SetParam(void) = 0;
    void Init(void)override;
    virtual void Update(void)override;
    virtual void Draw(void)override;


    /// <summary>
    /// ��̓����蔻��
    /// </summary>
    /// <param name="_chaser">�U������l</param>
    /// <param name="_target">�U�������l</param>
    /// <param name="_num">�����蔻�肷���̔z��ԍ�</param>
    /// <returns></returns>
    virtual const bool IsHitArrowAtk(const PlayerBase& _chaser, const UnitBase& _target, const int _num) { return false; }

    //���I�z���ATK��Ԃ�
    //virtual std::vector<ATK>GetAtks(void) { return atks_; }

    virtual std::vector<ATK> GetAtks(ATK_TYPE _type) { return atks_[_type]; }

    virtual void SetIsArrowHit(ATK_TYPE _type, const bool _flg, int _num){ atks_[_type][_num].isHit_ = _flg; }

     //�_���[�W�֐�
    void Damage(void);

    //���Z�b�g
    void Reset(void);

    //��E���ꂼ��̃o�t(�g��Ȃ���E�����邽�߃X�^�u)
    virtual void Buff(PlayerBase& _target) {}
    void BuffPerAdd(STATUSBUFF_TYPE _type, float _per);
    //�ړ��֘A
     //-------------------------------------
    //�ړ�����
    void Move(float _deg, VECTOR _axis);

    //��������
    void Turn(float _deg, VECTOR _axis);

    //�����Ă邩�ǂ���
    bool IsMove(void) { return speed_ > 0.0f; }

    //�X�L���g�p�\���ǂ���
    bool IsSkillable(void);

    //�X�L���ύX����
    void SkillChange(void);

    //�o�t�X�V
    void BuffUpdate(void);

    //*****************************************************
    //�Q�b�^
    //*****************************************************
    //�N�[����
    const bool GetIsCool(ATK_ACT _act) { return isCool_[static_cast<int>(_act)]; }

    //�N�[���^�C��
    const float GetCoolTime(ATK_ACT _act) { return coolTime_[static_cast<int>(_act)]; }

    //���݂̎g�������X�L��
    const ATK_ACT GetSkillNo(void) { return skillNo_; }

    //�U��������
    const bool GetIsAtk(void) { return isAtk_; }

    //�X�L��������
    const bool GetIsSkill(void) { return isSkill_; }

    //�O��
    const float GetAtkStartCnt(void) { return atkStartCnt_; }

    //�A�j���[�V�����X�e�b�v
    const float GetStepAnim(void) { return stepAnim_; }

    //���
    PlayerDodge* GetDodge(void) { return dodge_; }

    //�N�[���^�C�������̃Q�b�^
    const float* GetCoolTimePer(void) { return coolTimePer_; }

    //�o�t����Ă��邩�Q�b�^
    //const bool GetIsBuff(void) { return isBuff_; }

    const bool GetIsBuff(SKILL_BUFF _skill) { return buffs_[_skill].isBuff; }

    //��Ȃǂ̉���������̃Q�b�^(Knight��Archer�Ŏg��)
    virtual const ATK GetArrowAtk(const int i) { return ATK(); }
    virtual const ATK GetArrowAtk(const ATK_TYPE _act, const int i) { return ATK(); }


    //����������̌����l��
    virtual const int GetArrowCnt(const int _act) { return 0; }

    //�U���̓Q�b�^
    float GetAtkPow(void) { return atkPow_; }

    SceneManager::ROLE GetRole(void) { return role_; }

    VECTOR GetInitPos(int _num) { return initPos_[_num]; }
    


    //**************************************************************
    //�Z�b�^�[
    //**************************************************************
    //�U���n
    //-------------------------------------------------------------------------------------------------------------------
    //�U���̂��ꂼ��̒l
    void SetAtk(const ATK _atk) { atk_ = _atk; }
    //�O���̃J�E���^�[
    void SetAtkStartCnt(const float _atkStartCnt) { atkStartCnt_ = _atkStartCnt; }

    //�O���̍ő厞�ԃZ�b�^
    void SetAtkStartTime(const float _atkStartTime, const ATK_ACT _act) { atkStartTime_[static_cast<int>(_act)] = _atkStartTime; }

    //�U�����邩�ǂ���
    void SetIsAtk(const bool _isAtk) { isAtk_ = _isAtk; }

    //�U���������t���O
    const bool IsAtkStart(void)const { return 0.0f < atkStartCnt_ && atkStartCnt_ <= atkStartTime_[static_cast<int>(act_)]; }

    //�U�����������̂��m�F����
    const bool IsFinishAtkStart(void)const { return atkStartCnt_ > atkStartTime_[static_cast<int>(act_)]; }

    //�U���ύX�p(��ɓ��͂��ꂽ��ς���悤�ɂ���)
    void ChangeAct(const ATK_ACT _act);

    //�U���̍ő�l�̏�����(�|��Ƃ��̈Ⴄatk�̔z��Ƃ��g���p)
    void ResetParam(ATK& _atk);

    //�ߐڍU����atk�������p
    void ResetParam(void);

    //�U���I�������̏�����
    virtual void InitAtk(void);

    //�������ԃZ�b�^
    void SetDulation(const float _dulation) { atk_.duration_ = _dulation; }

    //�X�L�����邩
    void SetIsSkill(const bool _isSkill) { isSkill_ = _isSkill; }

    //�ړ��\���ǂ���
    void SetMoveAble(const bool _moveAble) { moveAble_ = _moveAble; }
    //�N�[���ɂ��邩�ǂ���
    void SetIsCool(const bool _isCool, const ATK_ACT _act) { isCool_[static_cast<int>(_act)] = _isCool; }

    //�N�[���^�C���Z�b�^
    void SetCoolTime(const float coolTime, ATK_ACT _act) { coolTime_[static_cast<int>(_act)] = coolTime; }

    //�A�[�`���[�̃T�[�`�Z�b�^
    void SetIsSerchArcher(const bool _isSerch) { isSerchArcher_ = _isSerch; }

    //���̑�
    //------------------------------------------------------------------------------------
    //�A�j���[�V�����X�e�b�v
    void SetStepAnim(const float _stepAnim) { stepAnim_ = _stepAnim; }

    //�X�s�[�h
    void SetSpeed(const float _speed) { speed_ = _speed; }

    //�o�t�Z�b�^(���Ԑ����t��)
    //void SetBuff(STATUSBUFF_TYPE _type, float _per,float _second);

    /// <summary>
    /// 
    /// </summary>
    /// <param name="_type"></param>
    /// <param name="_skill"></param>
    /// <param name="_per"></param>
    /// <param name="_second"></param>
    void SetBuff(STATUSBUFF_TYPE _type,SKILL_BUFF _skill, float _per,float _second);

    //���Ԑ����Ȃ�
    //void SetBuff(STATUSBUFF_TYPE _type, float _per);

    //�O�̃X�e�[�^�X�����Z�b�g����
    void SetPreStatus(void);

    //�o�t��������Z�b�^
    //void SetIsBuff(const bool _isBuff) { isBuff_ = _isBuff; }

    /// <summary>
    /// �o�t�������Z�b�^
    /// </summary>
    /// <param name="_isBuff">true:�o�t����</param>
    /// <param name="_skill">�o�t�����X�L���͉���</param>
    void SetIsBuff(SKILL_BUFF _skill,const bool _isBuff) { buffs_[_skill].isBuff = _isBuff; }

    void SetIsBuff(PlayerBase& _player,SKILL_BUFF _skill, const bool _isBuff) { _player.SetIsBuff(_skill, _isBuff); }


    //�^�[�Q�b�g�Z�b�^
    void SetTargetPos(const VECTOR _targetPos) { targetPos_ = _targetPos; } 

 

protected:
    //�|�C���^
      //����@�\
    PlayerDodge* dodge_;

    //*************************************************
    // �񋓌^
    //*************************************************
    struct PlayerAtk
    {
        ATK_ACT act_;
        float atkStartCnt_;
        ATK atk_;
        float CoolTime_[static_cast<int>(ATK_ACT::MAX)];
        float CoolTimeMax_[static_cast<int>(ATK_ACT::MAX)];                //�N�[���^�C���ő�
        std::map<ATK_ACT, ATK>atkMax_;
        float atkStartTime_[static_cast<int>(ATK_ACT::MAX)];            //�U����������
        bool IsAtkStart(void) { return 0.0f < atkStartCnt_ && atkStartCnt_ <= atkStartTime_[static_cast<int>(act_)]; }
    };


    //*************************************************
    // �����o�ϐ�
    //*************************************************
    //�V�X�e���n
    //�X�e�[�^�X�n
    ATK_ACT act_;                                               //�U�����
    std::map < ATK_ACT, std::function<void(void)>>changeAct_;   //�U���̕ύX
    std::function<void(void)>actUpdate_;                        //�U�����Ƃ̍X�V����
    float atkStartCnt_;                                         //�U������������܂ł̃J�E���g
    ATK_TYPE atkTypes_[static_cast<int>(ATK_ACT::MAX)];         //�U���̃^�C�v(�`���[�W���邩���Ȃ���)
    std::map<ATK_TYPE, std::function<void(void)>>changeAtkType_;//�U���^�C�v�ύX
    std::function<void(void)>atkTypeUpdate_;                    //�U���^�C�v���Ƃ̃A�b�v�f�[�g
    ATK_TYPE atkType_;                                          //�^�C�v�ϐ�
    float coolTime_[static_cast<int>(ATK_ACT::MAX)];            //���ꂼ��̃N�[���^�C���J�E���g
    bool isCool_[static_cast<int>(ATK_ACT::MAX)];               //���ꂼ��̍U���g���I�����i�[����
    float multiHitInterval_;                                    //���i�q�b�g�̃_���[�W�Ԋu
    VECTOR userOnePos_;                                         //���[�U�[1�Ǐ]�p�̍��W   
    VECTOR colPos_;                                             //�v���C���[�̓����蔻����W
    bool isSerchArcher_;                                              //�A�[�`���[�̎˒������ɓ��������ǂ���
    float speed_;                                               //�v���C���[�̃X�s�[�h(�X�e�[�^�X�ł͂Ȃ�1�t���[���ɓ�������)



    //�N���^�[�Q�b�g�ɂ��邩
    VECTOR targetPos_;

 


    bool isPush_;                                               //�������X�L���p�̃{�^���������Ă��邩�ǂ���  true:�����Ă���
    bool moveAble_;             //�ړ��\����Ԃ�  true:�ړ��\
    bool isAtk_;                                                 //�ʏ�U���J�n�������ǂ���
    bool isSkill_;                                                 //�X�L���J�n�������ǂ���
    bool isBuff_;                                               //�o�t�������łȂ���     true�F�o�t�ł���




    //���ꂼ��̍ő�l�Z�b�g�p(�U���̍��W�̓��[�J�����W�Őݒ肵�Ă܂�)
    std::map<ATK_ACT, ATK>atkMax_;
    float coolTimeMax_[static_cast<int>(ATK_ACT::MAX)];             //�N�[���^�C���ő�
    float atkStartTime_[static_cast<int>(ATK_ACT::MAX)];            //�U����������

    //�R���g���[���[�n
    InputManager::JOYPAD_NO padNum_;                //�Q�[���p�b�h�̔ԍ�
    int leftStickX_;            //�p�b�h�̃X�e�B�b�N��X�p�x  
    int leftStickY_;            //�p�b�h�̃X�e�B�b�N��Y�p�x
    float stickDeg_;            //�p�b�h�̃X�e�B�b�N�̊p�x


    //*************************************************
    //�����o�֐�
    //*************************************************
    VECTOR GetTargetVec(VECTOR _targetPos,bool _isMove=true);



    //�A�j��No������
    void InitAnimNum(void);

    //�������ƂɈႤ�A�j���[�V�����̏�����
    virtual void InitCharaAnim(void);

    //���ꂼ��̃A�N�V�����̏�����
    virtual void InitAct(void);

    //�X�L���ύX���̏�����
    void ChangeNmlAtk(void);
    void ChangeSkillOne(void);
    void ChangeSkillTwo(void);

    //USER�֌W
    //------------------------------------------------
    //���[�U�[������Ƃ��̍X�V
    void UserUpdate(void);

    SceneManager::ROLE role_;

    ATK_ACT skillNo_;     //�X�L���ύX�p


    //�v���C���[(CPU�ƃ��[�U�[)���ʏ���
    //--------------------------------------------------
    //�U������
    virtual void AtkFunc(void) = 0;
    //�X�L��1
    virtual void Skill1Func(void) = 0;
    //�X�L��2
    virtual void Skill2Func(void) = 0;

    //�U�����W�̓���
    void SyncActPos(ATK& _atk);

    //�X�L�����Ƃ̑���X�V
    void ChangeSkillControll(ATK_ACT _skill);

    //�U��
    //-------------------------------------
    //�U�������ǂ���(UnitBase�ŏC���\��)
    const bool IsAtkAction(void)const { return IsAtkStart() || atk_.IsAttack() || atk_.IsBacklash(); }

    //�U���\���ǂ���(true:�\)
    const bool IsAtkable(void)const;


    //����֘A
    //---------------------------------------
    //����\��
    //��𒆂��ǂ���

    const bool IsDodgeable(void)const;

    //�X�L��
    //virtual void Skill(void);
    /// <summary>
    /// �X�L�����t���O
    /// </summary>
    /// <param name="_frameSkillNo">�X�L���t���[���ő�l(���̓X�L��1��2)</param>
    /// <returns>�X�L����/�X�L�����łȂ�</returns>
    bool IsSkillAction(SKILL_NUM _num) {
        return atk_.IsAttack()
            || atk_.IsBacklash();
    }

    //�N�[���^�C���̃J�E���g
    void CoolTimeCnt(void);

private:
    //�����o�ϐ�
    float coolTimePer_[static_cast<int>(ATK_ACT::MAX)];

#ifdef DEBUG_INPUT

    //******************************************************************************************
#endif // DEBUG_ON

    //�A�N�V�����̔�������
    void ProcessAct(void);
    void CoolTimePerCalc(void);
    //std::vector <ATK>atks_;

    std::map<ATK_TYPE, std::vector<ATK>>atks_;

    std::map<SKILL_BUFF, BUFF>buffs_;
    std::map<STATUSBUFF_TYPE, float>buffpers_;

    //�o�t�֌W
    float buffCnt_[static_cast<int>(STATUSBUFF_TYPE::MAX)];                                          //�o�t�̃J�E���^�[(�U���́A�h��́A�X�s�[�h)  
    float buffPercent_[static_cast<int>(STATUSBUFF_TYPE::MAX)];                                      //�o�t�̉��Z
    STATUSBUFF_TYPE buffType_;                                   
    bool isBuffing_;                                                                             //�o�t�����ǂ���

    float bufAtk_;          //�o�t��̍U����
    float bufDef_;           //�o�t��̖h���
    float bufSpd_;           //�o�t��̃X�s�[�h

    float preAtkPow_;          //�o�t��̍U����
    float preDef_;           //�o�t��̖h���
    float preSpd_;           //�o�t��̃X�s�[�h

    VECTOR initPos_[PlayerManager::PLAYER_NUM];     //�v���C���[�����̏������W���i�[



    //�U������
    virtual void NmlAtkInit(void) = 0;
    //�X�L������
    virtual void SkillOneInit(void) = 0;
    virtual void SkillTwoInit(void) = 0;

};