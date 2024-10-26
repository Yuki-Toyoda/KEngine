#pragma once
#include "Engine/GameObject/IObject.h"
#include "Engine/Input/Input.h"
#include "State/StateList.h"
#include "Engine/Utility/Animation/AnimationManager.h"
#include "Engine/Audio/Audio.h"
#include "App/GameObject/User/Player/Combo/ComboManager.h"

// クラスの前方宣言
class Camera;
class FollowCamera;
class LockOn;
class Enemy;
class GameManager;

/// <summary>
/// プレイヤー
/// </summary>
class Player : public IObject
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
	/// 衝突した瞬間にコールバックされる関数
	/// </summary>
	/// <param name="collider">衝突したコライダー</param>
	void OnCollisionEnter(Collider* collider) override;

public: // 行動状態関数群

	/// <summary>
	/// 行動状態変更関数
	/// </summary>
	/// <param name="newState"></param>
	void ChangeState(std::unique_ptr<IState> newState);

	/// <summary>
	/// 行動状態の取得
	/// </summary>
	/// <returns></returns>
	const std::string GetStateName() { return state_->GetStateName(); }

public: // その他関数

	/// <summary>
	/// ダメージ処理関数
	/// </summary>
	/// <param name="translate">衝突したやつの座標</param>
	void HitDamage(const Vector3& translate);

public: // アクセッサ等

	/// <summary>
	/// ゲームマネージャーセッター
	/// </summary>
	/// <param name="manager">ゲームマネージャー本体</param>
	void SetGameManager(GameManager* manager) { gameManager_ = manager; }
	/// <summary>
	/// ゲームマネージャーゲッター
	/// </summary>
	/// <returns>ゲームマネージャー本体</returns>
	GameManager* GetGameManager() { return gameManager_; }

	/// <summary>
	/// 敵セッター
	/// </summary>
	/// <param name="enemy">敵本体</param>
	void SetEnemy(Enemy* enemy) { enemy_ = enemy; }
	/// <summary>
	/// 敵ゲッター
	/// </summary>
	/// <returns>敵本体</returns>
	Enemy* GetEnemy() { return enemy_; }

	/// <summary>
	/// 追従カメラセッター
	/// </summary>
	/// <param name="camera">追従カメラ</param>
	void SetFollowCamera(FollowCamera* camera) { followCamera_ = camera; }
	/// <summary>
	/// 追従カメラゲッター
	/// </summary>
	/// <returns>追従カメラ</returns>
	FollowCamera* GetFollowCamera() { return followCamera_; }

	/// <summary>
	/// 目標角度セッター
	/// </summary>
	/// <param name="angle">目標角度</param>
	void SetTargetAngle(const float angle) { targetAngle_ = angle; }

	/// <summary>
	/// 体力ゲッター
	/// </summary>
	/// <returns>プレイヤー体力</returns>
	int32_t GetHP() { return hp_; }

	/// <summary>
	/// コンボマネージャーゲッター
	/// </summary>
	/// <returns>コンボマネージャー</returns>
	ComboManager* GetComboManager() { return &comboManager_; }

	/// <summary>
	/// 攻撃状態セッター
	/// </summary>
	/// <param name="isAttacking">攻撃状態</param>
	void SetIsAttacking(const bool isAttacking) { isAttacking_ = isAttacking; }
	/// <summary>
	/// 攻撃状態ゲッター
	/// </summary>
	/// <returns>攻撃状態</returns>
	bool GetIsAttacking() { return isAttacking_; }

	/// <summary>
	/// 剣の当たり判定ゲッター
	/// </summary>
	/// <returns>剣の当たり判定</returns>
	Line* GetSwordLine() { return SwordLine_.get(); }

	/// <summary>
	/// 行動可能状態セッター
	/// </summary>
	/// <param name="canAction">行動可能状態</param>
	void SetCanAction(const bool canAction) { canAction_ = canAction; }

	/// <summary>
	/// 死亡状態セッター
	/// </summary>
	/// <param name="isDead">死亡しているか</param>
	void SetIsDead(const bool isDead) { isDead_ = isDead; }
	/// <summary>
	/// 死亡状態ゲッター
	/// </summary>
	/// <returns>死亡しているか</returns>
	bool GetIsDead() { return isDead_; }

	/// <summary>
	/// 身体の中心座標ゲッター
	/// </summary>
	/// <returns>身体の中心座標</returns>
	WorldTransform* GetBodyPos() { return &colliderTransform_; }

public: // パブリックなメンバ変数

	// 効果音群
	uint32_t SwingSword_ = 0u; // 素振り音
	uint32_t RotateSlash_ = 0u; // 回転切り音

private: // メンバ変数

	// 入力検知用
	Input* input_ = nullptr;

	// ゲームマネージャー
	GameManager* gameManager_ = nullptr;
	// 敵
	Enemy* enemy_ = nullptr;
	// 追従カメラ格納用
	FollowCamera* followCamera_ = nullptr;

	// 開始前のセットアップフラグ
	bool isSetUp_ = false;

	// 行動可能か
	bool canAction_ = false;

	// コンボマネージャー
	ComboManager comboManager_;

	// 武器のトランスフォーム
	WorldTransform weaponTransform_;
	// 攻撃判定用線
	std::unique_ptr <Line> SwordLine_;
	// 攻撃可能か
	bool canAttack_ = true;
	// 攻撃中か
	bool isAttacking_ = false;

	// 最大HP
	static const int32_t maxHP_ = 6;
	// HP
	int32_t hp_ = maxHP_;
	// 死亡トリガー
	bool isDead_ = false;

	// 体力UI
	Vector2 hertUITranslate_[maxHP_]; // 座標
	Vector2 hertUISize_[maxHP_];	  // サイズ

	// ヒットクールタイムタイマー
	KLib::DeltaTimer hitCoolTimeTimer_;
	// ヒットクールタイム定数値
	const float kHitCoolTime_ = 0.35f;

	// 追従対象の目標角度
	float targetAngle_ = 0.0f;

	// コライダー座標
	WorldTransform colliderTransform_;
	// コライダーのワールド座標
	Vector3 colliderWorldPos_;
	// コライダー半径
	float colliderRadius_ = 0.25f;

	// 行動状態格納変数
	std::unique_ptr<IState> state_;
};
