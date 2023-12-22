#pragma once
#include "../../BaseObject.h"
#include "../InputManager/InputManager.h"
class Wepon:public BaseObject
{
public:

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

public:
	//追従対象
	void SetTarget(const WorldTransform* target) { target_ = target; }

private:
	// 追従対象のワールド座標
	const WorldTransform* target_ = nullptr;
	//追従対象からの距離
	float distance_;
	//追従対象からの角度
	float theta_;
	//回転の向き　０なら右回り1なら左回り
	bool rotateDirection_;
	Vector3 worldPos_;
private://メンバ関数

	void Move();

};

