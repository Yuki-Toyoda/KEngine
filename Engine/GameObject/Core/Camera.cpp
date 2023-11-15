#include "Camera.h"
#include "../../Base/WinApp.h"
#include "../../Base/DirectXCommon.h"

void Camera::Initialize()
{
	// カメラ初期位置設定
	transform_.translate_ = { 0.0f, 1.0f, -10.0f };

	// 視野角初期値の設定
	fov_ = 0.45f;
}

void Camera::Update()
{
	// ビュープロジェクション行列の計算
	Matrix4x4 cameraMatrix = transform_.GetMatWorld(); // ワールド行列の生成
	viewMatrix_ = Math::Inverse(cameraMatrix); // ビュー表列の生成
	Matrix4x4 projectionMatrix = Math::MakePerspectiveFovMatrix(fov_, float(WinApp::kWindowWidth) / float(WinApp::kwindowHeight), 0.1f, 100.0f); // プロジェクション行列の生成
	viewProjectionMatrix_ = viewMatrix_ * projectionMatrix; // 実際に計算

	*vpDataTarget_ = viewProjectionMatrix_;
}

void Camera::DisplayImGui()
{
	// ワールド座標の表示
	transform_.DisplayImGui();
	// 視野角の調整
	ImGui::SliderFloat("FOV", &fov_, 0.45f, 1.2f);
}

void Camera::SetVPDataTarget()
{
	vpDataTarget_ = DirectXCommon::GetInstance()->GetCommandManager()->GetViewProjection();
}
