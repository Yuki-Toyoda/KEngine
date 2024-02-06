#include "TutrialCmera.h"

void TutrialCamera::Init()
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
	goalFov_ = variables->GetFloatValue(name_, "Fov");
	goalTransform_.translate_= variables->GetVector3Value(name_, "Position");
	goalTransform_.rotate_ = variables->GetVector3Value(name_, "Rotate");
}

void TutrialCamera::Update()
{
	Camera::Update();
}

void TutrialCamera::DisplayImGui()
{
	transform_.DisplayImGui();
	ImGui::DragFloat("Fov", &fov_, 0.1f);
	// ボタンを押すとこのカメラを使用する
	if (!isUseThisCamera_) {
		if (ImGui::Button("UseThisCamera"))
			UseThisCamera();
	}
	else {
		ImGui::Text("This Camera Is Used");
	}
	if (ImGui::Button("Save")) {
		// 調整項目クラスのインスタンス取得
		GlobalVariables* variables = GlobalVariables::GetInstance();

		// 値の設定
		variables->SetValue(name_, "Position", transform_.translate_);
		variables->SetValue(name_, "Rotate", transform_.rotate_);
		variables->SetValue(name_, "Fov", fov_);

		// ファイル保存
		variables->SaveFile(name_);
	}
}

void TutrialCamera::Shake()
{
}
