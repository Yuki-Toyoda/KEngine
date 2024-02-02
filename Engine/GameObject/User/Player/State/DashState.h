#pragma once
#include "IPlayerState.h"

class DashState : public IPlayerState
{
public:
	void Init() override;

	void Update() override;

	void DisplayImGui() override;

private:
	enum MoveStep {
		kAccleratorStep,
		kDecelerationStep,
	};

private:
	// 開始座標
	Vector3 velocity_ = {};
	Vector3 moveDirect_ = {};

	Vector3 maxVelocity_ = {};
	// 
	KLib::DeltaTimer dashTimer_;

	// 挙動の状態
	int moveStep_ = kAccleratorStep;

	// 減速タイム
	float brakeTimer_;
	// 加速タイム
	float axelTimer_;
	// パワー
	float dashPower_ = 5.5f;

};

