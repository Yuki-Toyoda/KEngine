#pragma once
#include "../BaseObject.h"
#include "../../Input/Input.h"

/// <summary>
/// プレイヤー
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

	void SetPosition(const Vector3& pos) { transform_.translate_ = pos; }

private: // メンバ関数

	// 作成した関数等の初期化
	void InitializeVariables();

	// デバッグ時にのみ動くGUI
	void DebugGui();

private: // メンバ変数

	/*----------------------//
	//	アイテム自体の変数		//
	//----------------------*/

	// アイテムの当たり判定の半径
	float radius_ = 1.0f;

	/*--------------------------------------//
	//	フラグによって操作も管理するために、		//
	//	一度クッションをはさむ					//
	//--------------------------------------*/
	
	// ジャンプするかどうかのフラグ
	bool isJumpTrigger_ = false;

	// 踏まれて消えるか消えないか
	bool isRePop_ = false;


#ifdef _DEBUG

	/*------------------//
	//	デバッグ用の変数	//
	//------------------*/



#endif // _DEBUG


};