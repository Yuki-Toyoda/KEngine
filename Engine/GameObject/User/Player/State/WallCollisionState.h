#pragma once
#include "IPlayerState.h"
#include "../../../../Utility/KLib.h"

class WallCollisionState : public IPlayerState
{
	void Init() override;

	void Update() override;

	void DisplayImGui() override;

private:
	// 開始座標
	Vector3 startPosition;
	// 終了座標（修正座標）
	Vector3 correctPosition;
	// 
	KLib::DeltaTimer lerpTimer_;

};

