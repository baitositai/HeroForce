#include<DxLib.h>
#include<string>
#include"DataBank.h"
#include"../Generic/ResourceManager.h"
#include "Timer.h"

Timer* Timer::instance_ = nullptr;

Timer::Timer(void)
{
	minute_ = 1;
	second_ = 0;
	cnt_ = 0;
	isEnd_ = false;

	DataBank& data = DataBank::GetInstance();

	//fontSize_ = TIME_FONT_SIZE / data.Output(DataBank::INFO::USER_NUM);
	//font_ = CreateFontToHandle(NULL, fontSize_, 4, DX_FONTTYPE_EDGE);

	std::string time = "残り" + std::to_string(minute_) + ":" + std::to_string(second_);
	int len = (int)strlen(time.c_str());
	//strWidth_ = GetDrawStringWidthToHandle(time.c_str(), len, font_);

	//pos_.x = static_cast<float>(data.Output(DataBank::INFO::SCREEN_SIZE, DataBank::GET_SIZE::X))/2.0f;
	pos_.x = Application::SCREEN_SIZE_X / 2.0f;
	pos_.y = 0.0f;
	pos_.z = 0.0f;

	imgNumbers_ = ResourceManager::GetInstance().Load(ResourceManager::SRC::NUMBERS2).handleIds_;
	imgTimerBack_ = ResourceManager::GetInstance().Load(ResourceManager::SRC::TIMER_BACK).handleId_;
}

Timer::~Timer(void)
{
	//DeleteFontToHandle(font_);
}

void Timer::CreateInstance(void)
{
	if (instance_ != nullptr)return;
	instance_ = new Timer();
}

Timer& Timer::GetInstance(void)
{
	return *instance_;
}

void Timer::Update()
{
	cnt_++;
	//FPS参照のカウント更新
	//FPSと同じ値のときは一秒立ったということ
	if (cnt_ >= Application::DEFAULT_FPS) {
		cnt_ = 0;

		//時間制限を減らす
		ReduceTime();
	}
}

void Timer::Draw()
{
	/*DrawBoxAA(pos_.x, pos_.y,
		pos_.x + static_cast<int>(strWidth_ * 1.5f), pos_.y + fontSize_,
		0xff0000,
		0x222222);

	DrawFormatStringToHandle(pos_.x, 0, 0xffff00, font_, "残り[%d:%d]", minute_, second_);*/

	//背景描画
	DrawRotaGraph(
		Application::SCREEN_SIZE_X / 2,
		TIMER_BACK_SIZE_Y / 2,
		TIMER_BACK_RATE,
		0.0f,
		imgTimerBack_,
		true);
	int num[NUM_CNT] = { second_ / 10 % 10, second_ % 10 };
	//秒数の描画
	for (int i = 0; i < NUM_CNT; i++) {
		DrawRotaGraph(
			NUM_POS_X[i],
			NUM_POS_Y,
			NUM_RATE,
			0.0f,
			imgNumbers_[num[i]],
			true);
	}
}

void Timer::Reset()
{
	minute_ = 1;
	second_ = 0;
	cnt_ = 0;
	isEnd_ = false;
}

void Timer::Release()
{
	instance_ = nullptr;
}

void Timer::Destroy()
{
	delete instance_;
}


void Timer::ReduceTime(void)
{
	//秒数を減らす
	second_--;

	//秒数が０未満になった時
	if (second_ < TIME_MIN) {
		//分数を減らす
		minute_--;
		//秒数を５９秒に
		second_ = TIME_MAX;
	}

	//分数が０未満のとき終了状態に
	if (minute_ <= TIME_MIN && second_ <= TIME_MIN)
	{
		isEnd_ = true;
	}
}