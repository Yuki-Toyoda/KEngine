#pragma once
#include"../../Core/Camera.h"
#include "../../../Input/Input.h"

// クラスの前方宣言
class Player;

/// <summary>
/// インゲーム中のカメラ
/// </summary>
class InGameCamera: public Camera
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

	/// <summary>
	/// ImGui表示関数
	/// </summary>
	void DisplayImGui() override;

	/// <summary>
	/// カメラシェイク開始関数
	/// </summary>
	/// <param name="shakeTime">シェイク時間</param>
	/// <param name="strength">シェイク強さ</param>
	void Shake(const float& time, const Vector2& strength);

public: // アクセッサ等

	/// <summary>
	/// プレイヤーセッター
	/// </summary>
	/// <param name="player">セットするプレイヤー本体</param>
	void SetPlayer(Player* player) { player_ = player; }

private: // メンバ関数

	// プレイヤー
	Player* player_ = nullptr;

	// 残像座標
	Vector3 interTarget_ = {};
	// カメラのオフセット
	Vector3 offset_ = { 0.0f, -15.0f, -100.0f };
	
	// 振動オフセット
	Vector3 shakeOffset_ = { 0.0f, 0.0f, 0.0f };
	// 振動強さ格納用
	Vector2 shakeStrength_ = { 0.0f, 0.0f };
	// 振動強さ始端値
	Vector2 startShakeStrength_ = { 0.0f, 0.0f };
	// 振動演出タイマー
	KLib::DeltaTimer shakeTimer_;

	// 目標角度Y軸
	float targetAngleY_ = 0.0f;

	// ImGui調整用の振動強さ変数
	Vector2 imGuiShakeStrength_ = { 15.0f, 15.0f };
	// ImGui調整用の振動時間
	float imGuiShakeTime_ = 0.25f;

private:

	/// <summary>
	/// データをセーブする関数
	/// </summary>
	void SaveData();

};
