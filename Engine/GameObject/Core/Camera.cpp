#include "Camera.h"
#include <vector>
#include "Engine/Base/WinApp.h"
#include "Engine/Base/DirectXCommon.h"
#include "Engine/Base/Renderer/RendererManager.h"
#include "Engine/GameObject/GameObjectManager.h"
#include "Engine/Base/DescriptorHeaps/HeapManager.h"
#include "Engine/Resource/Texture/Texture.h"

void Camera::Init()
{
	// 描画マネージャーの取得
	rendererManager_ = DirectXCommon::GetInstance()->GetRendererManager();

	// デバイスとヒープの取得
	DirectXDevice* device = DirectXCommon::GetInstance()->GetDirectXDevice(); // デバイス
	HeapManager* heaps    = DirectXCommon::GetInstance()->GetHeaps();		   // ヒープ

	// 定数バッファ生成
	cameraDataBuffer_.Init(device);
	// データに値を代入
	cameraDataBuffer_.data_->DrawMeshlets = isDrawMeshlets_;
	cameraDataBuffer_.data_->ViewProjection2D = Matrix4x4::MakeOrthGraphic(0.0f, 0.0f, (float)KEngine::Config::Window::KWindowWidth, (float)KEngine::Config::Window::KWindowHeight, 0.0f, 100.0f);
	cameraDataBuffer_.data_->ToonMapTex = TextureManager::Load("./Engine/Resource/Samples/Texture", "ToonMap.png").GetView();
	cameraDataBuffer_.data_->ToonMapTex -= KEngine::Config::Rendering::kMaxBuffer;

	// 各リソース初期化
	renderResource_.Init(device, heaps);  // レンダリングを行うリソース
	textureResource_.Init(device, heaps); // レンダリング語の結果を保存するリソース
	depthStencil_.Init(device, heaps);	  // DSVリソース

	// ポストプロセス初期化
	ppProcessor_.Init();

	// 入力取得
	input_ = Input::GetInstance();

	// カメラ初期位置設定
	transform_.translate_ = { 0.0f, 1.0f, -10.0f };

	// 視野角初期値の設定
	fov_ = 0.45f;

	// カメラ使用トリガーはFalse
	isUseThisCamera_ = false;

	// 行列に単位行列を書き込んでおく
	viewMatrix_ = Matrix4x4::kIdentity;

}

void Camera::Update()
{
	if (isUseThisCamera_) {
		// ビュープロジェクション行列の計算
		Matrix4x4 cameraMatrix = transform_.GetMatWorld();																																// ワールド行列の生成
		viewMatrix_ = Matrix4x4::MakeInverse(cameraMatrix);																																// ビュー表列の生成
		Matrix4x4 projectionMatrix = Matrix4x4::MakePerspectiveFov(fov_, float(KEngine::Config::Window::KWindowWidth) / float(KEngine::Config::Window::KWindowHeight), 0.1f, 100.0f);	// プロジェクション行列の生成
		viewProjectionMatrix_ = viewMatrix_ * projectionMatrix;																															// 実際に計算

		// ビルボード行列の計算
		Matrix4x4 backToFrontMatrix = Matrix4x4::MakeRotateY(std::numbers::pi_v<float>); // Y軸で π / 2 回転させる
		Matrix4x4 billboardMatrix	= backToFrontMatrix * cameraMatrix;					 // ビルボード行列の計算
		billboardMatrix.m[3][0]		= 0.0f;
		billboardMatrix.m[3][1]		= 0.0f;
		billboardMatrix.m[3][2]		= 0.0f;												 // 平行移動成分を削除する

		// バッファのデータに各データをセットする
		cameraDataBuffer_.data_->WorldViewProj	= viewProjectionMatrix_;
		cameraDataBuffer_.data_->Billboard		= billboardMatrix;
		cameraDataBuffer_.data_->WorldPosition	= transform_.GetWorldPos();
		cameraDataBuffer_.data_->DrawMeshlets	= isDrawMeshlets_;

		// ポストプロセスの更新
		ppProcessor_.Update();

		// ポストプロセスのアウトラインにプロジェクション行列の逆行列をセット
		ppProcessor_.outLine_.SetViewProjectionInverse(projectionMatrix.Inverse());
	}

	// デバッグカメラだった場合はキー入力でカメラを移動させる
	if (name_ == "DebugCamera") {

		// 移動速度
		const float speed = 0.1f;
		// 移動ベクトル
		Vector3 move = Vector3();

		// キー押下に寄る移動処理
		if (input_->PushKey(DIK_W)) {
			move.z = 1.0f;
		}
		else if (input_->PushKey(DIK_S)) {
			move.z = -1.0f;
		}

		if (input_->PushKey(DIK_A)) {
			move.x = -1.0f;
		}
		else if (input_->PushKey(DIK_D)) {
			move.x = 1.0f;
		}

		if (input_->PushKey(DIK_SPACE)) {
			move.y = 1.0f;
		}
		else if (input_->PushKey(DIK_LCONTROL)) {
			move.y = -1.0f;
		}

		if (input_->PushKey(DIK_UP)) {
			transform_.rotate_.x -= 0.015f;
		}
		else if (input_->PushKey(DIK_DOWN)) {
			transform_.rotate_.x += 0.015f;
		}

		if (input_->PushKey(DIK_LEFT)) {
			transform_.rotate_.y -= 0.015f;
		}
		else if (input_->PushKey(DIK_RIGHT)) {
			transform_.rotate_.y += 0.015f;
		}

		// 移動量を正規化、スピードを加算
		move = Vector3::Normalize(move) * speed;

		// カメラの角度から回転行列を生成
		Matrix4x4 rotateMat = Matrix4x4::MakeRotate(transform_.rotate_);
		// 移動ベクトルをカメラの角度に応じて回転させる
		move = move * rotateMat;

		// 移動
		transform_.translate_ = transform_.translate_ + move;
	}

}

void Camera::DisplayImGui()
{
	// デバッグカメラだった場合はキー入力でカメラを移動させる
	if (name_ == "DebugCamera") {
		ImGui::Text("Debug Movement");
		ImGui::Text("Move ... WASD");
		ImGui::Text("Look ... ArrowKey");
		ImGui::Text("Rise ... Space");
		ImGui::Text("Descent ... LCTRL");
	}

	// ワールド座標の表示
	transform_.DisplayImGui();
	// 視野角の調整
	ImGui::SliderFloat("FOV", &fov_, 0.45f, 1.2f);

	// メッシュレット描画フラグの切り替え
	ImGui::Checkbox("DrawMeshlets", &isDrawMeshlets_);

	// ボタンを押すとこのカメラを使用する
	if (!isUseThisCamera_) {
		if (ImGui::Button("UseThisCamera"))
			UseThisCamera();
	}
	else {
		ImGui::Text("This Camera Is Used");
	}

	// ImGuiを表示
	ppProcessor_.DisplayImGui();
}


Texture Camera::GetRenderTexture()
{
	// レンダリング後の結果をテクスチャとして返す
	return textureResource_;
}

void Camera::UseThisCamera() {
	// 配列
	std::vector<Camera*>cameraObjects = GameObjectManager::GetInstance()->GetGameObject<Camera>();
	// 全てのカメラの使用トリガーを一度false
	for (Camera* camera : cameraObjects)
		camera->SetIsUseThisCamera(false);

	// このカメラを使用する
	isUseThisCamera_ = true;

	// 使用中のカメラをゲームオブジェクトマネージャに登録する
	GameObjectManager::GetInstance()->SetUseCamera(this);
}
