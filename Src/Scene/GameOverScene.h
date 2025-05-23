#pragma once
#include <functional>
#include <memory>
#include <map>
#include "SceneBase.h"
#include "../Object/Character/PlayableChara/Other/OverPlayers.h"

class OverPlayers;

class GameOverScene : public SceneBase
{
public:

	//状態
	enum class STATE
	{
		DOWN,		//プレイヤーが倒れる
		DISPLAY,	//メッセージの表示
	};

	GameOverScene();
	~GameOverScene() = default;

	//メッセージ位置
	static constexpr int MES_DEFAULT_POS_X = Application::SCREEN_SIZE_X / 2;
	static constexpr int MES_DEFAULT_POS_Y = Application::SCREEN_SIZE_Y / 2 - 100;

	static constexpr int MES2_DEFAULT_POS_X = Application::SCREEN_SIZE_X / 2;
	static constexpr int MES2_DEFAULT_POS_Y = Application::SCREEN_SIZE_Y / 2 +100;

	//カメラ関連
	static constexpr VECTOR DEFAULT_CAMERA_POS = { 0.0f, 100.0f, -500.0f };		//カメラの座標
	static constexpr VECTOR DEFAULT_TARGET_POS = { 0.0f, 150.0f, -100.0f };		//カメラの注視点座標

	//アルファ値最大
	static constexpr int ALPHA_MAX = 255;
	static constexpr int ALPHA_MIN = 130;

	//状態遷移秒数
	static constexpr float CHANGE_SECOND = 2.0f;

	void Init() override;
	void Update() override;
	void Draw() override;
	void Release() override;

private:

	//状態
	STATE state_;

	//画像
	int imgGameOver_;		//ゲームオーバー画像
	int imgBack_;			//背景画像
	int imgMes_;			//メッセージ画像

	//更新用ステップ
	float step_;

	//アルファ値
	float alpha_;
	float alphaAdd_;

	// 状態管理(状態遷移時初期処理)
	std::map<STATE, std::function<void()>> stateChanges_;

	// 状態管理
	std::function<void()> stateUpdate_;	//更新

	//状態変更
	void ChangeState(const STATE state);
	void ChangeStateDown();
	void ChangeStateDisplay();

	//各種更新処理
	void UpdateDown();
	void UpdateDisplay();
	
	//プレイヤー
	std::unique_ptr<OverPlayers> player_;
};

