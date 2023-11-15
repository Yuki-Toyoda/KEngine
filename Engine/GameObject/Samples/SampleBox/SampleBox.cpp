#include "SampleBox.h"

void SampleBox::Initialize() {

	// メッシュを追加
	AddMesh(&transform_, color_, "./Engine/Resource/Samples/Sphere", "Sphere.obj");

}

void SampleBox::Update() {

	

}

void SampleBox::DisplayImGui() {
	// ワールド座標の表示
	transform_.DisplayImGui();
	// テキスト表示
	ImGui::Text("Color");
	//　色の変更
	ImGui::ColorPicker4("Color", &color_.x);
}