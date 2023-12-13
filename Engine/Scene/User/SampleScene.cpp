#include "SampleScene.h"
#include "../../GameObject/SampleList.h"

void SampleScene::Init()
{
	// 3Dモデル描画テスト用オブジェクトの追加
	GameObjectManager::GetInstance()->CreateInstance<SampleBox>("SampleBox", BaseObject::TagNone);
	// スプライト描画テスト用オブジェクトの追加
	GameObjectManager::GetInstance()->CreateInstance<SampleSprite>("SampleSprite", BaseObject::TagNone);

	// ImGuiで用いる変数の初期化
	imGuiGenerateTransform_.Init();
}

void SampleScene::Update()
{
	if (ImGui::Button("GeneratePlane")) {
		GameObjectManager::GetInstance()->CreateInstance<SampleBox>("SampleBox", BaseObject::TagNone);
	}
}
