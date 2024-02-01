#pragma once
#include "IPlayerState.h"

class DashState : public IPlayerState
{
public:
	void Init() override;

	void Update() override;

	void DisplayImGui() override;

private:
	// 開始座標
	Vector3 velocity_ = {};
	Vector3 moveDirect_ = {};
	// 
	KLib::DeltaTimer dashTimer_;


};

