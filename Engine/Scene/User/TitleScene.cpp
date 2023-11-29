#include "TitleScene.h"
#include "../SceneManager.h"

void TitleScene::Initialize()
{
	SampleBox* box = nullptr;
	box = gameObjectManager_->CreateInstance<SampleBox>("box", BaseObject::TagNone);
}

void TitleScene::Update()
{
	Vector3 from0 = Math::Normalize(Vector3{ 1.0f, 0.7f, 0.5f });
	Vector3 to0 = { from0.x * -1.0f, from0.y * -1.0f, from0.z * -1.0f };
	Vector3 from1 = Math::Normalize(Vector3{ -0.6f, 0.9f, 0.2f });
	Vector3 to1 = Math::Normalize(Vector3{ 0.4f, 0.7f, -0.5f });
	Matrix4x4 rotateMatrix0 = Math::DirectionToDirection(
		Math::Normalize(Vector3{ 1.0f, 0.0f, 0.0f }), Math::Normalize(Vector3{ -1.0f, 0.0f, 0.0f }));
	Matrix4x4 rotateMatrix1 = Math::DirectionToDirection(from0, to0);
	Matrix4x4 rotateMatrix2 = Math::DirectionToDirection(from1, to1);

	ImGui::Begin("Answer");
	Math::MatrixImGui(rotateMatrix0, "rotateMatrix0");
	Math::MatrixImGui(rotateMatrix1, "rotateMatrix1");
	Math::MatrixImGui(rotateMatrix2, "rotateMatrix2");
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
