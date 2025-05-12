#include"../../Arrow.h"
#include "Knight.h"

Knight::Knight(void)
{
	
}
void Knight::SetParam(void)
{
	trans_.SetModel(
		ResourceManager::GetInstance()
		.LoadModelDuplicate(ResourceManager::SRC::PLAYER_KNIGHT));
	float scale = CHARACTER_SCALE;
	trans_.scl = { scale, scale, scale };
	trans_.pos = { -300.0f, 0.0f, 0.0f };
	trans_.quaRot = Quaternion();
	trans_.quaRotLocal = Quaternion::Euler(
		0.0f, AsoUtility::Deg2RadF(180.0f),
		0.0f
	);

	ResetAnim(ANIM::IDLE, SPEED_ANIM_IDLE);

	//�X�e�[�^�X
	ParamLoad(CharacterParamData::UNIT_TYPE::KNIGHT);

	//��E�̊��蓖��
	role_ = SceneManager::ROLE::KNIGHT;
	
	//���ߍU���G�t�F�N�g���[�h
	auto& effIns = EffectManager::GetInstance();
	auto& resIns = ResourceManager::GetInstance();
	using EFFECT = EffectManager::EFFECT;
	effIns.Add(EFFECT::GUARD, resIns.Load(ResourceManager::SRC::GUARD).handleId_);

}

void Knight::Update(void)
{
	PlayerBase::Update();

	size_t arrowSize = arrow_.size();
	//��Ɩ�ɑΉ������U���̍X�V
	for (int a = 0; a < arrowSize; a++)
	{
		if (arrow_[a].get()->GetIsAlive())
		{
			CntUp(slashArrow_.cnt_);
		}
		//�U����Ԃ��I���������j��
		if (!slashArrow_.IsAttack())
		{
			arrow_[a].get()->Destroy();
			InitSlashAtk(slashArrow_);
			isShotArrow_ = false;
		}
		//�X�V
		arrow_[a].get()->Update(slashArrow_);
	}
}

void Knight::Draw(void)
{
	PlayerBase::Draw();
	size_t arrowSize = arrow_.size();
	for (auto& arrow : arrow_)
	{
		arrow.get()->Draw();
	}
}

void Knight::InitAct(void)
{
	//�ʏ�U���̍ő�l
	atkMax_.emplace(ATK_ACT::ATK, ATK_MAX);

	//�X�L���P�̍ő�l
	atkMax_.emplace(ATK_ACT::SKILL1, SKILL_ONE_MAX);

	//�X�L���Q�̍ő�l
	atkMax_.emplace(ATK_ACT::SKILL2, SKILL_TWO_MAX);

	//�a��
	slashArrow_ = SLASH_MAX;



	//�N�[���^�C��
	coolTimeMax_[static_cast<int>(ATK_ACT::ATK)] = ATK_COOLTIME;
	coolTimeMax_[static_cast<int>(ATK_ACT::SKILL1)] = SKILL_ONE_COOLTIME;
	coolTimeMax_[static_cast<int>(ATK_ACT::SKILL2)] = SKILL_TWO_COOLTIME;

	//�U����������
	atkStartTime_[static_cast<int>(ATK_ACT::ATK)] = ATK_START;
	atkStartTime_[static_cast<int>(ATK_ACT::SKILL1)] = SKILL_ONE_START;
	atkStartTime_[static_cast<int>(ATK_ACT::SKILL2)] = SKILL_TWO_START;



}


void Knight::AtkFunc(void)
{
	if (isSkill_)return;
	auto& ins = PlayerInput::GetInstance();
	using ACT_CNTL = PlayerInput::ACT_CNTL;

	if (!isAtk_)return;

	if (IsAtkStart())
	{
		moveAble_ = false;
		CntUp(atkStartCnt_);
		if (IsFinishAtkStart())
		{
			CntUp(atk_.cnt_);
		}
	}
	else if (IsFinishAtkStart())
	{
		if ((atk_.IsAttack() || atk_.IsBacklash()))
		{
			CntUp(atk_.cnt_);
			//�N�[���^�C���̏�����
			coolTime_[static_cast<int>(act_)] = 0.0f;
		}
		else if(atk_.IsFinishMotion())/*�������ƒʏ�A�ł̎��Ƀo�O��*/
		{
			InitAtk();
			isAtk_ = false;
		}
	}
}


void Knight::NmlAtkInit(void)
{
}

void Knight::SkillOneInit(void)
{
	//�X�L�����ƂɃA�j���[�V���������߂āA�J�E���g�J�n
	ChangeAct(static_cast<ATK_ACT>(skillNo_));
	ResetParam(atk_);
	CntUp(atkStartCnt_);
}

void Knight::SkillTwoInit(void)
{

}


void Knight::InitSkill(ATK_ACT _act)
{
	//�X�L�����ƂɃA�j���[�V���������߂āA�J�E���g�J�n
	if (isAtk_ || isSkill_ || isCool_[static_cast<int>(_act)])return;
	ChangeAct(static_cast<ATK_ACT>(_act));
	ResetParam(atk_);
	CntUp(atkStartCnt_);
	moveAble_ = false;
	isSkill_ = true;
}

void Knight::Skill1Func(void)
{
	if (isAtk_)return;
	//�a����΂�
	auto& ins = PlayerInput::GetInstance();
	using ACT_CNTL = PlayerInput::ACT_CNTL;
	//��������A�[�`���[�쐬����I
	if (IsFinishAtkStart() && !isShotArrow_)
	{
		if (hp_ == hpMax_)
		{
			CreateSlash();
			//CreateAtk();
			isShotArrow_ = true;
		}
	}

	//�ߐڍU���p(atk_�ϐ��Ɖ������ŕ�����)
	if (IsAtkStart())
	{
		moveAble_ = false;
		CntUp(atkStartCnt_);
		if (IsFinishAtkStart())
		{
			CntUp(atk_.cnt_);
		}
	}
	else if (IsFinishAtkStart())
	{
		if ((atk_.IsAttack() || atk_.IsBacklash()))
		{
			CntUp(atk_.cnt_);
			//�N�[���^�C���̏�����
			coolTime_[static_cast<int>(act_)] = 0.0f;
		}
		else if (atk_.IsFinishMotion())/*�������ƒʏ�A�ł̎��Ƀo�O��*/
		{
			InitAtk();
			isSkill_ = false;
		}
	}

}

void Knight::Skill2Func(void)
{
	if (!isSkill_)return;
	//�{�^�����������Ă���Ƃ��ɃN�[���^�C����0�b�ȉ��ɂȂ�����
	if (coolTime_[static_cast<int>(SKILL_NUM::TWO)] <= 0.0f)
	{
		//�X�L���؂�ւ��o���Ȃ��悤�ɂ���
		isSkill_ = false;
		isCool_[static_cast<int>(SKILL_NUM::TWO)] = true;
		InitAtk();
		return;
	}
	if (isSkill_&&!IsFinishAtkStart())CntUp(atkStartCnt_);

	else if (isSkill_ && IsFinishAtkStart())
	{
		EffectManager::GetInstance().Stop(EffectManager::EFFECT::HIT2);
		CntUp(atk_.cnt_);
		CntDown(coolTime_[static_cast<int>(SKILL_NUM::TWO)]);
		//�X�L���Q�p�����͖h��͂Q�{�ɂȂ�
		SetBuff(STATUSBUFF_TYPE::DEF_BUFF, SKILL_BUFF::GUARD, 100.0f, 0.1f);

		SetIsBuff(SKILL_BUFF::GUARD, true);
		//�ŏ���1�t���[�������̏���
		if (atk_.cnt_ <= 1.0f / 60.0f)
		{
			auto& efeIns = EffectManager::GetInstance();
			efeIns.Play(
				EffectManager::EFFECT::GUARD,
				atk_.pos_,
				Quaternion(),
				GUARD_EFFECT_SIZE,
				SoundManager::SOUND::NONE);
		}
	
	}
	//�{�^���������Ă��Ă��c��N�[���^�C����3�b�ȉ���������
	else if (coolTime_[static_cast<int>(SKILL_NUM::TWO)] <= SKILL_TWO_START_COOLTIME)
	{
		isCool_[static_cast<int>(SKILL_NUM::TWO)] = true;
		InitAtk();
		isSkill_ = false;
		return;
	}
}

void Knight::InitCharaAnim(void)
{
	animNum_.emplace(ANIM::UNIQUE_1, ATK_NUM);
	animNum_.emplace(ANIM::UNIQUE_2, BLOCKING_NUM);
	animNum_.emplace(ANIM::SKILL_1, SKILL_ONE_NUM);
	animNum_.emplace(ANIM::SKILL_2, SKILL_TWO_NUM);
}

void Knight::InitSlashAtk(ATK& arrowAtk)
{
	//�U���J�E���g������
	arrowAtk.ResetCnt();

	SyncActPos(arrowAtk);

	//��������
	arrowAtk.isHit_ = false;
}

void Knight::CreateSlash(void)
{
	//��̐�������
//�g���I������U��������ꍇ
	for (auto& arrow : arrow_)
	{
		// �j���Ԃ̂Ƃ�
		if (arrow->GetState() == Arrow::STATE::DESTROY)
		{
			//��̏����㏑��
			arrow = nullptr;

			// ����
			arrow = std::make_shared<Arrow>();

			// ������
			arrow->Init(arrowMdlId_, trans_, SLASH_SPEED);
			InitSlashAtk(slashArrow_);

			arrow->ChangeState(Arrow::STATE::SHOT);

			//�J�E���g����
			arrowCnt_++;

			return;
		}
	}


	//�V�������ꍇ
	//�V�����z���ǉ�
	std::shared_ptr<Arrow> arrow = std::make_shared<Arrow>();
	arrow->Init(arrowMdlId_, trans_, SLASH_SPEED);
	InitSlashAtk(slashArrow_);
	arrow->ChangeState(Arrow::STATE::SHOT);

	//�z��Ɋi�[
	arrow_.emplace_back(arrow);

	//�J�E���g����
	arrowCnt_++;
}

void Knight::CreateSlashAtk(void)
{

}

#ifdef DEBUG_ON
void Knight::DrawDebug(void)
{
	PlayerBase::DrawDebug();
	DrawFormatString(0, 200, 0xffffff
		, "AtkCooltime(%.2f)\nSkill1Cool(%.2f)\nSkill2Cool(%.2f)\natkDulation(%f)\natkCnt(%f)"
		, coolTime_[static_cast<int>(ATK_ACT::ATK)]
		, coolTime_[static_cast<int>(ATK_ACT::SKILL1)]
		, coolTime_[static_cast<int>(ATK_ACT::SKILL2)]
		, atk_.duration_
		, atkStartCnt_);
}
#endif // DEBUG_ON

