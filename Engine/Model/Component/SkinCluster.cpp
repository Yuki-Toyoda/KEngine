#include "SkinCluster.h"

SkinCluster::SkinCluster(uint32_t size)
{
	// バインドポーズ逆行列を格納する場所を確保する
	inverseBindPoseMatrices_.resize(size);
	// 生成した場所を単位行列で埋める
	std::generate(inverseBindPoseMatrices_.begin(), inverseBindPoseMatrices_.end(), Matrix4x4::MakeIndentity);
}
