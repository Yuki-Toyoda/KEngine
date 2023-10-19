#include "StageSelectManagerObject.h"

void StageSelectManagerObject::Initialize(std::string name, Tag tag)
{
	// 基底クラス初期化
	BaseObject::Initialize(name, tag);
	isActive_ = true;

	// 初期座標を設定
	transform_.translate_ = { 0.0f, -5.0f, 0.0f };

	// 全てのステージプレビュー座標を初期化
	for (int i = 0; i < 5; i++)
		previewStageTransforms_[i].Initialize();
	// ステージプレビュー座標を個々で設定
	previewStageTransforms_[0].translate_ = { 0.0f, 0.0f, 0.0f };
	previewStageTransforms_[1].translate_ = { 2.35f, -2.35f, 0.0f };
	previewStageTransforms_[2].translate_ = { 2.35f, -4.7f, 0.0f };
	previewStageTransforms_[3].translate_ = { -2.35f, -2.35f, 0.0f };
	previewStageTransforms_[4].translate_ = { -2.35f, -4.7f, 0.0f };

	// モデル読み込み
	for(int i = 0; i < 5; i++)
		AddOBJ(&previewStageTransforms_[i], color_, "./Resources/Gear", "Gear_L.obj", false);
}

void StageSelectManagerObject::Update()
{

	// 基底クラス更新
	BaseObject::Update();

#ifdef _DEBUG
	// Imgui
	ImGui::Begin(objectName_.c_str());

	for (int i = 0; i < 5; i++) {
		std::string name = "previewTransform" + i;
			if (ImGui::TreeNode(name.c_str())) {
			ImGui::DragFloat3("scale", &previewStageTransforms_[i].scale_.x, 0.5f);
			ImGui::DragFloat3("rotatate", &previewStageTransforms_[i].rotate_.x, 0.05f);
			ImGui::DragFloat3("translate", &previewStageTransforms_[i].translate_.x, 0.5f);
			ImGui::TreePop();
		}
	}

#endif // _DEBUG


}

void StageSelectManagerObject::Draw()
{
	// 全てのobjを描画
	DrawAllOBJ();
}

void StageSelectManagerObject::SpriteDraw()
{

}

void StageSelectManagerObject::AddGlobalVariables()
{

}

void StageSelectManagerObject::ApplyGlobalVariables()
{

}
