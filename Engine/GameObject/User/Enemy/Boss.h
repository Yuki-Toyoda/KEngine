#pragma once
#include"IEnemy.h"
#include "../../GameObjectManager.h"

// クラスの前方宣言
class ParticleEmitterManager;

/// <summary>
/// ボスのクラス
/// </summary>
class Boss:public IEnemy
{
public: // メンバ関数

	/// <summary>
	/// 固有初期化関数
	/// </summary>
	void SuccessorInit() override;

	/// <summary>
	/// 固有更新関数
	/// </summary>
	void SuccessorUpdate() override;

	/// <summary>
	/// ImGui表示関数
	/// </summary>
	void DisplayImGui() override;

	/// <summary>
	/// リセット関数
	/// </summary>
	void Reset() override;

	/// <summary>
	/// パラメータのセーブ関数
	/// </summary>
	/// <param name="levelName">レベル名</param>
	/// <param name="enemyName">敵名</param>
	void SetParameter(const std::string& levelName, const std::string& enemyName) override;

	/// <summary>
	/// パラメータのロード関数
	/// </summary>
	/// <param name="levelName">レベル名</param>
	/// <param name="enemyName">敵名</param>
	void ApplyParameter(const std::string& levelName, const std::string& enemyName) override;

public: // アクセッサ等

	/// <summary>
	/// 現在HPのゲッター
	/// </summary>
	/// <returns>現在HP</returns>
	float GetHP() { return hitPoint_; }
	
	/// <summary>
	/// 最大HPゲッター
	/// </summary>
	/// <returns>最大HP</returns>
	float GetMaxHP() { return kMaxHitPoint_; }

	/// <summary>
	/// シーン遷移トリガー状態ゲッター
	/// </summary>
	/// <returns>シーン遷移トリガー状態</returns>
	bool GetIsSceneChage() { return isSceneChange_; }

private: // メンバ関数

	/// <summary>
	/// 衝突した瞬間にコールバックされる関数
	/// </summary>
	/// <param name="collider">衝突したコライダー</param>
	void OnCollisionEnter(Collider* collider) override;

	/// <summary>
	/// 衝突中コールバック関数
	/// </summary>
	/// <param name="collider">衝突したコライダー</param>
	void OnCollision(Collider* collider)override;

	//StateListを作成する
	void MakeStateList();
	//GlobalVariablesを代入
	void ApplyGlobalVariables();
	//行動状態を生成
	std::unique_ptr<IEnemyState>MakeState(std::string name);
private:

	// パーティクルエミッタマネージャ
	ParticleEmitterManager* pem_ = nullptr;

	//体力
	float hitPoint_;
	// 最大体力
	float kMaxHitPoint_;

	// ゲームオブジェクトマネージャ
	GameObjectManager* gameObjectManager_;

	// 画面フェードトリガー
	bool isFade_ = false;
	// シーン遷移トリガー
	bool isSceneChange_ = false;

};
