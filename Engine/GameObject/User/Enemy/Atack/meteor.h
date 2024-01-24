#pragma once
#include "../../../BaseObject.h"
#include "../../Rubble/Rubble.h"
#include "../../GameManager/GameManager.h"
class Meteor:public BaseObject
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
	void SetgameManager(GameManager* gamemanager) { gameManager_ = gamemanager; }
private:
	void OnCollisionEnter(Collider* collider)override;
private: // メンバ変数

	// 移動方向ベクトル
	Vector3 velocity_;
	// 移動スピード
	float moveSpeed_ = 0.1f;
	// 当たり判定用ワールド座標
	Vector3 worldPos_;
	
	// 攻撃範囲開示用トランスフォーム
	WorldTransform attackAreaTransform_;
	// 攻撃範囲色
	Vector4 areaColor_;

	GameManager* gameManager_;
	
};

