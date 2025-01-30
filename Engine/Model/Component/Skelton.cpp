#include "Skelton.h"

void Skelton::Update()
{
	// 全ジョイント分ループ
	for (Joint& joint : joints_) {
		// ローカル行列作成
		Matrix4x4 localMat = joint.GetLocalMatrix();
		// 親がいた場合
		if (joint.parent_) { // その行列をかけ合わせる
			joint.skeltonSpaceMatrix_ = localMat * joints_[*joint.parent_].skeltonSpaceMatrix_;
		}
		else { // 親がいない場合ローカル行列を掛ける
			joint.skeltonSpaceMatrix_ = localMat;
		}
	}
}

void Skelton::Create(const Node& node)
{
	// ジョイント生成
	root_ = Joint::Create(node, {}, joints_);

	// 名前とインデックスのマッピングを行いアクセスしやすくする
	for (const Joint& joint : joints_) {
		jointMap_.emplace(joint.name_, joint.index_);
	}
}
