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
	/// カメラシェイク関数
	/// </summary>
	void Shake();



public: // アクセッサ等

	/// <summary>
	/// カメラシェイク状態ゲッター
	/// </summary>
	/// <returns>カメラシェイク状態</returns>
	bool GetIsShake() { return isShake_; }

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
	
	// 目標角度Y軸
	float targetAngleY_ = 0.0f;

	//シェイクする時間
	int shakeCount_;
	int maxShakeCount_=30;
	//画面シェイクの強さ
	float shakeForce_;
	float kShakeForce_=4.0f;
	//前フレームの座標
	Vector3 translate_;
	//シェイクしているか
	bool isShake_;

private:

	/// <summary>
	/// データをセーブする関数
	/// </summary>
	void SaveData();

};
