#pragma once

#include"UnitBase.h"
#include"Common/Transform.h"

class Arrow
{
public:

	//定数
	static constexpr VECTOR ARROW_LOCAL_POS = { 15.0f,30.0f,50.0f };	//打ち始めに違和感がないように調整
	static constexpr float GRAVITY = 0.5f;			//重力
	static constexpr float START_UP_ANGLE = 10.0f;	//最初の上アングル

	enum class STATE
	{
		NONE,
		SHOT,
		DESTROY,
		END,
	};

	//モデルID、発生位置、行き先,方向,対象位置,攻撃力
	Arrow(void);	
	~Arrow(void) = default;

	/// <summary>
	/// 初期化
	/// </summary>
	/// <param name="_mdlId">矢のモデルID</param>
	/// <param name="_trans">アーチャーの位置情報等</param>
	/// <param name="_speed">速度</param>
	void Init(const int _mdlId, const Transform& _trans, const float _speed);
	void Update(UnitBase::ATK& _atk);
	void Draw(void);
	void Release();
	void Destroy(void);

	const VECTOR GetPos(void)const { return trans_.pos; }
	const bool GetIsAlive(void)const { return isAlive_; }
	const STATE GetState(void)const { return state_; }

	//状況変位
	void ChangeState(const STATE _state) { state_ = _state; }

	////回転方向制御
	//void SetQuaRot(const Quaternion& _quaRot) { trans_.quaRot = _quaRot; }

	////位置設定
	//void SetPos(const VECTOR& _pos) { trans_.pos = _pos; }

private:
	//位置情報
	Transform trans_;
	//攻撃対象
	VECTOR targetPos_;
	//状態
	STATE state_;
	//攻撃力
	float atkPow_;
	//速度
	float speed_;
	//生存判定
	bool isAlive_;

	//移動処理
	void Move(void);
	//生存フラグ設定
	void SetIsAlive(const bool _flag) { isAlive_ = _flag; }
};

