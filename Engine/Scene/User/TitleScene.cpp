#include "TitleScene.h"
#include "../SceneManager.h"

void TitleScene::Initialize()
{
	SampleBox* box = nullptr;
	box = gameObjectManager_->CreateInstance<SampleBox>("box", BaseObject::TagNone);
}

void TitleScene::Update()
{

	Quaternion q1 = { 2.0f, 3.0f, 4.0f, 1.0f };
	Quaternion q2 = { 1.0f, 3.0f, 5.0f, 2.0f };
	Quaternion identity = Math::MakeIdentityQuaternion();
	Quaternion conj = Math::Conjugate(q1);
	Quaternion inv = Math::Inverse(q1);
	Quaternion normal = Math::Normalize(q1);
	Quaternion mul1 = q1 * q2;
	Quaternion mul2 = q2 * q1;
	float norm = Math::Length(q1);

	ImGui::Begin("answer");
	ImGui::DragFloat4("Identity", &identity.x, 0.0f);
	ImGui::DragFloat4("conj", &conj.x, 0.0f);
	ImGui::DragFloat4("inv", &inv.x, 0.0f);
	ImGui::DragFloat4("normal", &normal.x, 0.0f);
	ImGui::DragFloat4("mul1", &mul1.x, 0.0f);
	ImGui::DragFloat4("mul2", &mul2.x, 0.0f);
	ImGui::DragFloat("norm", &norm, 0.0f);
	ImGui::End();

	// デバッグ時のみ特定のキーでシーン遷移
#ifdef _DEBUG
	// デバッグ遷移
	if (input_->TriggerKey(DIK_RSHIFT)) {
		BaseScene* nextScene = new GameScene();
		SceneManager::GetInstance()->SetNextScene(nextScene);
	}
#endif // _DEBUG

}
