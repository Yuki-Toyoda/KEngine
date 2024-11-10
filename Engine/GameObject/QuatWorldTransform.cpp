#include "QuatWorldTransform.h"

// 親子タイプのフラグ
const int FLAG_SCALE = 0b100; // 大きさ用フラグ
const int FLAG_ROTATE = 0b010; // 角度用フラグ
const int FLAG_TRANSLATE = 0b001; // 座標用フラグ

QuatWorldTransform::QuatWorldTransform()
{
	// 初期化
	Init();
}

QuatWorldTransform::QuatWorldTransform(Vector3 scale, Quaternion rotate, Vector3 translate)
{
	// 初期化
	Init();

	// パブリックメンバ変数に代入
	scale_ = scale;
	rotate_ = rotate;
	translate_ = translate;
}

QuatWorldTransform::QuatWorldTransform(Quaternion rotate, Vector3 translate)
{
	// 初期化
	Init();

	// パブリックメンバ変数に代入
	rotate_ = rotate;
	translate_ = translate;
}

QuatWorldTransform::QuatWorldTransform(Vector3 translate)
{
	// 初期化
	Init();

	// パブリックメンバ変数に代入
	translate_ = translate;
}

void QuatWorldTransform::Init()
{
	// パブリックメンバ変数初期化
	scale_ = { 1.0f, 1.0f, 1.0f };
	rotate_ = Quaternion();
	translate_ = { 0.0f, 0.0f, 0.0f };
	parent_ = nullptr;
	parentType_ = 0b111;
}

void QuatWorldTransform::DisplayImGui()
{
	// 各種項目をImGuiにて表示
	ImGui::Text("Transform");
	ImGui::DragFloat3("Scale", &scale_.x, 0.01f);		  // 拡縮
	ImGui::DragFloat4("Rotate", &rotate_.x, 0.01f);		  // 回転
	ImGui::DragFloat3("Translate", &translate_.x, 0.01f); // 位置座標
}

void QuatWorldTransform::DisplayImGui(const std::string& id)
{
	// 各種項目をImGuiにて表示
	ImGui::Text(id.c_str());
	ImGui::DragFloat3("Scale", &scale_.x, 0.01f);		  // 拡縮
	ImGui::DragFloat4("Rotate", &rotate_.x, 0.01f);		  // 回転
	ImGui::DragFloat3("Translate", &translate_.x, 0.01f); // 位置座標
}

void QuatWorldTransform::DisplayImGuiWithTreeNode(const std::string& id)
{
	if (ImGui::TreeNode(id.c_str())) {
		ImGui::DragFloat3("Scale", &scale_.x, 0.01f);		  // 拡縮
		ImGui::DragFloat3("Rotate", &rotate_.x, 0.01f);		  // 回転
		ImGui::DragFloat3("Translate", &translate_.x, 0.01f); // 位置座標
		ImGui::TreePop();
	}
}

void QuatWorldTransform::SetParent(QuatWorldTransform* parent, uint8_t parentType)
{
	// 親子関係をセット
	parent_ = parent;
	// 親子関係フラグを設定
	parentType_ = parentType;
}

const QuatWorldTransform* QuatWorldTransform::GetParent() const
{
	// 親をそのまま返す
	return parent_;
}

Matrix4x4 QuatWorldTransform::GetMatWorld() const
{
	// 結果格納用
	Matrix4x4 result;

	// アフィン変換行列を計算
	result = Quaternion::MakeAffine(scale_, rotate_, translate_);

	// 親がいる場合
	if (parent_) {
		Matrix4x4 parentMat = Matrix4x4();
		// 親子関係タイプが一括でない場合
		if (parentType_ != 0b111) {
			const QuatWorldTransform* grandParent = parent_->parent_;
			if (grandParent) {
				parentMat = grandParent->GetMatWorld();
			}
			if (parentType_ & FLAG_SCALE) { // 拡縮
				parentMat = parentMat * Matrix4x4::MakeScale(parent_->scale_);
			}

			if (parentType_ & FLAG_ROTATE) { // 回転
				parentMat = parentMat * Quaternion::QuaternionToMatrix(parent_->rotate_);
			}

			if (parentType_ & FLAG_TRANSLATE) { // 座標
				parentMat = parentMat * Matrix4x4::MakeTranslate(parent_->translate_);
			}
		}
		else {
			parentMat = parent_->GetMatWorld();
		}
		result = result * parentMat;
	}

	// 結果を返す
	return result;
}

Vector3 QuatWorldTransform::GetWorldPos() const
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