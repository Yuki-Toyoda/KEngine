#include "Line.h"

Line::~Line()
{
	// メッシュを破壊する
	mesh_->isDestroy_ = true;
}

void Line::Init(const std::string& name, const Vector3& position, const Vector2& thickness, const float& length, Texture* texture)
{
	// 座標とかを初期化
	name_ = name;
	position_ = position;
	thickness_ = thickness;
	color_ = { 1.0f, 1.0f, 1.0f, 1.0f };
	length_ = length;

	// ワールド座標の初期化
	transform_.Init();
	transform_.translate_ = position_;
	transform_.scale_ = Vector3{ thickness_.x, thickness_.y, length_ };

	// 新しいメッシュのインスタンスを生成
	mesh_ = PrimitiveManager::GetInstance()->CreateInstance<Mesh>();
	mesh_->name_ = name_;
	mesh_->transform_ = &transform_;
	mesh_->LoadFile("./Engine/Resource/Samples/Box", "Box.obj");
	mesh_->commonColor = &color_;
	mesh_->material_.enableLighting_ = true;
	mesh_->texture_ = texture;
}

void Line::Update()
{
	// 表示状態切り替え
	mesh_->isActive_ = isActive_;

	// 回転角のリセット
	if (rotate_.x >= (float)std::numbers::pi * 2.0f) {
		rotate_.x -= (float)std::numbers::pi * 2.0f;
	}
	if (rotate_.x <= -(float)std::numbers::pi * 2.0f) {
		rotate_.x += (float)std::numbers::pi * 2.0f;
	}

	if (rotate_.y >= (float)std::numbers::pi * 2.0f) {
		rotate_.y -= (float)std::numbers::pi * 2.0f;
	}
	if (rotate_.y <= -(float)std::numbers::pi * 2.0f) {
		rotate_.y += (float)std::numbers::pi * 2.0f;
	}

	if (rotate_.z >= (float)std::numbers::pi * 2.0f) {
		rotate_.z -= (float)std::numbers::pi * 2.0f;
	}
	if (rotate_.z <= -(float)std::numbers::pi * 2.0f) {
		rotate_.z += (float)std::numbers::pi * 2.0f;
	}
	
	transform_.translate_ = position_;
	transform_.rotate_ = rotate_;

	transform_.scale_ = Vector3(thickness_.x, length_, thickness_.y);

	// オフセットを求めるよ
	Vector3 offset = { 0.0f, length_, 0.0f };
	// 回転角を元に回転行列を求める
	Matrix4x4 rotateMat = Math::MakeRotateXYZMatrix(transform_.rotate_);

	// オフセットを元に移動
	transform_.translate_ = position_ + Math::Transform(offset, rotateMat);
	
	// 親がいるのであれば
	if (transform_.GetParent() != nullptr) {
		worldPos_ = transform_.GetWorldPos();
	}

}

void Line::DisplayImGui()
{
	if (ImGui::TreeNode("Line")) {
		ImGui::Checkbox("IsActive", &isActive_);
		ImGui::DragFloat3("Position", &position_.x, 0.05f);
		ImGui::DragFloat2("Thickness", &thickness_.x, 0.05f, 0.05f);
		ImGui::DragFloat("Length", &length_, 0.05f, 0.05f);
		ImGui::DragFloat3("Rotate", &rotate_.x, 0.005f);
		ImGui::TreePop();
	}
}

void Line::AddCollider(const std::string& name, BaseObject* object)
{
	if (transform_.GetParent() != nullptr) {
		object->AddColliderOBB(name, &transform_.scale_, &transform_.rotate_, &worldPos_);
	}
	else {
		object->AddColliderOBB(name, &transform_.scale_, &transform_.rotate_, &transform_.translate_);
	}
}

void Line::SetParent(WorldTransform* parent, uint8_t parentType)
{
	// 親子関係をセット
	transform_.SetParent(parent, parentType);
}
