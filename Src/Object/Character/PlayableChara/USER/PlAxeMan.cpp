#include"../AxeMan.h"
#include"../../PlayerInput.h"
#include "PlAxeMan.h"


PlAxe::PlAxe(const SceneManager::CNTL _cntl)
{
	info_.cntrol_ = _cntl;
}

PlAxe::PlAxe(const InputManager::JOYPAD_NO _padNum)
{
	info_.cntrol_ = SceneManager::CNTL::PAD;
	padNum_ = _padNum;
}

void PlAxe::Init(void)
{
	//�I�u�W�F�N�g�̍쐬
	obj_ = new AxeMan();
	obj_->Init();
	SetInitPos(playerNum_);
	
}

void PlAxe::Update(void)
{
	//�X�V
	obj_->Update();
	if (!obj_->IsAlive())return;

	//�L�[����
	PlayerDodge* dodge = obj_->GetDodge();
	PlayerInput::GetInstance().Update(obj_,padNum_,info_.cntrol_);
	ActionInput(obj_,dodge);

	//�ʏ�U��
	AtkInput();


	//���݂̃X�L���ԍ�(skillNo_)�ɂ���čX�V��ς���
	PlayerBase::ATK_ACT skillNo = obj_->GetSkillNo();
	switch (skillNo)
	{
	case PlayerBase::ATK_ACT::SKILL1:
		SkillOneInput();
		break;
	case PlayerBase::ATK_ACT::SKILL2:
		SkillTwoInput();
		break;
	}
}

void PlAxe::Draw(void)
{
	obj_->Draw();
}

void PlAxe::Release(void)
{
	obj_->Destroy();
	delete obj_;
	obj_ = nullptr;
}

void PlAxe::InitSkill(PlayerBase::ATK_ACT _act)
{
	float deltaTime = 1.0f / Application::DEFAULT_FPS;
	if (obj_->GetIsAtk() || obj_->GetIsSkill())return;
	//�X�L�����ƂɃA�j���[�V���������߂āA�J�E���g�J�n
	obj_->ChangeAct(static_cast<PlayerBase::ATK_ACT>(_act));
	obj_->ResetParam();

	//atkStartCnt���J�n�����邽��
	obj_->SetAtkStartCnt(deltaTime);
	obj_->SetMoveAble(false);
	obj_->SetIsSkill(true);
}

void PlAxe::AtkInput(void)
{
	auto& ins = PlayerInput::GetInstance();
	using ACT_CNTL = PlayerInput::ACT_CNTL;
	using ATK_ACT = PlayerBase::ATK_ACT;
	float deltaTime = 1.0f / Application::DEFAULT_FPS;
	if (obj_->GetIsSkill()||obj_->GetIsCool(PlayerBase::ATK_ACT::ATK))return;
	if (ins.CheckAct(ACT_CNTL::NMLATK) && !obj_->GetIsAtk()&&!obj_->GetIsSkill())
	{
		if (obj_->GetIsCool(ATK_ACT::ATK))return;
		obj_->ChangeAct(ATK_ACT::ATK);
		obj_->ResetParam();
		obj_->SetAtkStartCnt(deltaTime);
		obj_->SetIsAtk(true);
	}
}

void PlAxe::SkillOneInput(void)
{
	auto& ins = PlayerInput::GetInstance();
	using ACT_CNTL = PlayerInput::ACT_CNTL;
	using ATK_ACT = PlayerBase::ATK_ACT;
	float deltaTime = 1.0f / Application::DEFAULT_FPS;
	int skillOne = static_cast<int>(ATK_ACT::SKILL1);
	if(obj_->GetIsCool(PlayerBase::ATK_ACT::ATK))return;
	if (!obj_->GetIsCool(ATK_ACT::SKILL1))
	{
		if (ins.CheckAct(ACT_CNTL::SKILL_DOWN) && !obj_->IsAtkStart())
		{
			InitSkill(ATK_ACT::SKILL1);
		}
		//�X�L��(�������ŃK�[�h��Ԉێ�)
		if (ins.CheckAct(ACT_CNTL::SKILL_KEEP) && obj_->IsAtkStart())
		{
			//�����Ă��锽��
			//CntUp(atkStartCnt_);
		}
		else if (ins.CheckAct(ACT_CNTL::SKILL_UP) && obj_->IsAtkStart())
		{
			if (obj_->GetAtkStartCnt() <= SKILL_ONE_START_NOCHARGE)
			{
				obj_->SetAtkStartTime(SKILL_ONE_START_NOCHARGE, ATK_ACT::SKILL1);
			}
			else
			{
				float atkStartCnt = obj_->GetAtkStartCnt();
				obj_->SetAtkStartTime(atkStartCnt, ATK_ACT::SKILL1);
			}
		}
	}
}

void PlAxe::SkillTwoInput(void)
{
	auto& ins = PlayerInput::GetInstance();
	using ACT_CNTL = PlayerInput::ACT_CNTL;
	using ATK_ACT = PlayerBase::ATK_ACT;
	float deltaTime = 1.0f / Application::DEFAULT_FPS;
	if (obj_->GetIsCool(PlayerBase::ATK_ACT::SKILL2))return;
	if (ins.CheckAct(ACT_CNTL::SKILL_DOWN))
	{
		InitSkill(ATK_ACT::SKILL2);
	}
}
