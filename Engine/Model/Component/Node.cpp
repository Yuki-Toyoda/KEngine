#include "Node.h"

#include <assimp/scene.h>

void Node::Load(aiNode* node)
{
	// ノードの位置情報格納用
	aiVector3D	 scale, translate;
	aiQuaternion rotate;

	// assimp の行列からSRTを抽出する関数を利用
	node->mTransformation.Decompose(scale, rotate, translate);
	// 抽出した要素を代入
	transform_.scale_	  = { scale.x, scale.y, scale.z };				  // 拡縮
	transform_.rotate_	  = { rotate.x, -rotate.y, -rotate.z, rotate.w }; // 回転
	transform_.translate_ = { -translate.x, translate.y, translate.z };   // 位置

	// Node名を取得
	name_ = node->mName.C_Str();

	// 子ノード分配列スペースを確保
	children_.resize(node->mNumChildren);

	// 子ノード数分ループ
	for (uint32_t childIndex = 0; childIndex < node->mNumChildren; childIndex++) {
		// 再帰的にノードを読み込み、階層構造を作る
		children_[childIndex].Load(node->mChildren[childIndex]);
	}
}

Matrix4x4 Node::GetLocalMatrix() const
{
	// ローカル行列を返す
	return transform_.GetMatWorld();
}
