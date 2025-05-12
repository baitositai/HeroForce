#pragma once
#include "SceneBase.h"
class FazeResult :
    public SceneBase
{
public:

    enum class STATE {
        NOMAL,
        LAST,
    };

    //�����N�̎�ސ�
    static constexpr int RANK_MAX_NUM = 4;
    //�����N�̊e��{��
    static constexpr float BNS_RANK_S = 2.0f;
    static constexpr float BNS_RANK_A = 1.5f;
    static constexpr float BNS_RANK_B = 1.3f;
    static constexpr float BNS_RANK_C = 1.0f;

    //�\���Ԋu
    static constexpr float INTERVEL = 1.25f;

    //�t�H���g�T�C�Y
    static constexpr int FONT_SIZE = 56;
    static constexpr int FONT_HEAD_SIZE = 72;

    //�e��`��ʒu
    static constexpr int HEAD_TEXT_POS_X = Application::SCREEN_SIZE_X / 2;
    static constexpr int HEAD_TEXT_POS_Y = 30;

    static constexpr int MES1_POS_X = 30;
    static constexpr int MES1_POS_Y = 150;

    static constexpr int MES2_POS_X = 30;
    static constexpr int MES2_POS_Y = 300;

    static constexpr int MES3_POS_X = 30;
    static constexpr int MES3_POS_Y = 450;

    static constexpr int RANK_POS_X = Application::SCREEN_SIZE_X - ResourceManager::RANK_SIZE / 2;
    static constexpr int RANK_POS_Y = Application::SCREEN_SIZE_Y - ResourceManager::RANK_SIZE / 2;

    //�]���l�֌W
    static constexpr int CHICKEN_VALUE = 10;    //�j���g�������̕]���l(��C��)
    static constexpr int ENEMY_VALUE = 5;       //�G��|�����Ƃ��̉��l(��̕�)

    static constexpr int RANK_S_BORDER = 170;
    static constexpr int RANK_A_BORDER = 140;
    static constexpr int RANK_B_BORDER = 100;


    enum class RANK {
        C,
        B,
        A,
        S,
        MAX
    };

    FazeResult(void);
    ~FazeResult(void);


    void Init(void) override;
    void Update(void) override;
    void Draw(void) override;
    void Release(void) override;

    //�I�����Ă��邩
    const bool IsEnd(void) { return isEnd_; };
    //���Z�b�g
    void Reset(void);
    //�ŏI�̃��U���g
    void SetLast(void);

    //���U���g���Z�b�g
    void SetResult(void);

    float GetExp(void);

private:

    //�X�e�[�g
    STATE state_;

    //�摜�i�[
    int rankImg_[RANK_MAX_NUM]; //�]��UI
    int backImg_;   //�w�i

    //�摜
    int imgBack_;
    int* imgRank_;

    //�t�H���g
    int heasdFont_;
    int font_;

    //�`��J�E���g
    float step_;

    int dunkEnm_;   //�|�����G�̐�
    int aliveChicken_;  //�����j���g��

    float exp_; //��b�o���l
    float afterExp_;//�v�Z��̌o���l
    RANK rank_; //�����N
    bool isEnd_;//�I�����Ă��邩

    bool isPlay_;

    //�o���l�̔{�����v�Z�������̂��擾
    float GetBonusExp(const RANK _rank)const;

    //�����N�̕ύX(�f�o�b�O�p)
    void ChangeRank(void);

    //�����N�̌���
    void JudgeRank(void);
};