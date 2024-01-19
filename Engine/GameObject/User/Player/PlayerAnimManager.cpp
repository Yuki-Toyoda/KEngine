#include "PlayerAnimManager.h"
#include "Player.h"

void PlayerAnimManager::Init()
{
	// 各トランスフォームの初期化
	bodyTransform_.Init();
	bodyTransform_.SetParent(&transform_);

	// メッシュ追加
#ifdef _DEBUG

	// デバッグ時はキューブ(簡易モデル)で描画
	//AddMesh(&transform_, color_, "./Engine/Resource/Samples/Box", "Box.obj");
	// 身体
	AddMesh(&bodyTransform_, color_, "./Resources/Player", "Player.obj");


#endif // _DEBUG

#ifndef _DEBUG

	// 身体
	AddMesh(&bodyTransform_, color_, "./Resources/Player", "Player.obj");

#endif // _RELEASE

}

void PlayerAnimManager::Update()
{

}

void PlayerAnimManager::DisplayImGui()
{
	// 表示状態の切り替え
	ImGui::Checkbox("isActive", &isActive_);

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
