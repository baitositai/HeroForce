#include "../Manager/Decoration/SoundManager.h"
#include "../Manager/Generic/ResourceManager.h"
#include "../Manager/Generic/InputManager.h"
#include "../Manager/GameSystem/DataBank.h"
#include "../Scene/SelectScene.h"
#include "../Object/Character/PlayableChara/Other/SelectPlayer.h"
#include "SelectImage.h"

SelectImage::SelectImage(SelectScene& select, std::shared_ptr<SelectPlayer> player) : selectScene_(select), player_(player)
{
	imgPlayerNum_ = nullptr;
	imgDisplayNum_ = nullptr;
	imgLeftPoint_ = nullptr;
	imgRightPoint_ = nullptr;
	imgReady_ = nullptr;
	imgRoleNum_ = nullptr;
	imgDeviceNum_ = nullptr;
	imgComingSoon_ = nullptr;

	state_ = SelectScene::SELECT::NUMBER;

	//���W(�l�p�`����邽�߂�2�̎O�p�`���g��)
	mesh_.vertex_[0].pos = AsoUtility::VECTOR_ZERO;		// ����
	mesh_.vertex_[1].pos = AsoUtility::VECTOR_ZERO;		// �E��
	mesh_.vertex_[2].pos = AsoUtility::VECTOR_ZERO;		// ����
	mesh_.vertex_[3].pos = AsoUtility::VECTOR_ZERO;		// �E��

	readyMesh_.vertex_[0].pos = AsoUtility::VECTOR_ZERO;	// ����
	readyMesh_.vertex_[1].pos = AsoUtility::VECTOR_ZERO;	// �E��
	readyMesh_.vertex_[2].pos = AsoUtility::VECTOR_ZERO;	// ����
	readyMesh_.vertex_[3].pos = AsoUtility::VECTOR_ZERO;	// �E��

	pointL_.mesh_.vertex_[0].pos = AsoUtility::VECTOR_ZERO;	// ����
	pointL_.mesh_.vertex_[1].pos = AsoUtility::VECTOR_ZERO;	// �E��
	pointL_.mesh_.vertex_[2].pos = AsoUtility::VECTOR_ZERO;	// ����
	pointL_.mesh_.vertex_[3].pos = AsoUtility::VECTOR_ZERO;	// �E��

	pointR_.mesh_.vertex_[0].pos = AsoUtility::VECTOR_ZERO;	// ����
	pointR_.mesh_.vertex_[1].pos = AsoUtility::VECTOR_ZERO;	// �E��
	pointR_.mesh_.vertex_[2].pos = AsoUtility::VECTOR_ZERO;	// ����
	pointR_.mesh_.vertex_[3].pos = AsoUtility::VECTOR_ZERO;	// �E��

	//UV���W�i�e�N�X�`�����W�j
	mesh_.vertex_[0].u = 0.0f / 4.0f;	mesh_.vertex_[0].v = 1.0f;	// ����
	mesh_.vertex_[1].u = 1.0f / 4.0f;	mesh_.vertex_[1].v = 1.0f;	// �E��
	mesh_.vertex_[2].u = 0.0f / 4.0f;	mesh_.vertex_[2].v = 0.0f;	// ����
	mesh_.vertex_[3].u = 1.0f / 4.0f;	mesh_.vertex_[3].v = 0.0f;	// �E��

	readyMesh_.vertex_[1].u = 1.0f;	readyMesh_.vertex_[1].v = 1.0f;	// ����
	readyMesh_.vertex_[0].u = 0.0f;	readyMesh_.vertex_[0].v = 1.0f;	// �E��
	readyMesh_.vertex_[2].u = 0.0f;	readyMesh_.vertex_[2].v = 0.0f;	// ����
	readyMesh_.vertex_[3].u = 1.0f;	readyMesh_.vertex_[3].v = 0.0f;	// �E��

	pointL_.mesh_.vertex_[0].u = 0.0f;	pointL_.mesh_.vertex_[0].v = 1.0f;	// ����
	pointL_.mesh_.vertex_[1].u = 1.0f;	pointL_.mesh_.vertex_[1].v = 1.0f;	// �E��
	pointL_.mesh_.vertex_[2].u = 0.0f;	pointL_.mesh_.vertex_[2].v = 0.0f;	// ����
	pointL_.mesh_.vertex_[3].u = 1.0f;	pointL_.mesh_.vertex_[3].v = 0.0f;	// �E��

	pointR_.mesh_.vertex_[0].u = 0.0f;	pointR_.mesh_.vertex_[0].v = 1.0f;	// ����
	pointR_.mesh_.vertex_[1].u = 1.0f;	pointR_.mesh_.vertex_[1].v = 1.0f;	// �E��
	pointR_.mesh_.vertex_[2].u = 0.0f;	pointR_.mesh_.vertex_[2].v = 0.0f;	// ����
	pointR_.mesh_.vertex_[3].u = 1.0f;	pointR_.mesh_.vertex_[3].v = 0.0f;	// �E��

	// �@���̐ݒ�i����͏ȗ��A�K���Ȓl��ݒ�j
	for (int i = 0; i < VERTEX_NUM; i++) {
		mesh_.vertex_[i].norm = VGet(0.0f, 0.0f, 1.0f);  //�@����-Z������
		readyMesh_.vertex_[i].norm = VGet(0.0f, 0.0f, 1.0f);
		pointL_.mesh_.vertex_[i].norm = VGet(0.0f, 0.0f, 1.0f);
		pointR_.mesh_.vertex_[i].norm = VGet(0.0f, 0.0f, 1.0f);
	}

	// �F�̐ݒ�i�f�B�t���[�Y�J���[�j
	for (int i = 0; i < VERTEX_NUM; i++) {
		mesh_.vertex_[i].dif = GetColorU8(255, 255, 255, 255);  //���F
		readyMesh_.vertex_[i].dif = GetColorU8(255, 255, 255, 255);
		pointL_.mesh_.vertex_[i].dif = GetColorU8(255, 255, 255, 255); 
		pointR_.mesh_.vertex_[i].dif = GetColorU8(255, 255, 255, 255); 
	}

	// �F�̐ݒ�i�f�B�t���[�Y�J���[�j
	for (int i = 0; i < VERTEX_NUM; i++) {
		mesh_.vertex_[i].spc = GetColorU8(0, 0, 0, 0);  //���F
		readyMesh_.vertex_[i].spc = GetColorU8(0, 0, 0, 0);
		pointL_.mesh_.vertex_[i].spc = GetColorU8(0, 0, 0, 0);
		pointR_.mesh_.vertex_[i].spc = GetColorU8(0, 0, 0, 0);
	}

	leftTop_ = AsoUtility::VECTOR_ZERO;
	leftBottom_ = AsoUtility::VECTOR_ZERO;
	rightTop_ = AsoUtility::VECTOR_ZERO;
	rightBottom_ = AsoUtility::VECTOR_ZERO;

	displayNum_ = -1;
	playerNum_ = -1;
	isPad_ = false;
	role_ = -1;
	isReady_ = false;

	keyPressTime_ = -1.0f;
	interval_ = -1.0f;
	press_ = false;
	angle_ = -1.0f;

	target_[0] = AsoUtility::VECTOR_ZERO;
	target_[1] = AsoUtility::VECTOR_ZERO;
	target_[2] = AsoUtility::VECTOR_ZERO;
	target_[3] = AsoUtility::VECTOR_ZERO;

	// ��ԊǗ�
	stateChanges_.emplace(
		SelectScene::SELECT::DISPLAY, std::bind(&SelectImage::ChangeStateDisplay, this));
	stateChanges_.emplace(
		SelectScene::SELECT::NUMBER, std::bind(&SelectImage::ChangeStateNumber, this));
	stateChanges_.emplace(
		SelectScene::SELECT::OPERATION, std::bind(&SelectImage::ChangeStateOperation, this));
	stateChanges_.emplace(
		SelectScene::SELECT::ROLE, std::bind(&SelectImage::ChangeStateRole, this));
}

void SelectImage::Destroy(void)
{
}

void SelectImage::Init(void)
{
	Load();

	InitVertex();

	displayNum_ = 1;
	playerNum_ = 1;
	isPad_ = false;
	role_ = 1;
	isReady_ = false;
	keyPressTime_ = 0.0f;
	interval_ = 0.0f;
	press_ = false;
	angle_ = 0.0f;

	target_[0] = SelectScene::DEFAULT_TARGET_POS;

	//�f�B�X�v���C���I������
	ChangeSelect(SelectScene::SELECT::DISPLAY);
}

void SelectImage::Update(void)
{
	//�X�V�X�e�b�v
	stateUpdate_();
}

void SelectImage::Draw(void)
{
	switch (selectScene_.GetSelect())
	{
	case SelectScene::SELECT::DISPLAY:
		DisplayDraw();
		break;

	case SelectScene::SELECT::NUMBER:
		NumberDraw();
		break;

	case SelectScene::SELECT::OPERATION:
		OperationDraw();
		break;

	case SelectScene::SELECT::ROLE:
		RoleDraw();
		break;

	default:
		break;
	}

}

void SelectImage::MoveVertexPos(void)
{
	//Lerp�œ����������ȂƂ����C����
	//�t���X�N�p
	//mesh_.vertex_[0].pos = { -70.0f, 100.0f, VERTEX_Z + 12.0f };	//����
	//mesh_.vertex_[1].pos = { 0.0f, 100.0f, VERTEX_Z + 12.0f };		//�E��
	//mesh_.vertex_[2].pos = { -70.0f, 170.0f, VERTEX_Z };			//����
	//mesh_.vertex_[3].pos = { 0.0f, 170.0f, VERTEX_Z };				//�E��

	//pointL_.mesh_.vertex_[0].pos = { POINT_LEFT_X - 40.0f, POINT_UNDER_Y - 10.0f, POINT_TOP_Z };	// ����
	//pointL_.mesh_.vertex_[1].pos = { POINT_RIGHT_X - 40.0f, POINT_UNDER_Y - 10.0f, POINT_TOP_Z };	// �E��
	//pointL_.mesh_.vertex_[2].pos = { POINT_LEFT_X - 40.0f, POINT_TOP_Y - 10.0f, POINT_UNDER_Z };	// ����
	//pointL_.mesh_.vertex_[3].pos = { POINT_RIGHT_X - 40.0f, POINT_TOP_Y - 10.0f, POINT_UNDER_Z };	// �E��

	//pointR_.mesh_.vertex_[0].pos = { -POINT_RIGHT_X + 40.0f, POINT_UNDER_Y - 10.0f, POINT_TOP_Z };// ����
	//pointR_.mesh_.vertex_[1].pos = { -POINT_LEFT_X + 40.0f, POINT_UNDER_Y - 10.0f, POINT_TOP_Z };	// �E��
	//pointR_.mesh_.vertex_[2].pos = { -POINT_RIGHT_X + 40.0f, POINT_TOP_Y - 10.0f, POINT_UNDER_Z };// ����
	//pointR_.mesh_.vertex_[3].pos = { -POINT_LEFT_X + 40.0f, POINT_TOP_Y - 10.0f, POINT_UNDER_Z };	// �E��

	//600,800�p
	mesh_.vertex_[0].pos = { ROLE_MESH_LEFT_X, ROLE_MESH_UNDER_Z, VERTEX_Z + 12.0f };	//����
	mesh_.vertex_[1].pos = { ROLE_MESH_RIGHT_X, ROLE_MESH_UNDER_Z, VERTEX_Z + 12.0f };	//�E��
	mesh_.vertex_[2].pos = { ROLE_MESH_LEFT_X, ROLE_MESH_TOP_Z, VERTEX_Z };				//����
	mesh_.vertex_[3].pos = { ROLE_MESH_RIGHT_X, ROLE_MESH_TOP_Z, VERTEX_Z };			//�E��

	pointL_.mesh_.vertex_[0].pos = { POINT_LEFT_X - 25.0f, POINT_UNDER_Y - 10.0f, POINT_TOP_Z };	// ����
	pointL_.mesh_.vertex_[1].pos = { POINT_RIGHT_X - 25.0f, POINT_UNDER_Y - 10.0f, POINT_TOP_Z };	// �E��
	pointL_.mesh_.vertex_[2].pos = { POINT_LEFT_X - 25.0f, POINT_TOP_Y - 10.0f, POINT_UNDER_Z };	// ����
	pointL_.mesh_.vertex_[3].pos = { POINT_RIGHT_X - 25.0f, POINT_TOP_Y - 10.0f, POINT_UNDER_Z };	// �E��

	pointR_.mesh_.vertex_[0].pos = { -POINT_RIGHT_X + 25.0f, POINT_UNDER_Y - 10.0f, POINT_TOP_Z };	// ����
	pointR_.mesh_.vertex_[1].pos = { -POINT_LEFT_X + 25.0f, POINT_UNDER_Y - 10.0f, POINT_TOP_Z };	// �E��
	pointR_.mesh_.vertex_[2].pos = { -POINT_RIGHT_X + 25.0f, POINT_TOP_Y - 10.0f, POINT_UNDER_Z };	// ����
	pointR_.mesh_.vertex_[3].pos = { -POINT_LEFT_X + 25.0f, POINT_TOP_Y - 10.0f, POINT_UNDER_Z };	// �E��

	readyMesh_.vertex_[0].pos = { -70.0f, 70.0f, -408.0f };	// ����
	readyMesh_.vertex_[1].pos = { 90.0f, 70.0f, -408.0f };	// �E��
	readyMesh_.vertex_[2].pos = { -70.0f, 160.0f, -410.0f };// ����
	readyMesh_.vertex_[3].pos = { 90.0f, 160.0f, -410.0f };	// �E��
}

void SelectImage::ReductinVertexPos(void)
{
	readyMesh_.vertex_[0].pos = { -60.0f, 80.0f, -408.0f };	// ����
	readyMesh_.vertex_[1].pos = { 60.0f, 80.0f, -408.0f };	// �E��
	readyMesh_.vertex_[2].pos = { -60.0f, 150.0f, -410.0f };// ����
	readyMesh_.vertex_[3].pos = { 60.0f, 150.0f, -410.0f };	// �E��
}

void SelectImage::Load(void)
{
	//�摜�̓ǂݍ���
	imgPlayerNum_ = ResourceManager::GetInstance().Load(ResourceManager::SRC::PLAYER_NUM).handleIds_;

	imgDisplayNum_ = ResourceManager::GetInstance().Load(ResourceManager::SRC::DISPLAY_NUM).handleIds_;

	imgRightPoint_ = ResourceManager::GetInstance().Load(ResourceManager::SRC::RIGHT_POINT).handleIds_;

	imgLeftPoint_ = ResourceManager::GetInstance().Load(ResourceManager::SRC::LEFT_POINT).handleIds_;

	imgReady_ = ResourceManager::GetInstance().Load(ResourceManager::SRC::READY).handleIds_;

	imgRoleNum_ = ResourceManager::GetInstance().Load(ResourceManager::SRC::CHARA_PARAMS).handleIds_;

	imgDeviceNum_ = ResourceManager::GetInstance().Load(ResourceManager::SRC::DEVICE).handleIds_;
	
	imgComingSoon_ = ResourceManager::GetInstance().Load(ResourceManager::SRC::COMING_SOON).handleIds_;

	auto& snd = SoundManager::GetInstance();
	auto& res = ResourceManager::GetInstance();

	//SE�ǂݍ���

	//��������
	snd.Add(SoundManager::TYPE::SE, SoundManager::SOUND::SCENE_CHANGE_1,
		res.Load(ResourceManager::SRC::SCENE_CHANGE_SE1).handleId_);
	int ret = res.Load(ResourceManager::SRC::SCENE_CHANGE_SE1).handleId_;
	if (ret == -1)
	{
		return;
	}
	//���ʒ���
	snd.AdjustVolume(SoundManager::SOUND::SCENE_CHANGE_1, 256);

	//�L�����Z��
	snd.Add(SoundManager::TYPE::SE, SoundManager::SOUND::CANCEL_SELECT,
		res.Load(ResourceManager::SRC::CANCEL_SELECT).handleId_);
	ret = res.Load(ResourceManager::SRC::CANCEL_SELECT).handleId_;
	if (ret == -1)
	{
		return;
	}
	//���ʒ���
	snd.AdjustVolume(SoundManager::SOUND::CANCEL_SELECT, 256);

	//�I��ύX
	snd.Add(SoundManager::TYPE::SE, SoundManager::SOUND::CHANGE_SELECT,
		res.Load(ResourceManager::SRC::CHANGE_SELECT).handleId_);
	 ret = res.Load(ResourceManager::SRC::CHANGE_SELECT).handleId_;
	if (ret == -1)
	{
		return;
	}
	//���ʒ���
	snd.AdjustVolume(SoundManager::SOUND::CHANGE_SELECT, 70);

	//�I������
	snd.Add(SoundManager::TYPE::SE, SoundManager::SOUND::DECIDE_SELECT,
		res.Load(ResourceManager::SRC::DECIDE_SELECT).handleId_);
	ret = res.Load(ResourceManager::SRC::DECIDE_SELECT).handleId_;
	if (ret == -1)
	{
		return;
	}
	//���ʒ���
	snd.AdjustVolume(SoundManager::SOUND::DECIDE_SELECT, 80);
}

void SelectImage::DisplayUpdate(void)
{
	DataBank& data = DataBank::GetInstance();
	float delta = 2.0f * SceneManager::GetInstance().GetDeltaTime();
	auto& snd = SoundManager::GetInstance();
	bool sound = false;

	//�E�̖��ON�̎��ɃL�[�̉E�ɒl������͂����������
	if (pointR_.isToggle_ &&
		selectScene_.GetConfig() == SelectScene::KEY_CONFIG::RIGHT)
	{
		if (!press_)
		{
			snd.Play(SoundManager::SOUND::CHANGE_SELECT);
			press_ = true;

			//�l�����P�ǉ�(���g��1�`4�Ɏ��߂�)
			displayNum_ = (displayNum_ % SceneManager::PLAYER_NUM) + 1;
		}

		//�L�[��������Ă���Ԍo�ߎ��Ԃ����Z���Ă���
		keyPressTime_ += delta;

		//�o�ߎ��Ԃ������莞�Ԍo�����ꍇ
		if (keyPressTime_ > SELECT_TIME)
		{
			//�C���^�[�o�������Z���Ă���
			interval_ += delta;

			//�C���^�[�o��1�b���Ƃɐ����P�����₵�Ă���
			if (interval_ > INTERVAL_TIME)
			{
				interval_ = 0.0f;
				displayNum_ = (displayNum_ % SceneManager::PLAYER_NUM) + 1;
				(!sound) ? snd.Play(SoundManager::SOUND::CHANGE_SELECT), sound != sound : sound;
			}
		}
	}
	else if (pointR_.isToggle_)
	{
		keyPressTime_ = 0.0f;
		interval_ = INTERVAL_TIME;
		press_ = false;
	}

	//��
	if (pointL_.isToggle_ &&
		selectScene_.GetConfig() == SelectScene::KEY_CONFIG::LEFT)
	{
		if (!press_)
		{
			snd.Play(SoundManager::SOUND::CHANGE_SELECT);
			press_ = true;

			//�l�����P�폜(���g��1�`4�Ɏ��߂�)
			displayNum_ = (displayNum_ + 3) % SceneManager::PLAYER_NUM;
			if (displayNum_ == 0)displayNum_ = SceneManager::PLAYER_NUM;
		}

		//�L�[��������Ă���Ԍo�ߎ��Ԃ����Z���Ă���
		keyPressTime_ += delta;

		//�o�ߎ��Ԃ������莞�Ԍo�����ꍇ
		if (keyPressTime_ > SELECT_TIME)
		{
			//�C���^�[�o�������Z���Ă���
			interval_ += delta;

			//�C���^�[�o��1�b���Ƃɐ����P�����炵�Ă���
			if (interval_ > INTERVAL_TIME)
			{
				interval_ = 0.0f;

				displayNum_ = (displayNum_ + 3) % SceneManager::PLAYER_NUM;
				if (displayNum_ == 0)displayNum_ = SceneManager::PLAYER_NUM;
				(!sound) ? snd.Play(SoundManager::SOUND::CHANGE_SELECT), sound != sound : sound;

			}
		}
	}
	else if (pointL_.isToggle_)
	{
		keyPressTime_ = 0.0f;
		interval_ = INTERVAL_TIME;
		press_ = false;
	}

	//�X�y�[�X�L�[�����Ō���&���̓f�o�C�X�I����
	if (selectScene_.GetConfig() == SelectScene::KEY_CONFIG::DECIDE)
	{
		snd.Play(SoundManager::SOUND::DECIDE_SELECT);

		//�v���C���[�l���̐ݒ�
		data.Input(SceneManager::PLAY_MODE::USER, displayNum_);

		//�f�B�X�v���C�̐ݒ�
		data.Input(DataBank::INFO::DHISPLAY_NUM, displayNum_);

		selectScene_.ChangeSelect(SelectScene::SELECT::NUMBER);
		ChangeSelect(SelectScene::SELECT::NUMBER);
	}

	//�I��������
	if (!pointR_.isToggle_ &&
		selectScene_.GetConfig() == SelectScene::KEY_CONFIG::RIGHT)
	{
		pointR_.isToggle_ = true;
		pointL_.isToggle_ = false;
	}

	if (!pointL_.isToggle_ &&
		selectScene_.GetConfig() == SelectScene::KEY_CONFIG::LEFT)
	{
		pointR_.isToggle_ = false;
		pointL_.isToggle_ = true;
	}

	//UV���W�i�e�N�X�`�����W�j
	mesh_.vertex_[0].u = ((float)(displayNum_)-1.0f) / 4.0f;	mesh_.vertex_[0].v = 1.0f;	// ����
	mesh_.vertex_[1].u = (float)(displayNum_) / 4.0f;			mesh_.vertex_[1].v = 1.0f;	// �E��
	mesh_.vertex_[2].u = ((float)(displayNum_)-1.0f) / 4.0f;	mesh_.vertex_[2].v = 0.0f;	// ����
	mesh_.vertex_[3].u = (float)(displayNum_) / 4.0f;			mesh_.vertex_[3].v = 0.0f;	// �E��

	pointL_.mesh_.vertex_[0].u = 0.0f;	pointL_.mesh_.vertex_[0].v = 1.0f;	// ����
	pointL_.mesh_.vertex_[1].u = 1.0f;	pointL_.mesh_.vertex_[1].v = 1.0f;	// �E��
	pointL_.mesh_.vertex_[2].u = 0.0f;	pointL_.mesh_.vertex_[2].v = 0.0f;	// ����
	pointL_.mesh_.vertex_[3].u = 1.0f;	pointL_.mesh_.vertex_[3].v = 0.0f;	// �E��
}

void SelectImage::NumberUpdate(void)
{
	DataBank& data = DataBank::GetInstance();
	float delta = 2.0f * SceneManager::GetInstance().GetDeltaTime();
	auto& snd = SoundManager::GetInstance();
	bool sound = false;

	//�E�̖��ON�̎��ɃL�[�̉E�ɒl������͂����������
	if (pointR_.isToggle_ &&
		selectScene_.GetConfig() == SelectScene::KEY_CONFIG::RIGHT)
	{
		if (!press_)
		{
			snd.Play(SoundManager::SOUND::CHANGE_SELECT);

			press_ = true;

			//�l�����P�ǉ�(���g��1�`4�Ɏ��߂�)
			playerNum_ = (playerNum_ % SceneManager::PLAYER_NUM) + 1;
		}

		//�L�[��������Ă���Ԍo�ߎ��Ԃ����Z���Ă���
		keyPressTime_ += delta;

		//�o�ߎ��Ԃ������莞�Ԍo�����ꍇ
		if (keyPressTime_ > SELECT_TIME)
		{
			//�C���^�[�o�������Z���Ă���
			interval_ += delta;

			//�C���^�[�o��1�b���ƂɃv���C�l�����P�����₵�Ă���
			if (interval_ > INTERVAL_TIME)
			{
				interval_ = 0.0f;
				playerNum_ = (playerNum_ % SceneManager::PLAYER_NUM) + 1 ;
				(!sound) ? snd.Play(SoundManager::SOUND::CHANGE_SELECT), sound != sound : sound;
			}
		}
	}
	else if (pointR_.isToggle_)
	{
		keyPressTime_ = 0.0f;
		interval_ = INTERVAL_TIME;
		press_ = false;
	}

	//��
	if (pointL_.isToggle_ &&
		selectScene_.GetConfig() == SelectScene::KEY_CONFIG::LEFT)
	{
		if (!press_)
		{
			snd.Play(SoundManager::SOUND::CHANGE_SELECT);
			press_ = true;

			//�l�����P�폜(���g��1�`4�Ɏ��߂�)
			playerNum_ = (playerNum_ + 3) % SceneManager::PLAYER_NUM;
			if (playerNum_ == 0)playerNum_ = SceneManager::PLAYER_NUM;
		}

		//�L�[��������Ă���Ԍo�ߎ��Ԃ����Z���Ă���
		keyPressTime_ += delta;

		//�o�ߎ��Ԃ������莞�Ԍo�����ꍇ
		if (keyPressTime_ > SELECT_TIME)
		{
			//�C���^�[�o�������Z���Ă���
			interval_ += delta;

			//�C���^�[�o��1�b���ƂɃv���C�l�����P�����炵�Ă���
			if (interval_ > INTERVAL_TIME)
			{
				interval_ = 0.0f;
				playerNum_ = (playerNum_ + 3) % SceneManager::PLAYER_NUM;
				if (playerNum_ == 0)playerNum_ = SceneManager::PLAYER_NUM;
				(!sound) ? snd.Play(SoundManager::SOUND::CHANGE_SELECT), sound != sound : sound;
			}
		}
	}
	else if (pointL_.isToggle_)
	{
		keyPressTime_ = 0.0f;
		interval_ = INTERVAL_TIME;
		press_ = false;
	}

	//�X�y�[�X�L�[�����Ō���&���̓f�o�C�X�I����
	if (selectScene_.GetConfig() == SelectScene::KEY_CONFIG::DECIDE)
	{
		snd.Play(SoundManager::SOUND::DECIDE_SELECT);

		//�v���C���[�l���̐ݒ�
		data.Input(SceneManager::PLAY_MODE::USER, playerNum_);
		
		for (int i = 1; i <= playerNum_; i++)
		{
			//�f�B�X�v���C�̐ݒ�
			//data.Input(DataBank::INFO::DHISPLAY_NUM, i);
			data.Input(DataBank::INFO::USER_NUM, i);
		}

		for (int i = playerNum_ + 1; i <= SceneManager::PLAYER_NUM; i++)
		{
			//CPU�l���̐ݒ�(CPU�͂P�l����R�l)
 			data.Input(SceneManager::PLAY_MODE::CPU, i);
			data.Input(SceneManager::ROLE::KNIGHT, i);
		}

		//�E�B���h�E�����̏���
		SceneManager::GetInstance().RedySubWindow();

		//�J�����̐ݒ�
		auto cameras = SceneManager::GetInstance().GetCameras();
		for (int i = 1; i < cameras.size(); i++)
		{
			//�ŏ��̍��W��ێ�
			VECTOR prevPos = target_[i - 1];
			//XZ���ʂō��W����]������
			target_[i] = AsoUtility::RotXZPos(SelectScene::DEFAULT_CAMERA_POS, prevPos, AsoUtility::Deg2RadF(90.0f));

			cameras[i]->SetPos(SelectScene::DEFAULT_CAMERA_POS, target_[i]);
			cameras[i]->ChangeMode(Camera::MODE::FIXED_POINT);
		}

		if (playerNum_ > 1)
		{
			//�v���C���[2�ȏ�̏ꍇ�A2P�ȏ�̃R���g���[���[��PAD����ɐݒ�
			for (int num = 2; num <= playerNum_; num++)
			{
				data.Input(SceneManager::CNTL::PAD, num);
			}
		}

		selectScene_.ChangeSelect(SelectScene::SELECT::OPERATION);
		ChangeSelect(SelectScene::SELECT::OPERATION);
	}

	//�I��������
	if (!pointR_.isToggle_ &&
		selectScene_.GetConfig() == SelectScene::KEY_CONFIG::RIGHT)
	{
		pointR_.isToggle_ = true;
		pointL_.isToggle_ = false;
	}

	if (!pointL_.isToggle_ &&
		selectScene_.GetConfig() == SelectScene::KEY_CONFIG::LEFT)
	{
		pointR_.isToggle_ = false;
		pointL_.isToggle_ = true;
	}

	//UV���W�i�e�N�X�`�����W�j
	mesh_.vertex_[0].u = ((float)(playerNum_)-1.0f) / 4.0f;		mesh_.vertex_[0].v = 1.0f;	// ����
	mesh_.vertex_[1].u = (float)(playerNum_) / 4.0f;			mesh_.vertex_[1].v = 1.0f;	// �E��
	mesh_.vertex_[2].u = ((float)(playerNum_)-1.0f) / 4.0f;		mesh_.vertex_[2].v = 0.0f;	// ����
	mesh_.vertex_[3].u = (float)(playerNum_) / 4.0f;			mesh_.vertex_[3].v = 0.0f;	// �E��

	pointL_.mesh_.vertex_[0].u = 0.0f;	pointL_.mesh_.vertex_[0].v = 1.0f;	// ����
	pointL_.mesh_.vertex_[1].u = 1.0f;	pointL_.mesh_.vertex_[1].v = 1.0f;	// �E��
	pointL_.mesh_.vertex_[2].u = 0.0f;	pointL_.mesh_.vertex_[2].v = 0.0f;	// ����
	pointL_.mesh_.vertex_[3].u = 1.0f;	pointL_.mesh_.vertex_[3].v = 0.0f;	// �E��
}

void SelectImage::OperationUpdate(void)
{
	DataBank& data = DataBank::GetInstance();
	float delta = 2.0f * SceneManager::GetInstance().GetDeltaTime();
	auto& snd = SoundManager::GetInstance();
	bool sound = false;

	//�E�̖��ON�̎��ɃL�[�̉E�ɒl������͂����������
	if (pointR_.isToggle_ &&
		selectScene_.GetConfig() == SelectScene::KEY_CONFIG::RIGHT)
	{
		if (!press_)
		{
			snd.Play(SoundManager::SOUND::CHANGE_SELECT);
			press_ = true;

			//�L�[���������ۂɑI�����Ă�����̂𔽓](2��ނ����Ȃ��̂�)
			isPad_ = !isPad_;
		}
		//�L�[��������Ă���Ԍo�ߎ��Ԃ����Z���Ă���
		keyPressTime_ += delta;

		//�o�ߎ��Ԃ������莞�Ԍo�����ꍇ
		if (keyPressTime_ > SELECT_TIME)
		{
			//�C���^�[�o�������Z���Ă���
			interval_ += delta;

			//�C���^�[�o��1�b���Ƃɐ����P�����₵�Ă���
			if (interval_ > INTERVAL_TIME)
			{
				interval_ = 0.0f;
				isPad_ = !isPad_;
				(!sound) ? snd.Play(SoundManager::SOUND::CHANGE_SELECT), sound != sound : sound;
			}
		}
	}
	else if (pointR_.isToggle_)
	{
		keyPressTime_ = 0.0f;
		interval_ = INTERVAL_TIME;
		press_ = false;
	}

	//��
	if (pointL_.isToggle_ &&
		selectScene_.GetConfig() == SelectScene::KEY_CONFIG::LEFT)
	{
		if (!press_)
		{
			snd.Play(SoundManager::SOUND::CHANGE_SELECT);
			press_ = true;

			//�L�[���������ۂɑI�����Ă�����̂𔽓](2��ނ����Ȃ��̂�)
			isPad_ = !isPad_;
		}
		//�L�[��������Ă���Ԍo�ߎ��Ԃ����Z���Ă���
		keyPressTime_ += delta;

		//�o�ߎ��Ԃ������莞�Ԍo�����ꍇ
		if (keyPressTime_ > SELECT_TIME)
		{
			//�C���^�[�o�������Z���Ă���
			interval_ += delta;

			//�C���^�[�o��1�b���ƂɃv���C�l�����P�����炵�Ă���
			if (interval_ > INTERVAL_TIME)
			{
				interval_ = 0.0f;
				isPad_ = !isPad_;
				(!sound) ? snd.Play(SoundManager::SOUND::CHANGE_SELECT), sound != sound : sound;
			}
		}
	}
	else if (pointL_.isToggle_)
	{
		keyPressTime_ = 0.0f;
		interval_ = INTERVAL_TIME;
		press_ = false;
	}

	//�X�y�[�X�L�[�����Ō���&��E�I����
	if (selectScene_.GetConfig() == SelectScene::KEY_CONFIG::DECIDE)
	{
		snd.Play(SoundManager::SOUND::DECIDE_SELECT);

		//1P�̑���̐ݒ�
		(isPad_) ? data.Input(SceneManager::CNTL::PAD, 1) : data.Input(SceneManager::CNTL::KEYBOARD, 1);
		(isPad_) ? selectScene_.Set1PDevice(SceneManager::CNTL::PAD) : selectScene_.Set1PDevice(SceneManager::CNTL::KEYBOARD);
		//selectedCntl_ = (isPad_) ? SceneManager::CNTL::PAD : SceneManager::CNTL::KEYBOARD;

		selectScene_.ChangeSelect(SelectScene::SELECT::ROLE);
		ChangeSelect(SelectScene::SELECT::ROLE);
	}

	//�I��������
	if (!pointR_.isToggle_ &&
		selectScene_.GetConfig() == SelectScene::KEY_CONFIG::RIGHT)
	{
		pointR_.isToggle_ = true;
		pointL_.isToggle_ = false;
	}

	if (!pointL_.isToggle_ &&
		selectScene_.GetConfig() == SelectScene::KEY_CONFIG::LEFT)
	{
		pointR_.isToggle_ = false;
		pointL_.isToggle_ = true;
	}

	//UV���W�i�e�N�X�`�����W�j
	mesh_.vertex_[0].u = (float)(isPad_) / 4.0f;			mesh_.vertex_[0].v = 1.0f;	// ����
	mesh_.vertex_[1].u = ((float)(isPad_)+1.0f) / 4.0f;		mesh_.vertex_[1].v = 1.0f;	// �E��
	mesh_.vertex_[2].u = (float)(isPad_) / 4.0f;			mesh_.vertex_[2].v = 0.0f;	// ����
	mesh_.vertex_[3].u = ((float)(isPad_)+1.0f) / 4.0f;		mesh_.vertex_[3].v = 0.0f;	// �E��

}

void SelectImage::RoleUpdate(void)
{
}

void SelectImage::DisplayDraw(void)
{
	mesh_.DrawTwoMesh(*imgDisplayNum_);

	PointsDraw();
}

void SelectImage::NumberDraw(void)
{
	mesh_.DrawTwoMesh(*imgPlayerNum_);

	PointsDraw();
}

void SelectImage::OperationDraw(void)
{
	mesh_.DrawTwoMesh(*imgDeviceNum_);

	PointsDraw();
}

void SelectImage::RoleDraw(void)
{
	if (GetReady() != true)
	{
		mesh_.DrawTwoMesh(*imgRoleNum_);
	}
	else
	{
		mesh_.DrawTwoMesh(*imgRoleNum_);
		SetDrawBlendMode(DX_BLENDMODE_SUB, BLEND_PARAM);
		mesh_.DrawTwoMesh(*imgRoleNum_);
		SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);	//�u�����h���[�h��߂�

		readyMesh_.DrawTwoMesh(*imgReady_);
	}

	PointsDraw();

	//���\�L
	if (GetRole() == static_cast<int>(SceneManager::ROLE::MAGE))
	{
		
		readyMesh_.DrawTwoMesh(*imgComingSoon_);
	}
}

void SelectImage::PointsDraw(void)
{
	//���̖��̕`��(�E�̖��͈Â߂ɂ���)
	if (pointL_.isToggle_)
	{
		////�E�̖��摜��`�悵�A�����摜�����Z�u�����h����
		pointR_.mesh_.DrawTwoMesh(*imgRightPoint_);
		SetDrawBlendMode(DX_BLENDMODE_SUB, BLEND_PARAM);
		pointR_.mesh_.DrawTwoMesh(*imgRightPoint_);
		SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);	//�u�����h���[�h��߂�

		//���ʂɕ`��
		pointL_.mesh_.DrawTwoMesh(*imgLeftPoint_);
	}
	//�E�̖��̕`��(���̖��͈Â߂ɂ���)
	if (pointR_.isToggle_)
	{
		//���̖��摜��`�悵�A�����摜�����Z�u�����h����
		pointL_.mesh_.DrawTwoMesh(*imgLeftPoint_);
		SetDrawBlendMode(DX_BLENDMODE_SUB, BLEND_PARAM);
		pointL_.mesh_.DrawTwoMesh(*imgLeftPoint_);
		SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);	//�u�����h���[�h��߂�

		//���ʂɕ`��
		pointR_.mesh_.DrawTwoMesh(*imgRightPoint_);
	}

	//�ǂ���̖����I��ł��Ȃ��Ƃ��̕`��
	if (!pointL_.isToggle_ && !pointR_.isToggle_)
	{
		//���ʂɕ`��
		pointL_.mesh_.DrawTwoMesh(*imgLeftPoint_);
		pointR_.mesh_.DrawTwoMesh(*imgRightPoint_);
		//���̏�̉摜�Ɍ��Z�u�����h����
		SetDrawBlendMode(DX_BLENDMODE_SUB, BLEND_PARAM);
		pointL_.mesh_.DrawTwoMesh(*imgLeftPoint_);
		pointR_.mesh_.DrawTwoMesh(*imgRightPoint_);
		SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);
	}
}

void SelectImage::ChangeObject(SelectScene::Device& input, int i)
{
	DataBank& data = DataBank::GetInstance();
	float delta = 2.0f * SceneManager::GetInstance().GetDeltaTime();
	auto& snd = SoundManager::GetInstance();
	bool sound = false;

	if (isReady_ == true)
	{
		if (input.config_ == SelectScene::KEY_CONFIG::CANCEL)
		{
			//�L�����Z����
 			snd.Play(SoundManager::SOUND::CANCEL_SELECT);

			isReady_ = false;
		}
		//���������̎��ɃL�����I���𑀍�ł��Ȃ��悤�ɂ���
		return;
	}

	//�E�̎O�p�`��ON�̎��ɃL�[�̉E�ɒl������͂����������
	if (pointR_.isToggle_ &&
		input.config_ == SelectScene::KEY_CONFIG::RIGHT)
	{
		if (!press_)
		{
			//�I��ύX��
			snd.Play(SoundManager::SOUND::CHANGE_SELECT);

			press_ = true;

			//��E��I��
			role_ = (role_ % SceneManager::PLAYER_NUM) + 1;
		}

		//�L�[��������Ă���Ԍo�ߎ��Ԃ����Z���Ă���
		keyPressTime_ += delta;

		//�o�ߎ��Ԃ������莞�Ԍo�����ꍇ
		if (keyPressTime_ > SELECT_TIME)
		{
			//�C���^�[�o�������Z���Ă���
			interval_ += delta;

			//�C���^�[�o��1�b���ƂɃv���C�l�����P�����₵�Ă���
			if (interval_ > INTERVAL_TIME)
			{
				interval_ = 0.0f;
				role_ = (role_ % SceneManager::PLAYER_NUM) + 1;
				(!sound) ? snd.Play(SoundManager::SOUND::CHANGE_SELECT), sound != sound: sound;
			} 
		}
	}
	else if (pointR_.isToggle_)
	{
		keyPressTime_ = 0.0f;
		interval_ = INTERVAL_TIME;
		press_ = false;
	}

	//��
	if (pointL_.isToggle_ &&
		input.config_ == SelectScene::KEY_CONFIG::LEFT)
	{
		if (!press_)
		{
			//�I��ύX��
			snd.Play(SoundManager::SOUND::CHANGE_SELECT);

			press_ = true;

			//��E��I��
			role_ = (role_ + 3) % SceneManager::PLAYER_NUM;
			if (role_ == 0)role_ = SceneManager::PLAYER_NUM;
		}
		//�L�[��������Ă���Ԍo�ߎ��Ԃ����Z���Ă���
		keyPressTime_ += delta;

		//�o�ߎ��Ԃ������莞�Ԍo�����ꍇ
		if (keyPressTime_ > SELECT_TIME)
		{
			//�C���^�[�o�������Z���Ă���
			interval_ += delta;

			//�C���^�[�o��1�b���ƂɃv���C�l�����P�����炵�Ă���
			if (interval_ > INTERVAL_TIME)
			{
				interval_ = 0.0f;
				role_ = (role_ + 3) % SceneManager::PLAYER_NUM;
				if (role_ == 0)role_ = SceneManager::PLAYER_NUM;
				(!sound) ? snd.Play(SoundManager::SOUND::CHANGE_SELECT), sound != sound : sound;
			}
		}
	}
	else if (pointL_.isToggle_)
	{
		keyPressTime_ = 0.0f;
		interval_ = INTERVAL_TIME;
		press_ = false;
	}

	//�X�y�[�X�L�[������E����
	if (role_ != 3	&&
		input.config_ == SelectScene::KEY_CONFIG::DECIDE)
	{
		//���艹
		snd.Play(SoundManager::SOUND::SCENE_CHANGE_1);

		//��E�̐ݒ�
 		data.Input(static_cast<SceneManager::ROLE>(role_ ), i + 1);

		isReady_ = true;
	}

	//�I��������
	if (!pointR_.isToggle_ &&
		input.config_ == SelectScene::KEY_CONFIG::RIGHT)
	{
		pointR_.isToggle_ = true;
		pointL_.isToggle_ = false;
	}

	if (!pointL_.isToggle_ &&
		input.config_ == SelectScene::KEY_CONFIG::LEFT)
	{
		pointR_.isToggle_ = false;
		pointL_.isToggle_ = true;
	}

	//UV���W�i�e�N�X�`�����W�j
	mesh_.vertex_[0].u = 0.0f;	mesh_.vertex_[0].v = ((float)(role_)) / 4.0f;	// ����
	mesh_.vertex_[1].u = 1.0f;	mesh_.vertex_[1].v = ((float)(role_)) / 4.0f;	// �E��
	mesh_.vertex_[2].u = 0.0f;	mesh_.vertex_[2].v = ((float)(role_)-1.0f) / 4.0f;	// ����
	mesh_.vertex_[3].u = 1.0f;	mesh_.vertex_[3].v = ((float)(role_)-1.0f) / 4.0f;	// �E��
}

void SelectImage::ChangeSelect(const SelectScene::SELECT _state)
{
	//��ԑJ��
	state_ = _state;

	// �e��ԑJ�ڂ̏�������
	stateChanges_[state_]();
}

void SelectImage::InitVertex(void)
{
	leftTop_ = { VERTEX_LEFT_X, VERTEX_TOP_Y, VERTEX_Z };
	leftBottom_ = { VERTEX_LEFT_X, VERTEX_UNDER_Y, VERTEX_UNDER_Z };
	rightTop_ = { VERTEX_RIGHT_X, VERTEX_TOP_Y, VERTEX_Z };
	rightBottom_ = { VERTEX_RIGHT_X, VERTEX_UNDER_Y, VERTEX_UNDER_Z };

	//���W(�l�p�`����邽�߂�2�̎O�p�`���g��)
	mesh_.vertex_[0].pos = leftBottom_;		// ����
	mesh_.vertex_[1].pos = rightBottom_;	// �E��
	mesh_.vertex_[2].pos = leftTop_;		// ����
	mesh_.vertex_[3].pos = rightTop_;		// �E��

	readyMesh_.vertex_[0].pos = { -80.0f, 70.0f, -408.0f };	// ����
	readyMesh_.vertex_[1].pos = { 80.0f, 70.0f, -408.0f };	// �E��
	readyMesh_.vertex_[2].pos = { -80.0f, 160.0f, -410.0f };// ����
	readyMesh_.vertex_[3].pos = { 80.0f, 160.0f, -410.0f };	// �E��

	pointL_.mesh_.vertex_[0].pos = { POINT_LEFT_X, POINT_UNDER_Y, POINT_TOP_Z };	// ����
	pointL_.mesh_.vertex_[1].pos = { POINT_RIGHT_X, POINT_UNDER_Y, POINT_TOP_Z };	// �E��
	pointL_.mesh_.vertex_[2].pos = { POINT_LEFT_X, POINT_TOP_Y, POINT_UNDER_Z };	// ����
	pointL_.mesh_.vertex_[3].pos = { POINT_RIGHT_X, POINT_TOP_Y, POINT_UNDER_Z };	// �E��

	pointR_.mesh_.vertex_[0].pos = { -POINT_RIGHT_X, POINT_UNDER_Y, POINT_TOP_Z };	// ����
	pointR_.mesh_.vertex_[1].pos = { -POINT_LEFT_X, POINT_UNDER_Y, POINT_TOP_Z };	// �E��
	pointR_.mesh_.vertex_[2].pos = { -POINT_RIGHT_X, POINT_TOP_Y, POINT_UNDER_Z };	// ����
	pointR_.mesh_.vertex_[3].pos = { -POINT_LEFT_X, POINT_TOP_Y, POINT_UNDER_Z };	// �E��
}

VECTOR SelectImage::RotateVertex(VECTOR pos, VECTOR center, float angle)
{
	//���b�V���̒��S�_���v�Z(���͂Ƃ肠�������b�V���̐^�񒆂̍��W)
	VECTOR Center;
	Center.x = (VERTEX_RIGHT_X + VERTEX_LEFT_X) * 0.5f;
	Center.y = (VERTEX_TOP_Y + VERTEX_UNDER_Y) * 0.5f;
	Center.z = VERTEX_Z;

	//���a
	const float DIS = 50.0f;

	// ���Ԑi�s
	angle_ += SceneManager::GetInstance().GetDeltaTime();

	// ��]�s���K�p���Ē��_����]

	//for (int i = 1; i < 4; i += 2)
	//{
		//XZ���ʏ�ŉ�]������
	//	vertices_[i].pos.x = Center.x + (DIS * (sinf(angle_)));
	//	vertices_[i].pos.z = Center.z + (DIS * (cosf(angle_)));
	//}
	//for (int i = 0; i < 4; i += 2)
	//{
	//	vertices_[i].pos.x = Center.x + (-DIS * (sinf(angle_)));
	//	vertices_[i].pos.z = Center.z + (-DIS * (cosf(angle_)));
	//}
	return AsoUtility::VECTOR_ZERO;
}

void SelectImage::ChangeStateDisplay(void)
{
	stateUpdate_ = std::bind(&SelectImage::DisplayUpdate, this);
}

void SelectImage::ChangeStateNumber(void)
{
	stateUpdate_ = std::bind(&SelectImage::NumberUpdate, this);
}

void SelectImage::ChangeStateOperation(void)
{
	stateUpdate_ = std::bind(&SelectImage::OperationUpdate, this);
}

void SelectImage::ChangeStateRole(void)
{
	MoveVertexPos();
	ReductinVertexPos();
	stateUpdate_ = std::bind(&SelectImage::RoleUpdate, this);
}

void SelectImage::Mesh::DrawTwoMesh(int handle)
{
	// 2�̎O�p�`��`��i�e�N�X�`���t���j
	// 1�ڂ̎O�p�`
	VERTEX3D triangle1[3] = { vertex_[2], vertex_[1], vertex_[0] };
	// 2�ڂ̎O�p�`
	VERTEX3D triangle2[3] = { vertex_[1], vertex_[2], vertex_[3] };

	// �O�p�`��`��
	DrawPolygon3D(triangle1, 1, handle, true);  // 1�ڂ̎O�p�`
	DrawPolygon3D(triangle2, 1, handle, true);  // 2�ڂ̎O�p�`
}
