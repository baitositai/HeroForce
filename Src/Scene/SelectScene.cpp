#include <vector>
#include <math.h>
#include<algorithm>
#include "../Application.h"
#include "../Manager/Decoration/SoundManager.h"
#include "../Manager/Generic/InputManager.h"
#include "../Manager/Generic/ResourceManager.h"
#include "../Manager/Generic/Camera.h"
#include "../Manager/GameSystem/DataBank.h"
#include "../Utility/AsoUtility.h"
#include "../Object/Stage/StageManager.h"
#include "../Object/Stage/SkyDome.h"
#include "../Object/Character/PlayableChara/Other/SelectPlayer.h"
#include "../Object/SelectImage.h"
#include "../Object/Character/PlayableChara/Other/SelectEnemy.h"
#include "SelectScene.h"

SelectScene::SelectScene(void)
{
	stage_ = nullptr;

	key_ = KEY_CONFIG::NONE;
	select_ = SELECT::NUMBER;

	// ��ԊǗ�
	stateChanges_.emplace(
		SELECT::DISPLAY, std::bind(&SelectScene::ChangeStateDisplay, this));
	stateChanges_.emplace(
		SELECT::NUMBER, std::bind(&SelectScene::ChangeStateNumber, this));
	stateChanges_.emplace(
		SELECT::OPERATION, std::bind(&SelectScene::ChangeStateOperation, this));
	stateChanges_.emplace(
		SELECT::ROLE, std::bind(&SelectScene::ChangeStateRole, this));
	stateChanges_.emplace(
		SELECT::MAX, std::bind(&SelectScene::ChangeStateMax, this));

	readyNum = 0;
	okNum = 0;
	isOk_[0] = false;
	isOk_[1] = false;
	isOk_[2] = false;
	isOk_[3] = false;

	input_[0].cntl_ = SceneManager::CNTL::NONE;
	input_[1].cntl_ = SceneManager::CNTL::NONE;
	input_[2].cntl_ = SceneManager::CNTL::NONE;
	input_[3].cntl_ = SceneManager::CNTL::NONE;

	input_[0].config_ = KEY_CONFIG::NONE;
	input_[1].config_ = KEY_CONFIG::NONE;
	input_[2].config_ = KEY_CONFIG::NONE;
	input_[3].config_ = KEY_CONFIG::NONE;
}

SelectScene::~SelectScene(void)
{
}

void SelectScene::Init(void)
{
	auto& snd = SoundManager::GetInstance();
	auto& res = ResourceManager::GetInstance();

	//���y�ǂݍ���
	snd.Add(SoundManager::TYPE::BGM, SoundManager::SOUND::SELECT,
		res.Load(ResourceManager::SRC::SELECT_BGM).handleId_);
	//���ʒ���
	snd.AdjustVolume(SoundManager::SOUND::SELECT, 128);
	//���y�Đ�
	snd.Play(SoundManager::SOUND::SELECT);

	//�摜�̓ǂݍ���
	imgPlayer_ = ResourceManager::GetInstance().Load(ResourceManager::SRC::PLAYER_IMG).handleId_;
 	imgDisplay_ = ResourceManager::GetInstance().Load(ResourceManager::SRC::DISPLAY_IMG).handleId_;
	imgOperation_ = ResourceManager::GetInstance().Load(ResourceManager::SRC::OPERATION_IMG).handleId_;
	imgRole_ = ResourceManager::GetInstance().Load(ResourceManager::SRC::ROLE_IMG).handleId_;
	imgWait_ = ResourceManager::GetInstance().Load(ResourceManager::SRC::WAIT_IMG).handleId_;

	//�X�J�C�h�[��
	skyDome_ = std::make_unique<SkyDome>();
	skyDome_->Init();
	
	//�w�i�p�X�e�[�W
	stage_ = new StageManager();
	stage_->Init();

	//�v���C���[�ݒ�
	for (int i = 0; i < SceneManager::PLAYER_NUM; i++)
	{
		players_[i] = std::make_shared<SelectPlayer>();
		players_[i]->Init();
		
		enemys_[i] = std::make_shared<SelectEnemy>();
		enemys_[i]->Init();

		images_[i] = std::make_unique<SelectImage>(*this, *players_);
		images_[i]->Init();
	}

	// �J�������[�h�F��_�J����
	auto camera = SceneManager::GetInstance().GetCameras();
	camera[0]->SetPos(DEFAULT_CAMERA_POS, DEFAULT_TARGET_POS);
	camera[0]->ChangeMode(Camera::MODE::FIXED_POINT);

	//�l���I������
	ChangeSelect(SELECT::DISPLAY);

	key_ = KEY_CONFIG::NONE;
	Change1PDevice(SceneManager::CNTL::NONE);

	Set1PDevice(SceneManager::CNTL::NONE);
}

void SelectScene::Update(void)
{
	//�����]
	skyDome_->Update();

	//�X�V�X�e�b�v
	stateUpdate_();
}

void SelectScene::Draw(void)
{
	auto& ins = InputManager::GetInstance();

	skyDome_->Draw();
	stage_->Draw();
	SetUseLightAngleAttenuation(FALSE);

	//�I�𒆂̎�ނ��Ƃ̍X�V����
	switch (select_)
	{
	case SELECT::DISPLAY:
		DisplayDraw();
		break;

	case SELECT::NUMBER:
		NumberDraw();
		break;

	case SELECT::OPERATION:
		OperationDraw();
		break;

	case SELECT::ROLE:
		RoleDraw();
		break;

	default:
		break;
	}
	//�f�o�b�O�`��
	//DrawDebug();

	SetUseLightAngleAttenuation(TRUE);
}

void SelectScene::Release(void)
{
	//SceneManager::GetInstance().ResetCameras();

	for (int i = 0; i < SceneManager::PLAYER_NUM; i++)
	{
		images_[i]->Destroy();
		players_[i]->Destroy();
		enemys_[i]->Destroy();
	}
}

void SelectScene::ChangeStateDisplay(void)
{
	stateUpdate_ = std::bind(&SelectScene::DisplayUpdate, this);
}

void SelectScene::ChangeStateNumber(void)
{
	stateUpdate_ = std::bind(&SelectScene::NumberUpdate, this);
}

void SelectScene::ChangeStateOperation(void)
{
	stateUpdate_ = std::bind(&SelectScene::OperationUpdate, this);
}

void SelectScene::ChangeStateRole(void)
{
	DataBank& data = DataBank::GetInstance();
	SceneManager::CNTL maincCntl = data.Output(1).cntrol_;
	Set1PDevice(maincCntl);

	stateUpdate_ = std::bind(&SelectScene::RoleUpdate, this);
}

void SelectScene::ChangeStateMax(void)
{
	stateUpdate_ = std::bind(&SelectScene::MaxUpdate, this);
}

void SelectScene::DisplayUpdate(void)
{
	KeyConfigSetting();
	ControllDevice();
	images_[0]->Update();
}

void SelectScene::NumberUpdate(void)
{
	KeyConfigSetting();
	ControllDevice();

	images_[0]->Update();
}

void SelectScene::OperationUpdate(void)
{
	auto camera = SceneManager::GetInstance().GetCameras();
	KeyConfigSetting();
	ControllDevice();

	images_[0]->Update();
	for (int i = 1; i < camera.size();i++)
	{
		for (int a = 0; a < SceneManager::PLAYER_NUM; a++) {
			VECTOR pos = AsoUtility::RotXZPos(DEFAULT_CAMERA_POS, enemys_[i - 1]->GetPosArray(a), AsoUtility::Deg2RadF(90.0f));
			enemys_[i]->SetPosArray(pos,a);
			enemys_[i]->SetRot(Quaternion::Euler(0.0f, AsoUtility::Deg2RadF(-90.0f * i), 0.0f));
		}
		enemys_[i]->Update();
	}
}

void SelectScene::RoleUpdate(void)
{
	auto camera = SceneManager::GetInstance().GetCameras();
	bool checkAllReady = false;
	if (IsAllReady()) checkAllReady = true;

	// ����������ĂȂ��Ƃ�
	for (auto& i : input_)i.config_ = KEY_CONFIG::NONE;

	//1P���L�[�{�[�h��������L�[�{�[�h����������悤��(���̏ꍇ1P��PAD�͑���ł��Ȃ��Ȃ�)
	if (Get1PDevice() == SceneManager::CNTL::KEYBOARD) KeyBoardProcess();
	PadProcess();

	for (auto& p : players_)
	{
		p->Update();
	}

	//�I�u�W�F�N�g��90�x����]������
	//(�J������90�x����]���Ă�̂ł���ɍ��킹�邽��)
	VERTEX3D ver[4];
	VERTEX3D ready[4];
	VERTEX3D pointL[4];
	VERTEX3D pointR[4];
	for (int m = 1; m < 4; m++)
	{
		for (int i = 0; i < 4; i++)
		{
			ver[i] = images_[m - 1]->GetMeshVertex(i);
			ready[i] = images_[m - 1]->GetReadyMeshVertex(i);
			pointL[i] = images_[m - 1]->GetPointLMeshVertex(i);
			pointR[i] = images_[m - 1]->GetPointRMeshVertex(i);

			VECTOR prevPos = ver[i].pos;
			VECTOR readyPos = ready[i].pos;
			VECTOR pointLPos = pointL[i].pos;
			VECTOR pointRPos = pointR[i].pos;

			//�l���I�u�W�F�N�g
			VECTOR pos = AsoUtility::RotXZPos(
				DEFAULT_CAMERA_POS, prevPos, AsoUtility::Deg2RadF(90.0f));
			images_[m]->RotMeshPos(pos, i);

			//Ready�I�u�W�F�N�g
			pos = AsoUtility::RotXZPos(
				DEFAULT_CAMERA_POS, readyPos, AsoUtility::Deg2RadF(90.0f));
			images_[m]->RotReadyMeshPos(pos, i);

			//�����I�u�W�F�N�g
			pos = AsoUtility::RotXZPos(
				DEFAULT_CAMERA_POS, pointLPos, AsoUtility::Deg2RadF(90.0f));
			images_[m]->RotPointLMeshPos(pos, i);

			//�E���I�u�W�F�N�g
			pos = AsoUtility::RotXZPos(
				DEFAULT_CAMERA_POS, pointRPos, AsoUtility::Deg2RadF(90.0f));
			images_[m]->RotPointRMeshPos(pos, i);
		}
	}

	for (int i = 0; i < camera.size(); i++)
	{
		//�v���C���[���Ƃ̑���ŃI�u�W�F�N�g��ύX����
		images_[i]->ChangeObject(input_[i], i);
		//�v���C���[�̏�����Ԃ������
		isOk_[i] = images_[i]->GetReady();
		//�v���C���[���Ƃ̑���ŕύX������E�ɉ����ďo���L�����N�^�[��ύX
		players_[i]->SetRole(images_[i]->GetRole());
	}

	//�L�����N�^�[�̈ʒu�ƌ�����ݒ�
	for (int i = 1; i < camera.size(); i++)
	{
		players_[i]->SetPos(AsoUtility::RotXZPos(DEFAULT_CAMERA_POS, players_[i - 1]->GetPos(), AsoUtility::Deg2RadF(90.0f)));
		players_[i]->SetRot(Quaternion::Euler(0.0f, AsoUtility::Deg2RadF(-90.0f * i), 0.0f));

		players_[i]->SetChickenPos(AsoUtility::RotXZPos(DEFAULT_CAMERA_POS, players_[i - 1]->GetChickenPos(), AsoUtility::Deg2RadF(90.0f)));
		players_[i]->SetRotChicken(Quaternion::Euler(0.0f, AsoUtility::Deg2RadF(-90.0f * i), 0.0f));
	}

	//�S������������Ԃ�1P������{�^�������ŃQ�[����ʂ�
	if (checkAllReady && input_[0].config_ == KEY_CONFIG::DECIDE)
	{
		SceneManager::GetInstance().ChangeScene(SceneManager::SCENE_ID::EXP);
	}

}

void SelectScene::MaxUpdate(void)
{
	//�������Ȃ�
}

void SelectScene::DisplayDraw(void)
{
	images_[0]->Draw();

	DrawRotaGraph(Application::SCREEN_SIZE_X/2, 60,1.0f, 0.0f, imgDisplay_, true);
}

void SelectScene::NumberDraw(void)
{
	images_[0]->Draw();
	DrawRotaGraph(Application::SCREEN_SIZE_X / 2, 60, 1.0f, 0.0f, imgPlayer_, true);
}

void SelectScene::OperationDraw(void)
{
	auto camera = SceneManager::GetInstance().GetCameras();

	images_[0]->Draw();

	for (int i = 1; i < camera.size(); i++)
	{
		enemys_[i]->Draw();
	}

	if (SceneManager::GetInstance().GetNowWindow() < 1)
	{ 
		DrawRotaGraph(Application::SCREEN_SIZE_X / 2, 60, 1.0f, 0.0f, imgOperation_, true); 
	}
	else{
		DrawRotaGraph(Application::SCREEN_SIZE_X / 2, 150, 1.0f, 0.0f, imgWait_, true); 
	}
}

void SelectScene::RoleDraw(void)
{
	auto camera = SceneManager::GetInstance().GetCameras();

	for (int i = 0; i < camera.size(); i++)
	{
		images_[i]->Draw();
	}

	SetUseLightAngleAttenuation(TRUE);
	for (int i = 0; i < camera.size(); i++)
	{
		players_[i]->Draw();
	}

	if (SceneManager::GetInstance().GetNowWindow() > -1)
	{
		DrawRotaGraph(Application::SCREEN_SIZE_X - 180, 100, 1.0f, 0.0f, imgRole_, true);
	}
}

void SelectScene::MaxDraw(void)
{
}

void SelectScene::DrawDebug(void)
{
	InputManager& ins = InputManager::GetInstance();
	Vector2 mPos = ins.GetMousePos();

	for (int i = 0; i < 4; i++)
	{
		DrawFormatString(Application::SCREEN_SIZE_X - 100, 0 + (20 * i), 0x000000, "isOk_ : %d", isOk_[i]);

		//DrawFormatString(0, 20 + (20 * i), 0Xff0000, "player : %0.2f,%0.2f,%0.2f",
		//	players_[0]->GetPosArray(i).x, players_[0]->GetPosArray(i).y, players_[0]->GetPosArray(i).z);

		//DrawFormatString(0, 120 + (20 * i), 0Xff0000, "player : %0.2f,%0.2f,%0.2f",
		//	players_[1]->GetPosArray(i).x, players_[1]->GetPosArray(i).y, players_[1]->GetPosArray(i).z);
		//
		//DrawFormatString(0, 20 + (20 * i), 0Xff0000, "ver.pos : %0.2f,%0.2f,%0.2f",
		//	images_[i]->GetVerPos().x, images_[i]->GetVerPos().y, images_[i]->GetVerPos().z);

		//DrawFormatString(0, 120 + (20 * i), 0x00CC00, "input_[%d]: %d", i, input_[i].cntl_);
		//DrawFormatString(500, 40 + (20 * i), 0x00CC00, "pos: %2.f,%2.f,%2.f", images_[i]->GetMeshVertex(i).pos.x, images_[i]->GetMeshVertex(i).pos.y, images_[i]->GetMeshVertex(i).pos.z);
		DrawFormatString(500, 40 + (20 * i), 0x00CC00, "isOk: %d", isOk_[i]);
		//DrawFormatString(Application::SCREEN_SIZE_X - 100, 100 + (i*20), 0xFF3333, "ready : %d", images_[i]->GetReady());
	}
	DrawFormatString(100, 120, 0x00CC00, "input_: %d", input_[0].config_);

}

void SelectScene::ChangeSelect(const SELECT _state)
{
	//��ԑJ��
	select_ = _state;

	// �e��ԑJ�ڂ̏�������
	stateChanges_[select_]();
}

void SelectScene::KeyConfigSetting(void)
{
	auto& ins = InputManager::GetInstance();

	for (auto& i : input_)
	{
		// ����������ĂȂ��Ƃ�
		i.config_ = KEY_CONFIG::NONE;
	}
	
	//2P�ȍ~�͑S��PAD����
	for (int i = 1; i < SceneManager::PLAYER_NUM; i++) {
		input_[i].cntl_ = SceneManager::CNTL::PAD;
	}

	switch (Get1PDevice())
	{
	case SceneManager::CNTL::KEYBOARD:

		KeyBoardProcess();
		break;

	case SceneManager::CNTL::PAD:

		PadProcess();

		break;
	default:
		break;
	}
}

void SelectScene::KeyBoardProcess(void)
{
	auto& ins = InputManager::GetInstance();

	//�L�[�̉�������
	if (ins.IsNew(KEY_INPUT_UP)	  || ins.IsNew(KEY_INPUT_W))	input_[0].config_ = KEY_CONFIG::UP;
	if (ins.IsNew(KEY_INPUT_DOWN) || ins.IsNew(KEY_INPUT_S))	input_[0].config_ = KEY_CONFIG::DOWN;
	if (ins.IsNew(KEY_INPUT_LEFT) || ins.IsNew(KEY_INPUT_A))	input_[0].config_ = KEY_CONFIG::LEFT;
	if (ins.IsNew(KEY_INPUT_RIGHT) || ins.IsNew(KEY_INPUT_D))	input_[0].config_ = KEY_CONFIG::RIGHT;

	//�L�[�̉�������(�������u�Ԃ���)
	if (ins.IsTrgDown(KEY_INPUT_UP)  || ins.IsTrgDown(KEY_INPUT_W))input_[0].config_ = KEY_CONFIG::UP_TRG;
	if (ins.IsTrgDown(KEY_INPUT_DOWN) || ins.IsTrgDown(KEY_INPUT_S))input_[0].config_ = KEY_CONFIG::DOWN_TRG;
	if (ins.IsTrgDown(KEY_INPUT_LEFT) || ins.IsTrgDown(KEY_INPUT_A))input_[0].config_ = KEY_CONFIG::LEFT_TRG;
	if (ins.IsTrgDown(KEY_INPUT_RIGHT) || ins.IsTrgDown(KEY_INPUT_D))input_[0].config_ = KEY_CONFIG::RIGHT_TRG;

	if (ins.IsTrgDown(KEY_INPUT_SPACE) || ins.IsTrgDown(KEY_INPUT_RETURN))input_[0].config_ = KEY_CONFIG::DECIDE;
	if (ins.IsTrgDown(KEY_INPUT_C))input_[0].config_ = KEY_CONFIG::CANCEL;
}

void SelectScene::PadProcess(void)
{
	auto& ins = InputManager::GetInstance();

	// ���X�e�B�b�N�̉���
	int leftStickX_[SceneManager::PLAYER_NUM];
	//�c��
	int leftStickY_[SceneManager::PLAYER_NUM];

	leftStickX_[0] = ins.GetJPadInputState(InputManager::JOYPAD_NO::PAD1).AKeyLX;
	leftStickX_[1] = ins.GetJPadInputState(InputManager::JOYPAD_NO::PAD2).AKeyLX;
	leftStickX_[2] = ins.GetJPadInputState(InputManager::JOYPAD_NO::PAD3).AKeyLX;
	leftStickX_[3] = ins.GetJPadInputState(InputManager::JOYPAD_NO::PAD4).AKeyLX;

	leftStickY_[0] = ins.GetJPadInputState(InputManager::JOYPAD_NO::PAD1).AKeyLY;
	leftStickY_[1] = ins.GetJPadInputState(InputManager::JOYPAD_NO::PAD2).AKeyLY;
	leftStickY_[2] = ins.GetJPadInputState(InputManager::JOYPAD_NO::PAD3).AKeyLY;
	leftStickY_[3] = ins.GetJPadInputState(InputManager::JOYPAD_NO::PAD4).AKeyLY;

	int mainCntl = 0;

	if (Get1PDevice() == SceneManager::CNTL::KEYBOARD)
	{
		mainCntl = 1;
	}
	for (int i= mainCntl; i < 4; i++)
	{
		if (leftStickY_[i] < -1)
		{
			input_[i].config_ = KEY_CONFIG::UP;

		}
		if (leftStickY_[i] > 1)
		{
			input_[i].config_ = KEY_CONFIG::DOWN;
		}

		if (leftStickX_[i] < -1)
		{
			input_[i].config_ = KEY_CONFIG::LEFT;
		}
		if (leftStickX_[i] > 1)
		{
			input_[i].config_ = KEY_CONFIG::RIGHT;
		}
		if (ins.IsPadBtnTrgDown(static_cast<InputManager::JOYPAD_NO>(i + 1), InputManager::JOYPAD_BTN::RIGHT))
		{
			input_[i].config_ = KEY_CONFIG::DECIDE;
		}
		if (ins.IsPadBtnTrgDown(static_cast<InputManager::JOYPAD_NO>(i + 1), InputManager::JOYPAD_BTN::DOWN))
		{
			input_[i].config_ = KEY_CONFIG::CANCEL;
		}
	}
}

void SelectScene::Change1PDevice(SceneManager::CNTL cntl)
{
	input_[0].cntl_ = cntl;
}

void SelectScene::ControllDevice(void)
{
	InputManager& ins = InputManager::GetInstance();
	int key = CheckHitKeyAll(DX_CHECKINPUT_KEY);		//���͂𒲂ׂ�(�L�[�{�[�h)
	int padNum = GetJoypadNum();
	int padState = CheckHitKeyAll(DX_CHECKINPUT_PAD);	//���͂𒲂ׂ�(�p�b�h)

	DataBank& data = DataBank::GetInstance();
	SceneManager::CNTL maincCntl = data.Output(1).cntrol_;

	//�L�[�{�[�h����̎��p�b�h������ł��Ȃ��悤��
	if (key != 0	&&
		padState == 0)
	{
		Change1PDevice(SceneManager::CNTL::KEYBOARD);
	}//�p�b�h����̎��L�[������ł��Ȃ��悤��
	else if (key <= 0 &&
		padNum > 0 &&
		padState != 0)
	{
		Change1PDevice(SceneManager::CNTL::PAD);
	}
}

SelectScene::KEY_CONFIG SelectScene::GetConfig(void)
{
	return input_[0].config_;
}

bool SelectScene::IsAllReady(void)
{
	auto camera = SceneManager::GetInstance().GetCameras();
	for (int i = 0; i < camera.size(); i++)
	{
		if (!isOk_[i])
		{
			return false;
		}
	}
	return true;
}
