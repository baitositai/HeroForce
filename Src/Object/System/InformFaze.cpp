#include"../../Manager/Generic/ResourceManager.h"
#include"../../Scene/GameScene.h"
#include "InformFaze.h"

InformFaze::InformFaze(void)
{
	informStrPos_ = { 0.0f,0.0f,0.0f };
	fazeCnt_ = 0;
	informCnt_ = 0;
	missionImg_[0] = ResourceManager::GetInstance().Load(ResourceManager::SRC::MISSION_NOMAL).handleId_;
	missionImg_[1] = ResourceManager::GetInstance().Load(ResourceManager::SRC::MISSION_LAST).handleId_;
	font_ = CreateFontToHandle(NULL, 80, 4, DX_FONTTYPE_EDGE);
	missionCnt_ = 0;

	informStrPos_ = { 0.0f,STRING_SPOS_Y,0.0f };
	informImgPos_ = { Application::SCREEN_SIZE_X / 2.0f,
		MISSION_SPOS_Y,
		0.0f };
}

InformFaze::~InformFaze(void)
{
}

bool InformFaze::Update(void)
{
	//�J�E���g��������傫���Ȃ�����I��
	if (informCnt_ > INFORM_FINISH)return false;

	informCnt_++;

	//�ڕW�n�_�ł͂Ȃ��Ƃ����W���ړ�
	if (informStrPos_.y != STRING_FPOS_Y)informStrPos_.y++;
	//�ʒm�̐܂�Ԃ��ɍs���ĂȂ��ꍇ�����ōX�V�I��
	if (informCnt_ <= INFORM_HARF)return true;

	//�܂�Ԃ���Ȃ̂Ń~�b�V������\�����̍X�V����
	//�ڕW�n�_�ł͂Ȃ��Ƃ����W���ړ�
	if (informImgPos_.y != MISSION_FPOS_Y)informImgPos_.y++;

	return true;
}

void InformFaze::Draw(void)
{
	//�ʒm���̔����̓t�F�[�Y���\��
	if (informCnt_ <= INFORM_HARF)
	{
		//�t�F�[�Y���\��
		DrawFormatStringToHandle(informStrPos_.x, informStrPos_.y, 0xff0000, font_, "��%d�t�F�[�Y", fazeCnt_);
		return;
	}
	
	DrawRotaGraph(informImgPos_.x, informImgPos_.y,
		0.5f,
		0.0f,
		missionImg_[missionCnt_],
		true);
}

void InformFaze::SetFazeCnt(const int _cnt)
{
	//�J�E���g�̐ݒ�y�ѕ��͂̍쐬
	fazeCnt_ = _cnt;
	std::string fazeStr_ = "��" + std::to_string(fazeCnt_) + "�t�F�[�Y";
	int len = (int)strlen(fazeStr_.c_str());
	int width = GetDrawStringWidthToHandle(fazeStr_.c_str(), len, font_);

	//�ʒu�̐ݒ�
	informStrPos_.x= Application::SCREEN_SIZE_X / 2.0f - width / 2.0f;
	informStrPos_.y = STRING_SPOS_Y;
	informImgPos_.y = MISSION_SPOS_Y;
	//�J�E���g������
	informCnt_ = 0;

	if (fazeCnt_ == GameScene::LAST_FAZE)missionCnt_ = 1;
	else missionCnt_ = 0;
}
