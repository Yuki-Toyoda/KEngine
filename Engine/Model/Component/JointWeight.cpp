#include "JointWeight.h"
#include <assimp/scene.h>

void JointWeight::Load(aiBone* bone)
{
	// バインドポーズ保存用の4x4行列をAssimpから取得
	aiMatrix4x4 bindPoseMatrixAssimp = bone->mOffsetMatrix.Inverse();
	// 保存用変数を定義
	aiVector3D scale, translate;
	aiQuaternion rotate;

	// バインドポーズ行列からSRTを抽出
	bindPoseMatrixAssimp.Decompose(scale, rotate, translate);

	// 抽出したSRTからアフィン変換行列の生成
	Matrix4x4 bindPoseMatrix = Quaternion::MakeAffine(
		Vector3(scale.x, scale.y, scale.z),
		Quaternion(rotate.x, -rotate.y, -rotate.z, rotate.w),
		Vector3(-translate.x, translate.y, translate.z)
	);
	// 求めたアフィン変換行列の逆行列を求める
	inverseBindPoseMatrix_ = Matrix4x4::MakeInverse(bindPoseMatrix);

	// ウェイト情報を解析する
	for (uint32_t weightIndex = 0; weightIndex < bone->mNumWeights; weightIndex++) {
		// ウェイト情報を追加
		vertexWeights_.push_back({ bone->mWeights[weightIndex].mWeight, bone->mWeights[weightIndex].mVertexId });
	}
}
