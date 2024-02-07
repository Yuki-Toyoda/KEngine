#pragma once
#include "../../../BaseObject.h"
#include "../../../../Audio/Audio.h"
#include "../../../../Utility/Animation/AnimationManager.h"

// クラスの前方宣言
class Camera;

/// <summary>
/// ウリ丸
/// </summary>
class Uribo :public BaseObject
{
public:
	void Init()override;
	void Update()override;
	void DisplayImGui()override;
	void OnCollision(Collider* collider)override;

	/// <summary>
	/// 演出用カメラ更新処理
	/// </summary>
	void CameraUpdate();

	/// <summary>
/// 回復関数
/// </summary>
	void Heal(int healPower);

/// <summary>
/// チュートリアル用の関数
/// </summary>
	void Tutrial();
	bool TutrialEnd();

	void Reset();
public://アクセッサ
/// <summary>
/// 体力のげった
	int GetHP() { return hitPoint_; }
/// </summary>

	/// <summary>
	/// HP最大値のゲッター
	/// </summary>
	/// <returns>HP最大値</returns>
	int GetDefaultHP() { return defaultHP_; }

	/// <summary>
	/// ボスの死亡状態トリガーセッター
	/// </summary>
	/// <param name="isBossDead">true or false</param>
	void SetIsBossDead(const bool& isBossDead) { isBossDead_ = isBossDead; }
	/// <summary>
	/// ボスの死亡状態トリガーゲッター
	/// </summary>
	/// <returns>死亡しているか</returns>
	bool GetIsBossDead() { return isBossDead_; }

	/// <summary>
	/// ウリボの死亡状態ゲッター
	/// </summary>
	/// <returns>ウリボの死亡状態</returns>
	bool GetIsDead() { return isDead_; }
/// <summary>
/// チュートリアル状態のセッター
/// </summary>
   void SetTutrial(bool flag) { isTutrial_ = flag; }

   bool GetIsCanHeal() { return IsCanHeal; }
   void SetTutrialStart(bool flag) { tutrialStart_ = flag; }
   void SetTutrialPlactice(bool flag) { tutrialPlactice_ = flag; }
private: // プライベートなメンバ関数

	/// <summary>
	/// 調整項目クラス
	/// </summary>
	void SetGlobalVariables();

	/// <summary>
	/// パラメータ生成関数
	/// </summary>
	/// <param name="name">生成するパラメータ名</param>
	void CreateParameter(const std::string& name);

public: // パブリックなメンバ変数

	// 身体のトランスフォーム
	WorldTransform bodyTransform_;

private: // メンバ変数

	// アニメーションマネージャー
	AnimationManager* animManager_ = nullptr;

	// アニメーション
	Animation* anim_ = nullptr;

	// 演出用カメラ
	Camera* stagingCamera_ = nullptr;
	// カメラオフセット
	Vector3 cameraOffset_ = { 0.0f, 0.0f, 0.0f };

	// 餌を与えられるエリアのトランスフォーム
	WorldTransform feedAreaTransform_;

	// エリア色
	Vector4 areaColor_ = { 1.0f, 1.0f, 1.0f, 0.75f };

	// 餌を与えられるエリアのメッシュ
	Mesh* feedAreaMesh_ = nullptr;

	//体力
	int hitPoint_;
	//開始時の体力
	int defaultHP_= 6000;
	//毎フレーム減らす体力
	int decrementHP = 1;
	Vector3 collisionScale_;

	// ボスの死亡トリガー
	bool isBossDead_ = false;

	// 餌を与えられる状態のトリガー
	bool canFeed_ = false;

	// フェード演出トリガー
	bool isFade_ = false;
	// 死亡状態トリガー
	bool isDead_ = false;
	//チュートリアルフラグ
	bool isTutrial_;
	//体力が減るまでの時間
	KLib::DeltaTimer timer_;
	//回復できるか
	bool IsCanHeal;

	bool tutrialStart_;
	bool tutrialPlactice_;
};

