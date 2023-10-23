#pragma once
#include "../BaseObject.h"
#include "../../Input/Input.h"

#include "../../Stage/BaseStage.h"



/// <summary>
/// カタパルト
/// </summary>
class Catapult : public BaseObject
{
public: // メンバ関数
	Catapult();
	~Catapult() override;

	// 固有の初期化処理
	void Initialize(std::string name, Tag tag) override;
	// 更新処理
	void Update() override;
	// 描画処理
	void Draw() override;

public: // その他関数群

	/// <summary>
	/// グローバル変数に値を追加する関数
	/// </summary>
	void AddGlobalVariables() override;

	/// <summary>
	/// グローバル変数の値を適用する関数
	/// </summary>
	void ApplyGlobalVariables() override;

	/// <summary>
	/// 衝突した瞬間にコールバックされる関数
	/// </summary>
	/// <param name="object">衝突したオブジェクト</param>
	void OnCollisionEnter(BaseObject* object)override;

	/// <summary>
	/// 衝突時コールバック関数
	/// </summary>
	/// <param name="object">衝突したオブジェクト</param>
	void OnCollision(BaseObject* object) override;

	/// <summary>
	/// 非衝突時コールバック関数
	/// </summary>
	/// <param name="object">前フレーム衝突していたオブジェクト</param>
	void OnCollisionExit(BaseObject* object) override;

	void SetCatapultInfo(const BaseStage::CatapultInfo& info);

	void SetPosition(const Vector3& pos) { transform_.translate_ = pos; }
	void SetIsRePop(bool isRePop) { isRePop_ = isRePop; }
	void SetPopTime(int32_t popTime) { kPopTime_ = popTime; }

	bool GetJumpEnable()const { return isJumpEnable_; }
	float GetTheta() const;

	void AirJump();

private: // メンバ関数

	// 作成した関数等の初期化
	void InitializeVariables();

	// 位置の補正
	void UpdateCatapult();

	// デバッグ時にのみ動くGUI
	void DebugGui();

private: // メンバ変数

	/*----------------------//
	//	カタパルト自体の変数	//
	//----------------------*/

	// 自分が何番目の数字なのか
	//int kIndex_ = 0;

	Player* player_ = nullptr;

	// カタパルトの初期角度
	float kTheta_ = 0.0f;

	// 中心からの距離
	float kLength_ = 1.0f;

	// アイテムの当たり判定の半径
	float radius_ = 1.0f;

	// 踏まれた後に使えなくなる時間
	int32_t kPopTime_ = 100;
	// こちらの数値を動かす
	int32_t rePopTime_ = 0;

	// 使用時の色
	Vector4 usedColor_;

	WorldTransform allowTransform_;

	/*--------------------------------------//
	//	フラグによって操作も管理するために、		//
	//	一度クッションをはさむ					//
	//--------------------------------------*/

	// ジャンプできるかどうかのフラグ
	bool isJumpEnable_ = false;

	// 踏まれて消えるか消えないか
	bool isRePop_ = false;


#ifdef _DEBUG

	/*------------------//
	//	デバッグ用の変数	//
	//------------------*/



#endif // _DEBUG


};