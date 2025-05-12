#include "ChickenBase.h"

ChickenBase::ChickenBase()
{
	imgHelp_ = -1;
	moveSpeed_ = -1.0f;
	fadeStep_ = -1.0f;
	state_ = STATE::NONE;
	aliveState_ = ALIVE_MOVE::MAX;
	targetPos_ = AsoUtility::VECTOR_ZERO;
	isHelp_ = false;
	isHelpCnt_ = 0.0f;
	smokeNum_ = -1;
	smokeStep_ = -1.0f;
	efeSpeed_ = -1.0f;

	int i = -1;
	imgSmoke_ = &i;

	// ��ԊǗ�
	stateChanges_.emplace(STATE::NONE, std::bind(&ChickenBase::ChangeStateNone, this));
	stateChanges_.emplace(STATE::ALIVE , std::bind(&ChickenBase::ChangeStateAlive, this));
	stateChanges_.emplace(STATE::DAMAGE , std::bind(&ChickenBase::ChangeStateDamage, this));
	stateChanges_.emplace(STATE::DEATH , std::bind(&ChickenBase::ChangeStateDeath, this));
	stateChanges_.emplace(STATE::END , std::bind(&ChickenBase::ChangeStateEnd, this));

	// ������ԊǗ�
	aliveChanges_.emplace(ALIVE_MOVE::IDLE, std::bind(&ChickenBase::ChangeAliveIdle, this));
	aliveChanges_.emplace(ALIVE_MOVE::ROTATION, std::bind(&ChickenBase::ChangeAliveWalk, this));
	aliveChanges_.emplace(ALIVE_MOVE::CALL, std::bind(&ChickenBase::ChangeAliveCall, this));
}

ChickenBase::~ChickenBase()
{
}

void ChickenBase::Create(const VECTOR &_pos)
{
	//���̎󂯎��
	trans_.pos = _pos;
	
	//���f���ݒ�
	ModelSet();

	//���\�[�X�̓ǂݍ���
	Load();

	//�A�j���[�V�����Ǘ��ԍ��̏�����
	InitAnimNum();

	//�p�����[�^�[�ݒ�
	SetParam();

	//HPUI�̐���
	hpUi_ = std::make_unique<CpuHpBar>();
	hpUi_->Init();
}

void ChickenBase::Update()
{
	//�o�b�N�A�b�v
	prePos_ = trans_.pos;	

	//�f�o�b�O����
	//DebagUpdate();

	//�A�j���[�V�����J�E���g
	Anim();

	// �X�V�X�e�b�v
 	stateUpdate_();

	//�c��HP�̏���
	SubHp();

	//HP��0�ȉ��̏ꍇ
	if (hp_ <= 0 && state_ != STATE::END) { ChangeState(STATE::DEATH); }

	//�摜�\���m�F
	CheckIsHelp();

	//UI�ݒ�
	SetUiParam();

	//�g�����X�t�H�[���X�V
	trans_.Update();
}

void ChickenBase::Draw()
{
	stateDraw_();

	//�f�o�b�O�`��
	//DebagDraw();

	//�r���{�[�h�`��
	DrawHelp();
}

void ChickenBase::SetIsHelp()
{
	//�\��
	isHelp_ = true;

	//�\�����Ԃ̐ݒ�
	isHelpCnt_ = IS_HELP_CNT;
}

void ChickenBase::ModelSet()
{
	//���f���̏�����
	trans_.SetModel(
		ResourceManager::GetInstance().
		LoadModelDuplicate(ResourceManager::SRC::CHICKEN));
	trans_.scl = SCALE;
	trans_.quaRot = Quaternion();
	trans_.quaRotLocal = Quaternion::Euler(
		0.0f,
		AsoUtility::Deg2RadF(180.0f),
		0.0f);

	trans_.Update();
}

void ChickenBase::Load()
{
	auto& res = ResourceManager::GetInstance();

	//�w���v�摜
	imgHelp_ = res.Load(ResourceManager::SRC::HELP).handleId_;

	//�X���[�N�摜
	imgSmoke_ = res.Load(ResourceManager::SRC::SMOKE).handleIds_;

	//�G�t�F�N�g�ǉ�
	EffectManager::GetInstance().Add(EffectManager::EFFECT::DAMAGE, 
		res.Load(ResourceManager::SRC::DAMAGE_EFE).handleId_);
}

void ChickenBase::SetParam()
{
	//�X�e�[�^�X�ݒ�
	ParamLoad(CharacterParamData::UNIT_TYPE::CHICKEN);

	// �t�F�[�h����
	fadeStep_ = TIME_FADE;

	//�摜�\��
	isHelp_ = false; 
	isHelpCnt_ = 0.0f;

	//���G�t�F�N�g
	smokeNum_ = 0;
	smokeStep_ =0.0f;
	efeSpeed_ = SMOKE_SPEED;
	
	//�������̍s���������_���Ō��߂�
	aliveState_ = static_cast<ALIVE_MOVE>(GetRand(ALIVE_MOVE_MAX - 1));

	//�������(���������̏�Ԃ����߂Ă���s��)
    ChangeState(STATE::ALIVE);

	//�A�j���[�V�������
	ResetAnim(ANIM::IDLE, DEFAULT_SPEED_ANIM);
}

void ChickenBase::InitAnimNum()
{
	animNum_.emplace(ANIM::IDLE, ANIM_IDLE);
	animNum_.emplace(ANIM::WALK, ANIM_WALK);
	animNum_.emplace(ANIM::DAMAGE, ANIM_DAMAGE);
	animNum_.emplace(ANIM::DEATH, ANIM_DEATH);
	animNum_.emplace(ANIM::UNIQUE_1, ANIM_CALL);
}

void ChickenBase::SetUiParam()
{
	//���W�ݒ�
	VECTOR pos = VAdd(trans_.pos, LOCAL_HP_POS);
	hpUi_->SetPos(pos);

	//HP�ݒ�
	hpUi_->SetHP(hp_);
}

void ChickenBase::ChangeState(const STATE _state)
{
	// ��ԕύX
	state_ = _state;

	// �e��ԑJ�ڂ̏�������
	stateChanges_[state_]();
}

void ChickenBase::ChangeStateNone()
{
	stateUpdate_ = std::bind(&ChickenBase::UpdateNone, this);
	stateDraw_ = std::bind(&ChickenBase::DrawNone, this);
}

void ChickenBase::ChangeStateAlive()
{
	stateUpdate_ = std::bind(&ChickenBase::UpdateAlive, this);
	stateDraw_ = std::bind(&ChickenBase::DrawAlive, this);

	//�������̍s����Ԃ̔��f
	ChangeAliveState(aliveState_);
}

void ChickenBase::ChangeStateDamage()
{
	stateUpdate_ = std::bind(&ChickenBase::UpdateDamage, this);
	stateDraw_ = std::bind(&ChickenBase::DrawDamage, this);

	//�摜�\��
	SetIsHelp();

	//�G�t�F�N�g�Đ�
	VECTOR localPos = { 
		GetRand(static_cast<int>(EFC_CREATE_RANGE.x)),
		GetRand(static_cast<int>(EFC_CREATE_RANGE.y)),
		GetRand(static_cast<int>(EFC_CREATE_RANGE.z)) };
	VECTOR pos = VAdd(trans_.pos, localPos);
	EffectManager::GetInstance().Play(
		EffectManager::EFFECT::DAMAGE,
		pos,
		Quaternion(),
		DAMAGE_EFE_SCALE,
		SoundManager::SOUND::NONE);
}

void ChickenBase::ChangeStateDeath()
{
	stateUpdate_ = std::bind(&ChickenBase::UpdateDeath, this);
	stateDraw_ = std::bind(&ChickenBase::DrawDeath, this);
}

void ChickenBase::ChangeStateEnd()
{
	stateUpdate_ = std::bind(&ChickenBase::UpdateEnd, this);
	stateDraw_ = std::bind(&ChickenBase::DrawEnd, this);
}

void ChickenBase::ChangeAliveState(const ALIVE_MOVE _state)
{
	// ��ԕύX
	aliveState_ = _state;

	// �e��ԑJ�ڂ̏�������
	aliveChanges_[aliveState_]();
}

void ChickenBase::ChangeAliveIdle()
{
	stateAliveUpdate_ = std::bind(&ChickenBase::AliveIdle , this);
}

void ChickenBase::ChangeAliveWalk()
{
	stateAliveUpdate_ = std::bind(&ChickenBase::AliveRotation, this);
}

void ChickenBase::ChangeAliveCall()
{
	stateAliveUpdate_ = std::bind(&ChickenBase::AliveCall, this);
}

void ChickenBase::UpdateNone()
{
	//NONE�̎��͉����s��Ȃ�
}

void ChickenBase::UpdateAlive()
{
   	stateAliveUpdate_();
}

void ChickenBase::UpdateDamage()
{
	//�A�j���[�V������ύX
	ResetAnim(ANIM::DAMAGE, DEFAULT_SPEED_ANIM);
}

void ChickenBase::UpdateDeath()
{	
	//�X�e�b�v�X�V
	float value = SceneManager::GetInstance().GetDeltaTime();
	fadeStep_ -= value;

	if (fadeStep_ < 0.0f)
	{
		//�G�t�F�N�g�A�j���[�V�����ԍ�
		smokeStep_ += value;
		smokeNum_ = static_cast<int>(smokeStep_ * efeSpeed_);
		if (smokeNum_ >= SMOKE_NUM)
		{
			ChangeState(STATE::END);
		}
		return;
	}

	//�A�j���[�V������ύX
	ResetAnim(ANIM::DEATH, DEFAULT_SPEED_ANIM);
}

void ChickenBase::UpdateEnd()
{
	//END�̂Ƃ��͉����s��Ȃ�
}

void ChickenBase::DrawNone()
{
	//�`����s��Ȃ�
}

void ChickenBase::DrawAlive()
{
	MV1DrawModel(trans_.modelId);

	//HPUI�\��
	hpUi_->Draw();
}

void ChickenBase::DrawDamage()
{
	MV1DrawModel(trans_.modelId);

	//HPUI�\��
	hpUi_->Draw();
}

void ChickenBase::DrawDeath()
{
	// ���Ԃɂ��F�̐��`���
	float diff = TIME_FADE - fadeStep_;
	auto c = AsoUtility::Lerp(FADE_C_FROM, FADE_C_TO, (diff / TIME_FADE));
	// ���f���̃}�e���A�����擾
	int num = MV1GetMaterialNum(trans_.modelId);
	for (int i = 0; i < num; i++)
	{
		// ���f���̃f�B�t���[�Y�J���[��ύX
		MV1SetMaterialDifColor(trans_.modelId, i, c);
	}
	// ���f���̕`��
	MV1DrawModel(trans_.modelId);

	//HPUI�\��
	hpUi_->Draw();

	//���G�t�F�N�g2D
	if (fadeStep_ < 0.0f) {
		VECTOR pos = VAdd(trans_.pos, LOCAL_SMOKE_POS);
		DrawBillboard3D(
			pos,
			SMOKE_CENTER_POS, SMOKE_CENTER_POS,
			SMOKE_SCALE,
			0.0f,
			imgSmoke_[smokeNum_],
			true);
	}
}

void ChickenBase::DrawEnd()
{
	//�`����s��Ȃ�
}

void ChickenBase::AliveIdle()
{
	//�^�[�Q�b�g��_��
	LookTarget();

	//�A�j���[�V������������
	ResetAnim(ANIM::IDLE, DEFAULT_SPEED_ANIM);
}

void ChickenBase::AliveRotation()
{
	ResetAnim(ANIM::WALK, DEFAULT_SPEED_ANIM);

	VECTOR addAxis = AsoUtility::VECTOR_ZERO;
	addAxis.y = 0.8f;

	if (!AsoUtility::EqualsVZero(addAxis))
	{
		//�����]����������]�ʂ��N�H�[�^�j�I���ō��
		Quaternion rotPow = Quaternion();
		rotPow = rotPow.Mult(
			Quaternion::AngleAxis(
				AsoUtility::Deg2RadF(addAxis.x),
				AsoUtility::AXIS_X));

		rotPow = rotPow.Mult(
			Quaternion::AngleAxis(
				AsoUtility::Deg2RadF(addAxis.y),
				AsoUtility::AXIS_Y));

		rotPow = rotPow.Mult(
			Quaternion::AngleAxis(
				AsoUtility::Deg2RadF(addAxis.z),
				AsoUtility::AXIS_Z));

		// ��]�ʂ�������(����)
		trans_.quaRot = trans_.quaRot.Mult(rotPow);
	}

	// �O�������擾
	VECTOR forward = trans_.GetForward();

	// �ړ�
	trans_.pos = VAdd(trans_.pos, VScale(forward, moveSpeed_));
}

void ChickenBase::AliveCall()
{
	//�^�[�Q�b�g�𒍖�
	LookTarget();

	ResetAnim(ANIM::UNIQUE_1, DEFAULT_SPEED_ANIM);
}

void ChickenBase::LookTarget()
{
	//�W�I�ւ̕����x�N�g�����擾						��TODO:�x�N�g����SceneGame������炤
	VECTOR targetVec = VSub(targetPos_, trans_.pos);

	//���K��
	targetVec = VNorm(targetVec);

	//Y���W�͕K�v�Ȃ��̂ŗv�f������
	targetVec = { targetVec.x,0.0f,targetVec.z };

	//��]
	trans_.quaRot = trans_.quaRot.LookRotation(targetVec);
}

void ChickenBase::FinishAnim()
{
	switch (anim_)
	{
	case UnitBase::ANIM::IDLE:
	case UnitBase::ANIM::WALK:
	case UnitBase::ANIM::UNIQUE_1:
		//���[�v�Đ�
		stepAnim_ = 0.0f;
		break;

	case UnitBase::ANIM::DAMAGE:
		ChangeState(STATE::ALIVE);
		break;

	case UnitBase::ANIM::DEATH:
		break;
	}
}

void ChickenBase::CheckIsHelp()
{
	//�摜����\���̏ꍇ�������Ȃ�
	if (!isHelp_) { return; }

	isHelpCnt_ -= SceneManager::GetInstance().GetDeltaTime();

	if (isHelpCnt_ <= 0.0f)
	{
		isHelp_ = false;
	}

}

void ChickenBase::DrawHelp()
{
	//3D����2D�֍��W�ϊ�
	VECTOR pos = VAdd(trans_.pos, LOCAL_HELP_POS);
	pos = ConvWorldPosToScreenPos(pos);

	//�摜�\��
	if (isHelp_ &&
		(state_ == STATE::ALIVE ||
			state_ == STATE::DAMAGE)) {

		DrawRotaGraph(
			pos.x,
			pos.y,
			1.0f,
			0.0f,
			imgHelp_,
			true);
	}
}

void ChickenBase::DebagUpdate()
{
	auto& ins = InputManager::GetInstance();

	//�|��鏈���̊m�F
	if (ins.IsTrgDown(KEY_INPUT_K))
	{
		ChangeState(STATE::DEATH);
	}
	//�_���[�W�����̊m�F
	else if (ins.IsTrgDown(KEY_INPUT_O))
	{
		ChangeState(STATE::DAMAGE);
		//SetDamage(50,);
	}
}

void ChickenBase::DebagDraw()
{
	constexpr int DIV_NUM = 20;
	constexpr int INTERVAL = 16;
	int cnt = 0;
	bool fill = false;
	Vector2 pos = { 0,Application::SCREEN_SIZE_Y - INTERVAL };

	//�p�����[�^�[�̕`��
	DrawFormatString(pos.x, pos.y - INTERVAL * cnt, 0xffffff, "�U����%d", atkPow_);
	cnt++;
	DrawFormatString(pos.x, pos.y - INTERVAL * cnt, 0xffffff, "�h���%d", def_);
	cnt++;
	DrawFormatString(pos.x, pos.y - INTERVAL * cnt, 0xffffff, "�X�s�[�h%d", moveSpeed_);
	cnt++;
	DrawFormatString(pos.x, pos.y - INTERVAL * cnt, 0xffffff, "�̗�%d", hp_);

	//�����蔻��̕`��
	DrawSphere3D(trans_.pos, radius_, DIV_NUM, AsoUtility::RED, AsoUtility::RED, fill);
}
