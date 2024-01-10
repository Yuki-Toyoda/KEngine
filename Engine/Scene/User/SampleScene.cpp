#include "SampleScene.h"
#include "../../GameObject/SampleList.h"

void SampleScene::Init()
{
	// オブジェクトの生成
	gameObjectManager_->CreateInstance<TestObject>("Test", BaseObject::TagNone);
}

void SampleScene::Update()
{
	Quaternion rotation0 = Math::MakeRotateAxisAngleQuaternion({ 0.71f, 0.71f, 0.0f }, 0.3f);
	Quaternion rotation1 = Math::MakeRotateAxisAngleQuaternion({ 0.71f, 0.0f, 0.71f }, 3.141592f);
	Quaternion interpolate = Math::Slerp(0.0f, rotation0, rotation1);
	Quaternion interpolate1 = Math::Slerp(0.3f, rotation0, rotation1);
	Quaternion interpolate2 = Math::Slerp(0.5f, rotation0, rotation1);
	Quaternion interpolate3 = Math::Slerp(0.7f, rotation0, rotation1);
	Quaternion interpolate4 = Math::Slerp(1.0f, rotation0, rotation1);

	ImGui::Begin("Answer");
	ImGui::DragFloat4("interpolate0", &interpolate.x);
	ImGui::DragFloat4("interpolate1", &interpolate1.x);
	ImGui::DragFloat4("interpolate2", &interpolate2.x);
	ImGui::DragFloat4("interpolate3", &interpolate3.x);
	ImGui::DragFloat4("interpolate4", &interpolate4.x);
	ImGui::End();
}
