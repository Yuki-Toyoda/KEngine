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

int32_t WorldTransform::CreateJoint(const Node& node, const std::optional<int32_t>& parent, std::vector<Joint>& joints)
{
	// 結果返還用
	Joint joint;
	// 情報取得
	joint.name		  = node.name;					 // ノード名
	joint.localMatrix = node.localMatrix;			 // ローカル行列
	joint.skeltonSpaceMatrix = Matrix4x4::kIdentity; // 行列生成
	joint.transform   = node.transform;				 // トランスフォーム
	joint.index		  = int32_t(joints.size());		 // 現在登録されている数をIndexに
	joint.parent	  = parent;						 // 親を取得
	// ジョイント配列に追加
	joints.push_back(joint);

	// 子ノード分ループ
	for (const Node& child : node.children) {
		// 子Jointの生成、インデックスの登録
		int32_t childIndex = CreateJoint(child, joint.index, joints);
		joints[joint.index].children.push_back(childIndex);
	}

	// 結果を返す
	return joint.index;
}

WorldTransform::Skelton WorldTransform::CreateSkelton(const Node& rootNode)
{
	// 結果返還用構造体
	Skelton skelton;
	// ジョイント生成
	skelton.root = CreateJoint(rootNode, {}, skelton.joints);

	// 名前とインデックスのマッピングを行いアクセスしやすくする
	for (const Joint& joint : skelton.joints) {
		skelton.jointMap.emplace(joint.name, joint.index);
	}

	// 結果を返す
	return skelton;
}

void WorldTransform::SkeltonUpdate(Skelton& skelton)
{
	// 全ジョイント分ループ
	for (Joint& joint : skelton.joints) {
		// ローカル行列作成
		joint.localMatrix = Quaternion::MakeAffine(joint.transform.scale, joint.transform.rotate, joint.transform.translate);
		// 親がいた場合
		if (joint.parent) { // その行列をかけ合わせる
			joint.skeltonSpaceMatrix = joint.localMatrix * skelton.joints[*joint.parent].skeltonSpaceMatrix;
		}
		else { // 親がいない場合ローカル行列を掛ける
			joint.skeltonSpaceMatrix = joint.localMatrix;
		}
	}
}

void WorldTransform::ApplyAnimation(Skelton& skelton, const Animation& anim, float animationTime)
{
	// 全ジョイント分ループ
	for (Joint& joint : skelton.joints) {
		// 対象のジョイントのアニメーションがあれば値の適用を行う
		if (auto it = anim.nodeAnimations.find(joint.name); it != anim.nodeAnimations.end()) {
			const NodeAnimation& rootNodeAnimation = (*it).second;
			joint.transform.translate = Animation::CalculateValue(rootNodeAnimation.translate.keyFrames, animationTime);
			joint.transform.rotate = Animation::CalculateValue(rootNodeAnimation.rotate.keyFrames, animationTime);
			joint.transform.scale = Animation::CalculateValue(rootNodeAnimation.scale.keyFrames, animationTime);
		}
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
	// 親をそのまま返す
	return parent_;
}

Matrix4x4 WorldTransform::GetMatWorld() const
{
	// 結果格納用
	Matrix4x4 result;

	// アフィン変換行列を計算
	result = Matrix4x4::MakeAffin(scale_, rotate_, translate_);

	// ワールド行列セット中はそれを使う
	if (worldMat_ != nullptr) {
		result = *worldMat_;
	}

	// 親がいる場合
	if (parent_) {
		Matrix4x4 parentMat = Matrix4x4();
		// 親子関係タイプが一括でない場合
		if (parentType_ != 0b111) {
			const WorldTransform* grandParent = parent_->parent_;
			if (grandParent) {
				parentMat = grandParent->GetMatWorld();
			}
			if (parentType_ & FLAG_SCALE) { // 拡縮
				parentMat = parentMat * Matrix4x4::MakeScale(parent_->scale_);
			}

			if (parentType_ & FLAG_ROTATE) { // 回転
				parentMat = parentMat * Matrix4x4::MakeRotate(parent_->rotate_);;
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


