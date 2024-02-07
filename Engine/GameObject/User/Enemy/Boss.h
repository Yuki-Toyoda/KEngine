#pragma once
#include"IEnemy.h"
#include "../../GameObjectManager.h"

// クラスの前方宣言
class ParticleEmitterManager;
class InGameCamera;

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
	/// インゲームカメラのセッター
	/// </summary>
	/// <param name="inGameCamera">使用中インゲームカメラ</param>
	void SetInGameCamera(InGameCamera* inGameCamera) { inGameCamera_ = inGameCamera; }

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

	/// <summary>
	/// チュートリアルフラグのセッター
	/// </summary>
	/// <param name="flag">チュートリアルを有効にするか</param>
	void SetIsTutrial(bool flag) { isTutrial_ = flag; }

	/// <summary>
	/// 攻撃チュートリアルが終了しているかのゲッター
	/// </summary>
	/// <returns>攻撃チュートリアルが終了しているか</returns>
	bool GetTutrialAtackEnd() { return tutrialAtackEnd_; }
	/// <summary>
	/// 攻撃チュートリアルの終了状態セッター
	/// </summary>
	/// <param name="flag">攻撃チュートリアルを終了するか</param>
	void SetTutrialAtackEnd(bool flag) { tutrialAtackEnd_ = flag; }

	
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

	// インゲーム内のカメラ
	InGameCamera* inGameCamera_ = nullptr;

	//体力
	float hitPoint_;
	// 最大体力
	float kMaxHitPoint_;

	// 衝突時のシェイク秒数
	float shakeTime_ = 0.5f;
	// 衝突時のシェイク強さ
	Vector2 shakeStrength_ = { 1.0f, 1.0f };
	// シェイク強さの倍率
	float shakeMagnification_ = 1.0f;
	// 衝突時のシェイク強さ最大値
	Vector2 maxShakeStrength_ = { 15.0f, 15.0f };

	// ゲームオブジェクトマネージャ
	GameObjectManager* gameObjectManager_;

	// 画面フェードトリガー
	bool isFade_ = false;
	// シーン遷移トリガー
	bool isSceneChange_ = false;
	bool isTutrial_=true;
	bool tutrialAtackEnd_=false;
	
};
