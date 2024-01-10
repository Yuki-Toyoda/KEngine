#include "PlayerAnimManager.h"
#include "Player.h"

void PlayerAnimManager::Init()
{
	// 各トランスフォームの初期化
	bodyTransform_.Init();
	bodyTransform_.SetParent(&transform_);
	// 左はね
	wingTransform_L_.Init();
	wingTransform_L_.SetParent(&bodyTransform_);
	wingTransform_L_.translate_ = { 0.15f, 0.0f, 0.95f };
	// 右はね
	wingTransform_R_.Init();
	wingTransform_R_.SetParent(&bodyTransform_);
	wingTransform_R_.translate_ = { -0.15f, 0.0f, 0.95f };
	// 左腕
	armTransform_L_.Init();
	armTransform_L_.SetParent(&bodyTransform_);
	armTransform_L_.translate_ = { 0.75f, 0.15f, 0.0f };
	armTransform_L_.rotate_.z = (float)std::numbers::pi / 4.0f;
	// 右腕
	armTransform_R_.Init();
	armTransform_R_.SetParent(&bodyTransform_);
	armTransform_R_.translate_ = { -0.75f, 0.15f, 0.0f };
	armTransform_R_.rotate_.z = -(float)std::numbers::pi / 4.0f;
	// 左足
	footTransform_L_.Init();
	footTransform_L_.SetParent(&bodyTransform_);
	footTransform_L_.translate_ = { 0.35f, -0.75f, 0.0f };
	// 右足
	footTransform_R_.Init();
	footTransform_R_.SetParent(&bodyTransform_);
	footTransform_R_.translate_ = { -0.35f, -0.75f, 0.0f };

	// メッシュ追加
	AddMesh(&bodyTransform_, color_, "./Resources/Player", "Body.obj");
	AddMesh(&wingTransform_L_, color_, "./Resources/Player", "Wing_L.obj");
	AddMesh(&wingTransform_R_, color_, "./Resources/Player", "Wing_R.obj");
	AddMesh(&armTransform_L_, color_, "./Resources/Player", "Arm_L.obj");
	AddMesh(&armTransform_R_, color_, "./Resources/Player", "Arm_R.obj");
	AddMesh(&footTransform_L_, color_, "./Resources/Player", "Foot_L.obj");
	AddMesh(&footTransform_R_, color_, "./Resources/Player", "Foot_R.obj");
}

void PlayerAnimManager::Update()
{

}

void PlayerAnimManager::DisplayImGui()
{
	// 各種パーツの情報表示
	bodyTransform_.DisplayImGuiWithTreeNode("body");
	wingTransform_L_.DisplayImGuiWithTreeNode("wing_L");
	wingTransform_R_.DisplayImGuiWithTreeNode("wing_R");
	armTransform_L_.DisplayImGuiWithTreeNode("arm_L");
	armTransform_R_.DisplayImGuiWithTreeNode("arm_R");
	footTransform_L_.DisplayImGuiWithTreeNode("foot_L");
	footTransform_R_.DisplayImGuiWithTreeNode("foot_R");
}

void PlayerAnimManager::SetPlayer(Player* player)
{
	// プレイヤーをセット
	player_ = player;
	// プレイヤーの座標に追従させる
	transform_.SetParent(&player_->transform_);
}
