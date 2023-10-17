#include "TitleScene.h"
#include "../SceneManager.h"

void TitleScene::Initialize()
{

	Camera* camera = new Camera();
	camera->Initialize("camera", BaseObject::tagCamera);
	camera->UseThisCamera();
	gameObjectManager_->AddGameObject(camera); // ゲームオブジェクトマネージャーに追加

	// タイトルマネージャーオブジェクト生成生成
	TitleManagerObject* titleManager = new TitleManagerObject(); // インスタンス生成
	titleManager->SetCamera(camera);
	titleManager->Initialize("titleManagerObject", BaseObject::tagOther); // 初期化
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
		BaseScene* nextScene = new GameScene();
		SceneManager::GetInstance()->SetNextScene(nextScene);
	}
}
