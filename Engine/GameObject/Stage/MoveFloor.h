#pragma once
#include "../BaseObject.h"

/// <summary>
/// 動作する床クラス
/// </summary>
class StageMoveFloor : public BaseObject
{
public: // メンバ関数

	// 固有の初期化処理
	void Initialize(std::string name, Tag tag) override;
	// 更新処理
	void Update() override;
	// 描画処理
	void Draw() override;

public: // その他関数

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

	/// <summary>
	/// 開始座標セッター
	/// </summary>
	/// <param name="startPos">開始座標</param>
	void SetStartPos(Vector3 startPos) { startPos_ = startPos; }

	/// <summary>
	/// 終端座標セッター
	/// </summary>
	/// <param name="endPos">終端座標</param>
	void SetEndPos(Vector3 endPos) { endPos_ = endPos; }

private: // メンバ変数

	// イージング用t
	float t_;

	// 折り返しトリガー
	bool isReturn_;

	// 開始座標
	Vector3 startPos_;
	// 終端座標
	Vector3 endPos_;

};

