#include "SampleBox.h"

void SampleBox::Initialize() {

	// メッシュを追加
	AddMesh(&transform_, color_, "./Engine/Resource/Samples/cube", "cube.obj");

}

void SampleBox::Update() {

	

}

void SampleBox::DisplayImGui() {
	// ワールド座標の表示
	transform_.DisplayImGui();
}