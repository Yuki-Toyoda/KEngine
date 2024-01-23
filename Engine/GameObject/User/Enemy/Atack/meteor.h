#pragma once
#include "../../../BaseObject.h"
#include "../../../GameObjectManager.h"
#include "../../Rubble/Rubble.h"
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

private:
	void OnCollisionEnter(Collider* collider)override;
private: // メンバ変数

	// 移動方向ベクトル
	Vector3 velocity_;
	// 移動スピード
	float moveAcceleration_ = 0.1f;
	// 当たり判定用ワールド座標
	Vector3 worldPos_;
	GameObjectManager* gameObjectmanager_=nullptr;
};

