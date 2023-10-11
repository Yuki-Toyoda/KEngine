#include "TPCamera.h"

void TPCamera::Initialize(std::string name, Tag tag)
{
	// 基底クラス初期化
	BaseObject::Initialize(name, tag);

	// 入力取得
	input_ = Input::GetInstance();
	// コントローラー入力取得
	input_->GetJoystickState(0, joyState_); // 現在フレームの入力取得
	preJoyState_ = joyState_; // 前フレームの入力取得

	// カメラ初期位置の設定
	transform_.translate_ = { 0.0f, 1.0f, -10.0f };

	// グローバル変数に調整したい値を追加
	AddGlobalVariables();
}

void TPCamera::Update()
{

	// 基底クラス更新
	BaseObject::Update();

	// 入力取得
	preJoyState_ = joyState_; // 前フレームの入力取得
	input_->GetJoystickState(0, joyState_); // 現在フレームの入力取得

	// 旋回速度
	const float speed = 0.05f;

	// スティックの横移動に対してカメラを旋回
	transform_.rotate_.y += (float)joyState_.Gamepad.sThumbRX / SHRT_MAX * speed;

	// 追従座標更新
	UpdateTarget();

#ifdef _DEBUG

	// デバックトリガーがTrueの時
	if (isDebug_) {
		// Wキーが押されたとき
		if (input_->PushKey(DIK_W))
			transform_.translate_.z += 0.35f;
		// Sキーが押されたとき
		if (input_->PushKey(DIK_S))
			transform_.translate_.z -= 0.35f;
		// Aキーが押されたとき
		if (input_->PushKey(DIK_D))
			transform_.translate_.x += 0.35f;
		// Dキーが押されたとき
		if (input_->PushKey(DIK_A))
			transform_.translate_.x -= 0.35f;

		// スペースキーが押されたとき
		if (input_->PushKey(DIK_SPACE))
			transform_.translate_.y += 0.15f;
		// 左コントロールが押されたとき
		if (input_->PushKey(DIK_LCONTROL))
			transform_.translate_.y -= 0.15f;

		// 上キーが押されたとき
		if (input_->PushKey(DIK_UP))
			transform_.rotate_.x -= 0.01f;
		// 下キーが押されたとき
		if (input_->PushKey(DIK_DOWN))
			transform_.rotate_.x += 0.01f;
		// 右キーが押されたとき
		if (input_->PushKey(DIK_RIGHT))
			transform_.rotate_.y += 0.01f;
		// 左キーが押されたとき
		if (input_->PushKey(DIK_LEFT))
			transform_.rotate_.y -= 0.01f;

	}

	ImGui::Begin(objectName_.c_str());
	if (ImGui::Button("UseThisCamera"))
		UseThisCamera();
	ImGui::End();

#endif // _DEBUG
}

void TPCamera::Draw()
{
	// カメラオブジェクトの描画
	for (OBJ* obj : objects_) {
		if (isActive_) {
			obj->Draw();
		}
	}
}

void TPCamera::UseThisCamera()
{
	// OBJクラスにビュープロジェクション行列をセット
	OBJ::SetViewProjection(&viewProjectionMatrix_);
}

void TPCamera::AddGlobalVariables()
{

}

void TPCamera::ApplyGlobalVariables()
{

}

void TPCamera::UpdateTarget()
{
	// 追従対象が存在すれば
	if (target_) {
		// 追従対象からカメラまでのオフセットを設定
		Vector3 offset = { 0.0f, 2.0f, -10.0f };

		// カメラの角度から回転行列を生成
		Matrix4x4 rotateMat = Math::MakeRotateXYZMatrix(transform_.rotate_);

		// オフセットをカメラの回転に合わせて回転させる
		offset = Math::Transform(offset, rotateMat);
		Vector3 parentPosition = target_->GetWorldPos();

		// 座標コピーからオフセット分ずらす
		transform_.translate_ = parentPosition + offset;
	}

	// カメラのワールド行列の取得
	Matrix4x4 cameraMatrix = transform_.GetMatWorld();
	// ビュー行列計算
	Matrix4x4 viewMatrix = Math::Inverse(cameraMatrix);
	// プロジェクション行列の計算
	Matrix4x4 projectionMatrix = Math::MakePerspectiveFovMatrix(0.45f, float(WinApp::kWindowWidth) / float(WinApp::kwindowHeight), 0.1f, 100.0f);
	// ビュープロジェクション行列の計算
	viewProjectionMatrix_ = viewMatrix * projectionMatrix;
}
