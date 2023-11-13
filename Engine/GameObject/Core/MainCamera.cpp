#include "MainCamera.h"
#include "../../Base/WinApp.h"

void MainCamera::Initialize()
{
	// カメラ初期位置設定
	transform_.translate_ = { 0.0f, 1.0f, -10.0f };

	// 視野角初期値の設定
	fov_ = 0.45f;
}

void MainCamera::Update()
{
	// ビュープロジェクション行列の計算
	Matrix4x4 cameraMatrix = transform_.GetMatWorld(); // ワールド行列の生成
	viewMatrix_ = Math::Inverse(cameraMatrix); // ビュー表列の生成
	Matrix4x4 projectionMatrix = Math::MakePerspectiveFovMatrix(fov_, float(WinApp::kWindowWidth) / float(WinApp::kwindowHeight), 0.1f, 100.0f); // プロジェクション行列の生成
	viewProjectionMatrix_ = viewMatrix_ * projectionMatrix; // 実際に計算
}

void MainCamera::DisplayImGui()
{
	// ワールド座標の表示
	transform_.DisplayImGui(name_);
}
