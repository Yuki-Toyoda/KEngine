#include "SampleBox.h"

void SampleBox::Initialize() {

	// メッシュを追加
	AddMesh(&transform_, color_, "./Engine/Resource/Samples/Box", "Box.obj");

}

void SampleBox::Update() {

	transform_.rotate_.y += 0.01f;

}

void SampleBox::DisplayImGui() {
	// ワールド座標の表示
	transform_.DisplayImGui();
	// テキスト表示
	ImGui::Text("Color");
	//　色の変更
	ImGui::ColorPicker4("Color", &color_.x);

	for (Mesh* mesh : meshes_) {
		mesh->DisplayImGui();
		ImGui::Checkbox("isUI", &mesh->isUI_);
	}
}