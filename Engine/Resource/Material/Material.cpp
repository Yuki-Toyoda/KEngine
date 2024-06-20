#include "Material.h"
#include "../../Base/DirectXCommon.h"
#include "../../Resource/Texture/TextureManager.h"

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
	if (tex_.GetView_() != -1) { // テクスチャの取得
		materialBuffer_->data_->textureIndex = tex_.GetView_();
	}
	else { // デフォルトテクスチャ番号の取得
		materialBuffer_->data_->textureIndex = TextureManager::GetInstance()->GetDefaultTexture().GetView_();
	}

	// テクスチャのインデックスをバッファのオフセット分戻す
	materialBuffer_->data_->textureIndex -= KEngine::Config::Rendering::kMaxBuffer;
}
