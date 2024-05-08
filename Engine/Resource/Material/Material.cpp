#include "Material.h"
#include "../../Base/DirectXCommon.h"

Material::Material()
{
	// uvトランスフォーム初期化
	uvTransform_.Init();

	// マテリアル用バッファ生成
	materialBuffer_ = std::make_unique<ConstantBuffer<MaterialData>>();		 // 生成
	materialBuffer_->Init(DirectXCommon::GetInstance()->GetDirectXDevice()); // 初期化

	// データの代入
	materialBuffer_->data_->color = color_;								// 色
	materialBuffer_->data_->enableLighting = enableLighting_;			// ライティングトリガー
}

void Material::UploadMaterial()
{
	// マテリアルデータの更新
	materialBuffer_->data_->color = color_;								// 色
	materialBuffer_->data_->enableLighting = enableLighting_;			// ライティングトリガー
}
