#include "Level.h"

void Level::Init()
{
	// 基底クラスの初期化をよび出す
	SampleLevelObjects::Init();

	// 球形コライダーの追加
	AddColliderSphere("Level", &colliderPosition_, &colliderRadius_);
}

void Level::Update()
{
	// 基底クラスの更新を呼び出す
	SampleLevelObjects::Update();

	// コライダーの座標にワールド座標を設定
	colliderPosition_ = transform_.GetWorldPos();
}

void Level::DisplayImGui()
{
	// 基底クラスの更新を呼び出す
	SampleLevelObjects::DisplayImGui();
}
