#pragma once
#include "Engine/GameObject/IObject.h"

// クラスの前方宣言
class Player;

/// <summary>
/// ナ〇ィ
/// </summary>
class Fairy : public IObject
{
public: // メンバ関数

	/// <summary>
	/// 初期化関数
	/// </summary>
	void Init() override;

	/// <summary>
	/// 更新関数
	/// </summary>
	void Update() override;

	// ImGui表示関数
	// ImGuiを表示させたい場合はこの関数に処理を追記
	void DisplayImGui() override;

	/// <summary>
	/// カメラの座標を動かした際の更新関数
	/// </summary>
	void UpdateTarget();

	/// <summary>
	/// オフセット計算関数
	/// </summary>
	/// <returns>計算後オフセット</returns>
	Vector3 CalcOffset() const;

public: // アクセッサ

	/// <summary>
	/// 追従対象セッター
	/// </summary>
	/// <param name="target">追従対象</param>
	void SetTarget(const WorldTransform* target) { target_ = target; }

	/// <summary>
	/// プレイヤーセッター
	/// </summary>
	/// <param name="player">プレイヤー</param>
	void SetPlayer(Player* player) { player_ = player; }

private: // メンバ変数

	// プレイヤー
	Player* player_ = nullptr;

	// 追従対象のワールド座標
	const WorldTransform* target_ = nullptr;

	// オフセットなしのカメラのワールドトランスフォーム
	WorldTransform noOffsetTransform_;

	// 追従対象の残像座標
	Vector3 interTarget_ = {};
	// カメラの追従遅延量
	const float trackingDelay_ = 0.1f;

	// オフセット
	const Vector3 kOffset_ = { -0.5f, 1.75f, -0.8f };
	Vector3 offset_ = kOffset_;

};

