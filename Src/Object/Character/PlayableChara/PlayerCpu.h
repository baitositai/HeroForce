#pragma once
#include "PlayerBase.h"
class PlayerCpu :
    public PlayerBase
{
public:
    //CPU
 //--------------------------------------------------------
 //�͈͊֌W
    static constexpr float SEARCH_RANGE = 800.0f * CHARACTER_SCALE;		//���G����̑傫��
    static constexpr float SEARCH_RADIUS = 400.0f;

    //�v���C���[�Ǐ]�͈�
    static constexpr float FOLLOW_PLAYER_RADIUS = 100.0f * CHARACTER_SCALE;

    //�Ăяo���ꂽ�Ƃ��̃X�s�[�h
    static constexpr float CALLED_MOVE_SPEED_SCALE = 3.0f;

    enum class CPU_STATE
    {
        NORMAL			//�ʏ�
        , ATTACK		//�U��
        , BREAK			//�x�e
        , MAX
    };

    //�R���X�g���N�^���f�X�g���N�^
    PlayerCpu(void);
    ~PlayerCpu(void)=default;

    void SetParam(void)override;
    void Update(void)override;





    //��ԕύX
    void ChangeState(const CPU_STATE _state);

    //�Q�b�^
   //-----------------------------------------------
   //�U���J�n����
    const int GetAtkStartRange(void) { return atkStartRange_; }

    //���G����
    const float GetSearchRange(void) { return searchRange_; }

    //�v���C���[�ɌĂяo���ꂽ���ǂ���
    const bool GetIsCalledPlayer(void) { return isMove2CallPlayer_; }

    //��ԃQ�b�^
    CPU_STATE GetState(void) { return cpuState_; }

    //�Z�b�^
    //---------------------------------------------------
    //CPU�̏�ԃZ�b�^
    void SetState(const CPU_STATE _state) { cpuState_ = _state; }

    //CPU�̈ړ��Z�b�^
    void SetIsMove(const bool _isMove) { isMove_ = _isMove; }

  

    //�G�T�[�`�Z�b�^
    void SetisEnemySerch(const bool _isEnemySerch) { isEnemySerch_ = _isEnemySerch; }

    //�Ǐ]�Ώۂ��Z�b�g
    void SetTargetPos(const VECTOR _targetPos) { targetPos_ = _targetPos; }

    void CpuReset(void);

private:
    //*************************************************
    //�����o�֐�
    //*************************************************
       //�͈͌n
    int atkStartRange_;     //�U���J�n�͈�
    float searchRange_;      //���G�͈�


    //CPU�n
    CPU_STATE cpuState_;        //���
    std::map < CPU_STATE, std::function<void(void)>> cpuStateChanges_;  //��Ԃ��Ƃ̏�����
    std::function<void(void)> cpuStateUpdate_;                          //��Ԃ��Ƃ̍X�V
    bool isMove_;               //�����Ă��邩�ǂ���
    bool isCall_;               //�v���C���[�ɌĂяo���ꂽ��
    bool isMove2CallPlayer_;    //�����Ăяo���ꒆ���@true:�Ăяo����ăv���C���[�܂ňړ���
    bool isEnemySerch_;         //�G���T�[�`���������ĂȂ���
    float calledMoveSpeed_;     //�v���C���[�ɌĂяo���ꂽ�Ƃ��̉����p�ϐ�
    float moveStartDelay_;      //�ړ��̏��߂�x�点��


    //CPU�̑O�̍U���i�[�p
    ATK_ACT preAtk_;

    //�x�e�J�E���g
    float breakCnt_;



    //�O�ɂ��čs���Ă��^�[�Q�b�g�̕ۑ��p
    VECTOR preTargetPos_;


    //CPU�̃A�b�v�f�[�g
    void CpuStateUpdate(void);
    void CpuActUpdate(ATK_ACT _act);

    //�e�����̏�����
    void CpuChangeNml(void);
    void CpuChangeAtk(void);
    void CpuChangeBreak(void);

    //�e��Ԃ̍X�V
    void CpuNmlUpdate(void);
    void CpuAtkUpdate(void);
    void CpuBreakUpdate(void);

    //CPU�ړ�
    void CpuMove(VECTOR _targetPos);


};

