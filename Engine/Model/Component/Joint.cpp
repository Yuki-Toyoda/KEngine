#include "Joint.h"

int32_t Joint::Create(const Node& node, const std::optional<int32_t>& parent, std::vector<Joint>& joints)
{
	// 結果返還用
	Joint joint;
	// 情報取得
	joint.name_ = node.name_;						  // ノード名
	joint.skeltonSpaceMatrix_ = Matrix4x4::kIdentity; // 行列生成
	joint.transform_ = node.transform_;				  // トランスフォーム
	joint.index_ = int32_t(joints.size());			  // 現在登録されている数をIndexに
	joint.parent_ = parent;							  // 親を取得
	// ジョイント配列に追加
	joints.push_back(joint);

	// 子ノード分ループ
	for (const Node& child : node.children_) {
		// 子Jointの生成、インデックスの登録
		int32_t childIndex = Create(child, joint.index_, joints);
		joints[joint.index_].children_.push_back(childIndex);
	}

	// 結果を返す
	return joint.index_;
}

Matrix4x4 Joint::GetLocalMatrix() const
{
	// ローカル行列を返す
	return transform_.GetMatWorld();
}
