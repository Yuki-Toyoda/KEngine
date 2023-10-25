#pragma once
#include "../BaseObject.h"
#include "../../Input/Input.h"
#include "../../Audio/Audio.h"

#include "../../Stage/BaseStage.h"

/// <summary>
/// アイテム
/// </summary>
class Item : public BaseObject
{
public: // メンバ関数

	Item();
	~Item() override;

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

	void SetItemInfo(const BaseStage::ItemInfo& info);

	void SetPosition(const Vector3& pos) { transform_.translate_ = pos; }
	void SetIsRePop(bool isRePop) { isRePop_ = isRePop; }
	bool GetIsRePop() { return isRePop_; }
	void SetPopTime(int32_t popTime) { kPopTime_ = popTime; }

	bool GetJumpEnable()const { return isJumpEnable_; }

	void AirJump();

	bool GetIsUsed()const { return isUsed_; }

private: // メンバ関数

	// 作成した関数等の初期化
	void InitializeVariables();

	// デバッグ時にのみ動くGUI
	void DebugGui();

private: // メンバ変数

	/*----------------------//
	//	アイテム自体の変数		//
	//----------------------*/

	// 自分が何番目の数字なのか
	//int kIndex_ = 0;

	// 音再生
	Audio* audio_ = nullptr;
	// 音量
	float* seVolume_; // SE

	// アイテム使用時の音
	uint32_t soundHandleUseItem_;

	// アイテムの当たり判定の半径
	float radius_ = 1.0f;

	// 踏まれた後に使えなくなる時間
	int32_t kPopTime_ = 0;
	// こちらの数値を動かす
	int32_t rePopTime_ = 0;

	// 使用時の色
	Vector4 usedColor_;

	// イージング用変数
	float rotateT_ = 0;

	// 浮遊ギミックをやりたい
	float floatingT_ = 0;

	// 描画のために保存する
	WorldTransform bufferWorldTransform_;

	/*--------------------------------------//
	//	フラグによって操作も管理するために、		//
	//	一度クッションをはさむ					//
	//--------------------------------------*/

	// ジャンプできるかどうかのフラグ
	bool isJumpEnable_ = false;

	// 踏まれて消えるか消えないか
	bool isRePop_ = false;

	bool isUsed_ = false;

#ifdef _DEBUG

	/*------------------//
	//	デバッグ用の変数	//
	//------------------*/



#endif // _DEBUG


};