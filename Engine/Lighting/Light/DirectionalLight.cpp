#include "DirectionalLight.h"
#include "../../Base/DirectXCommon.h"

void DirectionalLight::Init(const Vector4& color, const Vector3& rotation, const float intensity)
{
	// 引数の値をメンバ変数に代入
	color_	   = color;		// ライト色
	rotation_  = rotation;	// ライト角
	intensity_ = intensity; // 輝度

	// バッファの生成
	buffer_ = std::make_unique<ConstantBuffer<DirectionalLightData>>();	// 生成
	buffer_->Init(DirectXCommon::GetInstance()->GetDirectXDevice());	// 初期化

	// データの代入
	buffer_->data_->color	  = color_;		// ライト色
	buffer_->data_->rotation  = rotation_;	// ライト角
	buffer_->data_->intensity = intensity_; // 輝度
}

void DirectionalLight::Update()
{
	// データの更新
	buffer_->data_->color	  = color_;		// ライト色
	buffer_->data_->rotation  = rotation_;	// ライト角
	buffer_->data_->intensity = intensity_; // 輝度
}

void DirectionalLight::DisplayImGui()
{
	// ツリーノードでライトの各種情報を表示
	if (ImGui::TreeNode(name_.c_str())) {
		ImGui::ColorPicker4("Color", &color_.x);						  // 色
		ImGui::DragFloat3("Direction", &rotation_.x, 0.01f, -1.0f, 1.0f); // 角度
		ImGui::DragFloat("intensity", &intensity_, 0.01f, 0.0f, 10.0f);   // 輝度
		ImGui::TreePop();
	}
}
