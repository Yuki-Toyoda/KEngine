#include "Camera.h"
#include <vector>
#include "../../Base/WinApp.h"
#include "../../Base/DirectXCommon.h"
#include "../GameObjectManager.h"

void Camera::Init()
{
	// 定数バッファ生成
	cameraDataBuffer_.Init(DirectXCommon::GetInstance()->GetDirectXDevice());
	// 

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
		Matrix4x4 cameraMatrix = transform_.GetMatWorld(); // ワールド行列の生成
		viewMatrix_ = Matrix4x4::MakeInverse(cameraMatrix); // ビュー表列の生成
		Matrix4x4 projectionMatrix = Matrix4x4::MakePerspectiveFov(fov_, float(WinApp::kWindowWidth) / float(WinApp::kwindowHeight), 0.1f, 100.0f); // プロジェクション行列の生成
		viewProjectionMatrix_ = viewMatrix_ * projectionMatrix; // 実際に計算

		// バッファのデータにビュープロジェクション行列をセット
		cameraDataBuffer_.data_->WorldViewProj = viewProjectionMatrix_;
	}

	// デバッグカメラだった場合はキー入力でカメラを移動させる
	if (name_ == "DebugCamera") {

		// 移動速度
		const float speed = 0.1f;
		// 移動ベクトル
		Vector3 move = Vector3();

		// Wキーが押されたら
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

	// ボタンを押すとこのカメラを使用する
	if (!isUseThisCamera_) {
		if (ImGui::Button("UseThisCamera"))
			UseThisCamera();
	}
	else {
		ImGui::Text("This Camera Is Used");
	}
}


void Camera::UseThisCamera() {
	// 配列
	std::vector<Camera*>cameraObjects = GameObjectManager::GetInstance()->GetGameObject<Camera>();
	// 全てのカメラの使用トリガーを一度false
	for (Camera* camera : cameraObjects)
		camera->SetIsUseThisCamera(false);

	// このカメラを使用する
	isUseThisCamera_ = true;

	// メインカメラをセット
	DirectXCommon::GetInstance()->SetMainCamera(this);

	// 使用中のカメラをゲームオブジェクトマネージャに登録する
	GameObjectManager::GetInstance()->SetUseCamera(this);
}
