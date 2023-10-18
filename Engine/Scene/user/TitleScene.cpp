#include "TitleScene.h"
#include "../SceneManager.h"

void TitleScene::Initialize()
{
	// カメラ生成
	Camera* camera = new Camera(); // インスタンス生成
	camera->Initialize("camera", BaseObject::tagCamera); // 初期化
	camera->UseThisCamera(); // 使用カメラに設定
	gameObjectManager_->AddGameObject(camera); // ゲームオブジェクトマネージャーに追加

	// 環境生成
	Ambient* ambient = new Ambient(); // インスタンス生成
	ambient->Initialize("ambient", BaseObject::tagOther); // 初期化
	gameObjectManager_->AddGameObject(ambient); // ゲームオブジェクトマネージャーに追加

	// タイトルマネージャーオブジェクト生成
	titleManager_ = new TitleManagerObject(); // インスタンス生成
	titleManager_->SetCamera(camera);
	titleManager_->Initialize("titleManagerObject", BaseObject::tagOther); // 初期化
	gameObjectManager_->AddGameObject(titleManager_); // ゲームオブジェクトマネージャーに追加
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
	// タイトルからゲームシーンへの遷移を行うよう命令されたら遷移する
	if (input_->TriggerKey(DIK_RSHIFT) || titleManager_->GetIsGoGameScene()) {
		BaseScene* nextScene = new GameScene();
		SceneManager::GetInstance()->SetNextScene(nextScene);
	}
}
