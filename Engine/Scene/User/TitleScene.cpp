#include "TitleScene.h"
#include "../SceneManager.h"

void TitleScene::Initialize()
{
	SampleBox* box = nullptr;
	box = gameObjectManager_->CreateInstance<SampleBox>("box", BaseObject::TagNone);
}

void TitleScene::Update()
{

	Quaternion rotation = Math::MakeRotateAxisAngleQuaternion(
		Math::Normalize(Vector3{ 1.0f, 0.4f, -0.2f }), 0.45f);
	Vector3 pointY = { 2.1f, -0.9f, 1.3f };
	Matrix4x4 rotateMatrix = Math::QuaternionToMatrix(rotation);
	Vector3 rotateByQuaternion = Math::RoatateVector(pointY, rotation);
	Vector3 rotateByMatrix = Math::Transform(pointY, rotateMatrix);

	ImGui::Begin("answer");
	ImGui::DragFloat4("rotation", &rotation.x, 0.0f);
	Math::MatrixImGui(rotateMatrix, "rotateMatrix");
	ImGui::DragFloat3("rotateByQuaternion", &rotateByQuaternion.x, 0.0f);
	ImGui::DragFloat3("rotateByMatrix", &rotateByMatrix.x, 0.0f);
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
