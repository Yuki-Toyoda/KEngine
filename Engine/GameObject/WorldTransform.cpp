#include "WorldTransform.h"

// 親子タイプのフラグ
const int FLAG_SCALE = 0b100; // 大きさ用フラグ
const int FLAG_ROTATE = 0b010; // 角度用フラグ
const int FLAG_TRANSLATE = 0b001; // 座標用フラグ

WorldTransform::WorldTransform()
{
	// 初期化
	Init();
}

WorldTransform::WorldTransform(Vector3 scale, Vector3 rotate, Vector3 translate)
{
	// 初期化
	Init();
	
	// パブリックメンバ変数に代入
	scale_ = scale;
	rotate_ = rotate;
	translate_ = translate;
}

WorldTransform::WorldTransform(Vector3 rotate, Vector3 translate)
{
	// 初期化
	Init();

	// パブリックメンバ変数に代入
	rotate_ = rotate;
	translate_ = translate;
}

WorldTransform::WorldTransform(Vector3 translate)
{
	// 初期化
	Init();

	// パブリックメンバ変数に代入
	translate_ = translate;
}

void WorldTransform::Init()
{
	// パブリックメンバ変数初期化
	scale_ = { 1.0f, 1.0f, 1.0f };
	rotate_ = {0.0f, 0.0f, 0.0f};
	translate_ = {0.0f, 0.0f, 0.0f};
	parent_ = nullptr;
	parentType_ = 0b111;
}

void WorldTransform::DisplayImGui()
{
	// 各種項目をImGuiにて表示
	ImGui::Text("Transform");
	ImGui::DragFloat3("Scale", &scale_.x, 0.01f);		  // 拡縮
	ImGui::DragFloat3("Rotate", &rotate_.x, 0.01f);		  // 回転
	ImGui::DragFloat3("Translate", &translate_.x, 0.01f); // 位置座標
}

void WorldTransform::DisplayImGui(const std::string& id)
{
	// 各種項目をImGuiにて表示
	ImGui::Text(id.c_str());
	ImGui::DragFloat3("Scale", &scale_.x, 0.01f);		  // 拡縮
	ImGui::DragFloat3("Rotate", &rotate_.x, 0.01f);		  // 回転
	ImGui::DragFloat3("Translate", &translate_.x, 0.01f); // 位置座標
}

void WorldTransform::DisplayImGuiWithTreeNode(const std::string& id)
{
	if (ImGui::TreeNode(id.c_str())) {
		ImGui::DragFloat3("Scale", &scale_.x, 0.01f);		  // 拡縮
		ImGui::DragFloat3("Rotate", &rotate_.x, 0.01f);		  // 回転
		ImGui::DragFloat3("Translate", &translate_.x, 0.01f); // 位置座標
		ImGui::TreePop();
	}
}

void WorldTransform::SetParent(WorldTransform* parent, uint8_t parentType)
{
	// 親子関係をセット
	parent_ = parent;
	// 親子関係フラグを設定
	parentType_ = parentType;
}

const WorldTransform* WorldTransform::GetParent()
{
	return parent_;
}

Matrix4x4 WorldTransform::GetMatWorld() const
{
	// 結果格納用
	Matrix4x4 result = Matrix4x4::MakeAffin(scale_, rotate_, translate_);

	// ワールド行列セット中はそれを使う
	if (worldMat_ != nullptr) {
		result = *worldMat_;
	}

	// 親がいる場合
	if (parent_) {
		Matrix4x4 parentMat = Matrix4x4::kIdentity;
		// 親子関係タイプが一括でない場合
		if (parentType_ != 0b111) {
			const WorldTransform* grandParent = parent_->parent_;
			if (grandParent) {
				parentMat = grandParent->GetMatWorld();
			}
			if (parentType_ & FLAG_SCALE)
				parentMat = parentMat * Matrix4x4::MakeScale(parent_->scale_);
			if (parentType_ & FLAG_ROTATE)
				parentMat = parentMat * Matrix4x4::MakeRotate(parent_->rotate_);
			if (parentType_ & FLAG_TRANSLATE)
				parentMat = parentMat * Matrix4x4::MakeTranslate(parent_->translate_);
		}
		else {
			parentMat = parent_->GetMatWorld();
		}
		result = result * parentMat;
	}
	// 結果を返す
	return result;
}

Vector3 WorldTransform::GetWorldPos() const
{
	// ワールド行列の取得
	Matrix4x4 matWorld = GetMatWorld();
	// ワールド座標の取得
	Vector3 result = {
		matWorld.m[3][0], matWorld.m[3][1], matWorld.m[3][2]
	};
	// 結果を返す
	return result;
}

WorldTransform WorldTransform::operator+(const WorldTransform wt) const
{
	// 変換用インスタンス
	WorldTransform result = *this;
	// 計算
	result.scale_ = this->scale_ + wt.scale_;
	result.rotate_ = this->rotate_ + wt.rotate_;
	result.translate_ = this->translate_ + wt.translate_;

	// 結果を返す
	return result;
}

WorldTransform WorldTransform::operator-(const WorldTransform wt) const
{
	// 変換用インスタンス
	WorldTransform result = *this;
	// 計算
	result.scale_ = this->scale_ - wt.scale_;
	result.rotate_ = this->rotate_ - wt.rotate_;
	result.translate_ = this->translate_ - wt.translate_;

	// 結果を返す
	return result;
}

WorldTransform WorldTransform::operator*(const WorldTransform wt) const
{
	// 変換用インスタンス
	WorldTransform result = *this;
	// 計算
	result.scale_ = this->scale_ * wt.scale_;
	result.rotate_ = this->rotate_ * wt.rotate_;
	result.translate_ = this->translate_ * wt.translate_;

	// 結果を返す
	return result;
}

WorldTransform WorldTransform::operator*(const float f) const
{
	// 変換用インスタンス
	WorldTransform result = *this;
	// 計算
	result.scale_ = this->scale_ * f;
	result.rotate_ = this->rotate_ * f;
	result.translate_ = this->translate_ * f;

	// 結果を返す
	return result;
}

WorldTransform WorldTransform::operator/(const WorldTransform wt) const
{
	// 変換用インスタンス
	WorldTransform result = *this;
	// 計算
	result.scale_ = this->scale_ / wt.scale_;
	result.rotate_ = this->rotate_ / wt.rotate_;
	result.translate_ = this->translate_ / wt.translate_;

	// 結果を返す
	return result;
}

bool WorldTransform::operator==(const WorldTransform wt) const
{
	// 結果を返す
	return { this->scale_ == wt.scale_ && this->rotate_ == wt.rotate_ && this->translate_ == wt.translate_ };
}


