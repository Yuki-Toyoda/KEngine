#include "InGameUIManager.h"
#include "../../../Resource/Texture/TextureManager.h"

void InGameUIManager::Init()
{
	// スプライトの追加
	AddSprite("PauseMenu", { 0.0f, 0.0f }, { 224.0f, 64.0f }, TextureManager::Load("./Resources/UI/InGame", "pauseBotton.png"));
	AddSprite("Slam", { 880.0f, 0.0f }, { 768.0f, 128.0f }, TextureManager::Load("./Resources/UI/InGame", "slam.png"));
	AddSprite("MoveUI_BG", { 1020.0f, 0.0f }, { 232.0f, 85.3f }, TextureManager::Load("./Resources/UI/InGame", "moveBack.png"));
	AddSprite("MoveUI_F", { 1022.0f, 0.0f }, { 232.0f, 85.3f }, TextureManager::Load("./Resources/UI/InGame", "moveFront.png"));

	// スプライトの描画範囲設定
	sprites_[0]->texSize_ = { 448.0f, 128.0f };
	sprites_[1]->texSize_ = { 768.0f, 128.0f };
}

void InGameUIManager::Update()
{

}

void InGameUIManager::DisplayImGui()
{
	// スプライト情報があれば
	if (sprites_.size() > 0) {
		ImGui::Text("Sprites");
		ImGui::BeginChild("Sprites", ImVec2(0, 300), ImGuiWindowFlags_NoTitleBar);
		for (Sprite* s : sprites_) {
			s->DisplayImGui();
		}
		ImGui::EndChild();
	}
}
