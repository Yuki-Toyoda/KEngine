#pragma once
#include "../../BaseObject.h"
#include "../Player/Weapon.h"

/// <summary>
/// 敵の基底クラス
/// </summary>
class IEnemy : public BaseObject
{
public: // メンバ関数

	/// <summary>
	/// 共通初期化関数
	/// </summary>
	void PreInit();

	/// <summary>
	/// 初期化関数
	/// </summary>
	void Init() override = 0;

	/// <summary>
	/// 更新関数
	/// </summary>
	void Update() override = 0;

	/// <summary>
	/// ImGui表示関数
	/// </summary>
	void DisplayImGui() override {};

	/// <summary>
	/// リセット関数
	/// </summary>
	virtual void Reset() = 0;

	/// <summary>
	/// パラメータのセーブ関数
	/// </summary>
	/// <param name="levelName">レベル名</param>
	/// <param name="enemyName">敵名</param>
	virtual void SaveParameter(const std::string& levelName, const std::string& enemyName) = 0;

	/// <summary>
	/// パラメータのロード関数
	/// </summary>
	/// <param name="levelName">レベル名</param>
	/// <param name="enemyName">敵名</param>
	virtual void LoadParameter(const std::string& levelName, const std::string& enemyName) = 0;

	/// <summary>
	/// 衝突した瞬間にコールバックされる関数
	/// </summary>
	/// <param name="collider">衝突したコライダー</param>
	void OnCollisionEnter(Collider* collider) override = 0;

public: // アクセッサ等

	/// <summary>
	/// 武器のセッター
	/// </summary>
	/// <param name="target"></param>
	void SetWepon(Weapon* weapon) { weapon_ = weapon; }

	/// <summary>
	/// 移動ベクトルセッター
	/// </summary>
	/// <param name="velocity">移動ベクトル</param>
	void Setveclocity(Vector3 velocity) { hitvelocity_ = velocity; }

protected: // 継承先メンバ変数

	// 音再生用クラス
	Audio* audio_;

	// 武器
	Weapon* weapon_ = nullptr;
	// 武器とペアレントをしているか
	bool isParent_;

	// 当たり判定用ワールド座標
	Vector3 worldPos_;

	//速度方向ベクトル
	Vector3 velocity_;
	Vector3 hitvelocity_;

};

