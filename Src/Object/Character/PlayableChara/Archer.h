#pragma once

#include "PlayerBase.h"
class Arrow;
class Archer :
    public PlayerBase
{
public:
#ifdef DEBUG_ON
    void DrawDebug(void)override;
#endif // DEBUG_ON

    //�X�e�[�^�X
    static constexpr int ATK_POW = 90;
    static constexpr int MAX_HP = 235;
    static constexpr int MAX_DEF = 90;
    static constexpr float SPEED = PlayerBase::MOVE_SPEED_FAST;

    //�o�t�����l
    static constexpr float BUFF = 20.0f;

    static constexpr float ATK_START_RANGE = 250.0f * CHARACTER_SCALE;	//�U���J�n����̑傫��
    static constexpr float ATKABLE_TIME = 0.0f;                         //�A�ˎ�t����

    //�G�t�F�N�g�T�C�Y
    static constexpr float ARROW_EFFECT_SIZE = 20.0f;
    //�ʏ�U��
    //-----------------------------------------------------------
    static constexpr float ATK_COOLTIME = 0.3f;
    static constexpr float ATK_START = 0.3f;
    static constexpr float FRAME_ATK_DURATION = 2.0f;
    static constexpr float FRAME_ATK_BACKRASH = 0.1f;
    static constexpr VECTOR ATK_COL_LOCAL_POS = { 0.0f,100.0f,100.0f };
    static constexpr float NMLATK_POW = 7.0f;

    static constexpr float COL_ATK = CHARACTER_SCALE * 100.0f;

    //�v���C���[���̂̌㌄
    static constexpr float NMLATK_BACKRASH_MAX = 0.2f;
    //static constexpr float NMLATK_BACKRASH_MAX = 100.0f;


    //-----------------------------------------------------------



    //�X�L��1
    //-----------------------------------------------------------
    static constexpr float SKILL_ONE_COOLTIME = 3.0f;
    static constexpr float SKILL_ONE_START = 2.0f;

    static constexpr float SKILL_ONE_CHARE_STARTCNT = 1.0f;

    static constexpr float FRAME_SKILL1_DURATION = 0.7f;
    static constexpr float FRAME_SKILL1_BACKRASH = 0.2f;
    static constexpr VECTOR SKILL1_COL_LOCAL_POS = { 0.0f,100.0f,100.0f };
    static constexpr float COL_SKILL1 = CHARACTER_SCALE * 150.0f;

    //���߂����Ɨ��߂Ȃ��������̈З�
    static constexpr float SKILL_ONE_POW_MIN = 15.0f;
    static constexpr float SKILL_ONE_POW_MAX = 25.0f;

    //���߃A�j���[�V�����̃X�e�b�v
    static constexpr float SKILL_CHARGE_STEPANIM = 9.5f;

    //�v���C���[���̂̌㌄
    static constexpr float SKILL1_BACKRASH_MAX = 0.5f;





    //-----------------------------------------------------------

    //�X�L��2
    //-----------------------------------------------------------
    static constexpr float SKILL_TWO_COOLTIME = 5.0f;
    static constexpr float SKILL_TWO_START = 0.2f;
    static constexpr float FRAME_SKILL2_DURATION = 3.0f - SKILL_TWO_START;
    static constexpr float FRAME_SKILL2_BACKRASH = 0.2f;
    static constexpr VECTOR SKILL2_COL_LOCAL_POS = { 0.0f,100.0f,0.0f };
    static constexpr float COL_SKILL2 = CHARACTER_SCALE * 200.0f;
    static constexpr float SKILL_TWO_POW = 0.0f;


    static constexpr float SKILL2_CHANGE_ANIM_TIME = 0.25f;
    static constexpr float SKILL2_BUFF_TIME = 20.0f;
    static constexpr float SKILL2_BACKRASH_MAX = 0.2f;


    //-----------------------------------------------------------
    // 
    //�ŗL�A�j���[�V����
    static constexpr int ATK_NUM = 16;
    static constexpr int SKILL_ONE_NUM = 16;
    static constexpr int SKILL_TWO_NUM = 16;
    //static constexpr int AIMING_NUM = 14;

    //�ʏ�U���̍ő�l
    static constexpr ATK ATK_MAX{ ATK_COL_LOCAL_POS,COL_ATK,NMLATK_POW,FRAME_ATK_DURATION,FRAME_ATK_BACKRASH,0.0f,false };

    //�X�L���P
    static constexpr ATK SKILL_ONE_MAX{ SKILL1_COL_LOCAL_POS ,COL_SKILL1 ,SKILL_ONE_POW_MIN,FRAME_SKILL1_DURATION ,FRAME_SKILL1_BACKRASH ,0.0f,false };

    //�X�L���Q
    static constexpr ATK SKILL_TWO_MAX{ SKILL2_COL_LOCAL_POS ,COL_SKILL2 ,SKILL_TWO_POW,FRAME_SKILL2_DURATION ,FRAME_SKILL2_BACKRASH ,0.0f,false };


    //�|��֌W
    static constexpr int ARROW_SIZE_MAX = 5;	//��̍ő�ێ���
    static constexpr float RELOAD_TIME = 5.0f;	//��̃����[�h����
    static constexpr float ARROW_SPEED = 10.0f;	//��̂ƂԃX�s�[�h

    //�G�t�F�N�g
    //�X�L���`���[�W
    static constexpr float CHARGE_SKILL_EFF_SIZE = 50.0f;
 

    Archer(void);
    ~Archer(void) = default;
    void SetParam(void)override;

    //�e�A�N�V�����̏�����
    void InitAct(void)override;

    //�ŗL�A�j���[�V�����ԍ��̏�����
    void InitCharaAnim(void)override;

    //�U�����I�������̏�����
    void InitAtk(void)override;

    //�|��̍쐬
    void CreateArrow(ATK_TYPE _type);

    //arrowAtk�̍쐬
    //void CreateAtk(void);
    void CreateAtk(ATK_TYPE _type);

    //��̃Q�b�^
    //const PlayerBase::ATK GetArrowAtk(const int i)override;
    const PlayerBase::ATK GetArrowAtk(const ATK_TYPE _type, const int i)override;
    const int GetArrowCnt(const int _type)override { return arrow_[static_cast<ATK_TYPE>(_type)].size(); }
    void SetIsArrowHit(ATK_TYPE _type, const bool _flg, int _num)override;

    //�o�t
    void Buff(PlayerBase& _target)override;

    //�q�b�g�̃Z�b�^
    //void SetIsArrowHit(const bool _flg, ATK _atk)override { _atk.isHit_ = _flg; }

    //std::vector<ATK>GetAtks(void)override { return arrowAtk_; }

    std::vector<ATK> GetAtks(ATK_TYPE _type)override { return arrowAtk_[_type]; }
    void Destroy(void)override;

    void Update(void)override;

    void Draw(void)override;

protected:
    void AtkFunc(void)override;
    void Skill1Func(void)override;
    void Skill2Func(void)override;

    void NmlAtkInit(void)override;
    void SkillOneInit(void)override;
    void SkillTwoInit(void)override;

    //��Ɩ�ɑΉ������U���ɑΉ�����X�V����(�X�L���ɖ��O��t���Ă���Ă݂�)
    void ArrowUpdate(ATK_TYPE _type);

    //�|�A�˗p
    //void NmlActCommon (void)override;

    //�|����atk������
    void InitArrowAtk(ATK& arrowAtk);

    //std::vector<ATK>arrowAtk_;

    std::map<ATK_TYPE, std::vector<ATK>>arrowAtk_;

    int arrowMdlId_;							//��̃��f��
    //std::vector<std::shared_ptr<Arrow>> arrow_;	//�|��
    std::map<ATK_TYPE, std::vector<std::shared_ptr<Arrow>>> arrow_;	//�|��
    //std::map<ATK_ACT, std::shared_ptr<Arrow>>arrow_;
    bool isShotArrow_;							//�����������̔���(true:������)

    //�e�U���̖�̌��J�E���g
    int arrowCnt_[static_cast<int>(ATK_TYPE::MAX)];								//��̎g�p���J�E���^
    float reloadCnt_;							//��̃����[�h����

    float atkAbleCnt_;                          //��̔��ˉ\�J�E���g
    float backrashCnt_;                            //�A�[�`���[���̂̌㌄
};

