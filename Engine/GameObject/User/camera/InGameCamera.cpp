#include "InGameCamera.h"
#include <vector>
#include "../../../Base/WinApp.h"
#include "../../../Base/DirectXCommon.h"
#include "../../GameObjectManager.h"
#include "../Random/RandomEngine.h"
#include"../../../GlobalVariables/GlobalVariables.h"
#include "../../User/Player/Player.h"
#include "../InputManager/InputManager.h"

void InGameCamera::Init()
{
	// 基底クラスの初期化を呼び出す
	Camera::Init();

	// 調整項目クラスのインスタンス取得
	GlobalVariables* variables = GlobalVariables::GetInstance();
	// 調整項目クラスのグループ生成
	variables->CreateGroup(name_);

	// 追加
	variables->AddItem(name_, "Position", transform_.translate_);
	variables->AddItem(name_, "Rotate", transform_.rotate_);
	variables->AddItem(name_, "Fov", fov_);

	// 取得
	//transform_.translate_ = variables->GetVector3Value(name_, "Position");
	transform_.rotate_.x = 0.825f;
	fov_ = variables->GetFloatValue(name_, "Fov");
}

void InGameCamera::Update()
{
	// 振動タイマー終了が終了していない時
	if (!shakeTimer_.GetIsFinish()) {
		// カメラの振動オフセットを軸ごとにランダムに設定する
		shakeOffset_.x = Math::RandomF(-shakeStrength_.x, shakeStrength_.x, 2);
		shakeOffset_.y = Math::RandomF(-shakeStrength_.y, shakeStrength_.y, 2);
		// 振動強さを補間で下げる
		shakeStrength_.x = KLib::Lerp<float>(startShakeStrength_.x, 0.0f, KLib::EaseOutQuad(shakeTimer_.GetProgress()));
		shakeStrength_.y = KLib::Lerp<float>(startShakeStrength_.y, 0.0f, KLib::EaseOutQuad(shakeTimer_.GetProgress()));

		// 振動タイマー更新
		shakeTimer_.Update();
	}
	else {
		// オフセットを0でリセット
		shakeOffset_ = { 0.0f, 0.0f, 0.0f };
	}

	// オフセットを取得
	Vector3 offset = offset_;

	// プレイヤーがセットされているときのみ
	if (player_ != nullptr) {
		/// プレイヤーの座標によってカメラのオフセットを移動させる
		// x軸
		if (player_->transform_.translate_.x > 0.0f) {
			offset.x = Math::Linear(player_->transform_.translate_.x, 0.0f, 5.0f, 53.0f);
		}
		else if (player_->transform_.translate_.x < 0.0f) {
			offset.x = Math::Linear(player_->transform_.translate_.x, 0.0f, -5.0f, -53.0f);
		}
		// z軸
		if (player_->transform_.translate_.z > 0.0f) {
			offset.z = Math::Linear(player_->transform_.translate_.z, offset_.z, offset_.z + 5.0f, 53.0f);
		}
		else if (player_->transform_.translate_.z < 0.0f) {
			offset.z = Math::Linear(player_->transform_.translate_.z, offset_.z, offset_.z - 5.0f, -53.0f);
		}
	}
	// 回転角から行列を生成
	Matrix4x4 rotateMat = Math::MakeRotateXYZMatrix(transform_.rotate_);
	// オフセットをカメラの回転に合わせて回転させる
	offset = Math::Transform(offset, rotateMat);

	// 回転可能か
	if (canRotate_) {
		// 右スティックの右方向の入力があった場合
		if (InputManager::GetRTInput() >= 155.0f) {
			// 目標角度を設定
			targetAngleY_ = -(float)std::numbers::pi / 2.0f;
		}// 右スティックの左方向の入力があった場合
		else if (InputManager::GetLTInput() >= 155.0f) {
			// 目標角度を設定
			targetAngleY_ = (float)std::numbers::pi / 2.0f;
		}
		else {
			// 目標角度をリセット
			targetAngleY_ = 0.0f;
		}
	}

	// 目標角度に徐々に補間する
	transform_.rotate_.y = Math::LerpShortAngle(transform_.rotate_.y, targetAngleY_, 0.1f);
	

	// オフセットをもとにカメラ座標を計算する
	transform_.translate_ = Vector3(0.0f, 2.0f, 0.0f) + offset;

	if (isUseThisCamera_) {
		// ビュープロジェクション行列の計算
		Matrix4x4 cameraMatrix = transform_.GetMatWorld(); // ワールド行列の生成
		viewMatrix_ = Math::Inverse(cameraMatrix); // ビュー表列の生成
		float far_z = 200.0f;
		Matrix4x4 projectionMatrix = Math::MakePerspectiveFovMatrix(fov_, float(WinApp::kWindowWidth) / float(WinApp::kwindowHeight), 0.1f, far_z); // プロジェクション行列の生成
		viewProjectionMatrix_ = viewMatrix_ * projectionMatrix; // 実際に計算

		// アドレスに
		*vpDataTarget_ = viewProjectionMatrix_;
	}

}

void InGameCamera::DisplayImGui()
{
	// ワールド座標の表示
	transform_.DisplayImGui();
	// 視野角の調整
	ImGui::SliderFloat("FOV", &fov_, 0.45f, 1.2f);

	// オフセットのImGuiを表示
	ImGui::DragFloat3("Offset", &offset_.x);

	// テストの振動値の強さを設定
	ImGui::DragFloat2("ShakeStrength", &imGuiShakeStrength_.x, 0.1f, 0.0f);
	// テスト用の振動時間の調整
	ImGui::DragFloat("ShakeTime", &imGuiShakeTime_, 0.01f, 0.01f);
	// ボタンを押したら振動演出開始
	if (ImGui::Button("ShakeStart")) {
		Shake(imGuiShakeTime_, imGuiShakeStrength_);
	}

	if (ImGui::Button("Save")) {
		SaveData();
	}

	// ボタンを押すとこのカメラを使用する
	if (!isUseThisCamera_) {
		if (ImGui::Button("UseThisCamera"))
			UseThisCamera();
	}
	else {
		ImGui::Text("This Camera Is Used");
	}
}

void InGameCamera::Shake(const float& time, const Vector2& strength)
{
	// 引数で指定した秒数で振動タイマー開始
	shakeTimer_.Start(time);

	// 振動強さ取得
	startShakeStrength_ = strength;
}

void InGameCamera::SaveData()
{
	// 調整項目クラスのインスタンス取得
	GlobalVariables* variables = GlobalVariables::GetInstance();
		
	// 値の設定
	variables->SetValue(name_, "Position", transform_.translate_);
	variables->SetValue(name_, "Rotate", transform_.rotate_);
	variables->SetValue(name_, "Fov", fov_);

	// ファイル保存
	variables->SaveFile(name_);

}
