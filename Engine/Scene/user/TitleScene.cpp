#include "TitleScene.h"

void TitleScene::Initialize()
{

	Camera* camera = new Camera();
	camera->Initialize("camera", BaseObject::tagCamera);
	camera->transform_.translate_ = {0.0f, 0.25f, -16.0f};
	camera->UseThisCamera();
	gameObjectManager_->AddGameObject(camera); // ゲームオブジェクトマネージャーに追加

	// タイトルマネージャーオブジェクト生成生成
	TitleManagerObject* titleManager = new TitleManagerObject(); // インスタンス生成
	titleManager->Initialize("titleManagerObject", BaseObject::tagOther); // 初期化
	titleManager->SetCamera(camera);
	gameObjectManager_->AddGameObject(titleManager); // ゲームオブジェクトマネージャーに追加
}

void TitleScene::Update()
{

#ifdef _DEBUG

	ImGui::Begin("Light");
	// ライトデバック
	if (ImGui::TreeNode("Light")) {

		ImGui::Checkbox("lightActive", &enableLighting_);
		ImGui::DragFloat3("Direction", &setRotation_.x, 0.05f, -1.0f, 1.0f);
		ImGui::ColorEdit4("color", &setLightColor.x);
		ImGui::DragFloat("Intensity", &setIntensity_, 0.05f, 0.0f, 5.0f);

		// ライト設定
		OBJ::SetLightActive(0, enableLighting_);
		OBJ::SetLightColor(0, setLightColor);
		OBJ::SetLightRotation(0, setRotation_);
		OBJ::SetLightIntensity(0, setIntensity_);

		ImGui::TreePop();
	}
	ImGui::End();

#endif // _DEBUG


	// 右シフトが押されたら次のシーンへ
	if (input_->TriggerKey(DIK_RSHIFT)) {
		nextScene_ = new GameScene();
		gameObjectManager_->Initialize();
	}
}
