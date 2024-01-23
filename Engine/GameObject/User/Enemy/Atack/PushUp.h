#pragma once
#include "../../../baseObject.h"
#include "../../../../Utility/Timer/DeltaTimer.h"
#include "../../../../Utility/Lerp/lerp.h"
class PushUp :public BaseObject
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

	void Reset();
	//攻撃準備
	void PreparationAtack();
	//攻撃
	void Atack();
	//攻撃の判定の残る時間
	void AtackRemain();
	//攻撃終了後
	void AtackEnd();
	void SetActive();
private:
	Vector3 velocity_;
	//攻撃しているかどうか
	bool isAtack_;
	//攻撃前の回転をしているか
	bool isRotate_;
	//攻撃終了
	bool isAtackEnd_;
	//判定が残ってるかどうか
	bool isRemain_;
	//タイマー
	KLib::DeltaTimer timer_;
	//攻撃前の回転にかかる時間
	float RotateTime_=2.0f;
	//攻撃の時間
	float AtackTime_=1.0f;
	//判定の残る時間
	float remainTime_=4.0f;
	//下がったときの座標
	float rawPosition_;
	//上がったときの座標
	float upPosition_;
	//通常の高さ
	float defaultPosition_;
};

