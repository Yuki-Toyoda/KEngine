#pragma once
#include "../BaseObject.h"

/// <summary>
/// ゴールオブジェクト
/// </summary>
class Goal : public BaseObject
{
public: // メンバ関数

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
	/// 衝突時コールバック関数
	/// </summary>
	/// <param name="object">衝突したオブジェクト</param>
	void OnCollision(BaseObject* object) override;

	/// <summary>
	/// 非衝突時コールバック関数
	/// </summary>
	/// <param name="object">前フレーム衝突していたオブジェクト</param>
	void OnCollisionExit(BaseObject* object) override;

public: // アクセッサ等
	
	/// <summary>
	/// ゴール状態ゲッター
	/// </summary>
	/// <returns>ゴールしているか</returns>
	bool GetIsGoaled() { return isGoaled_; }

private: // メンバ変数

	// ゴールしたか
	bool isGoaled_;

};

