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

void Material::LoadMaterial(aiMaterial* material, const std::string& filePath)
{
	// マテリアル名の取得
	aiString matName = material->GetName(); // マテリアル名取得
	name_			 = matName.C_Str();		// メンバ変数に代入

	// テクスチャがあるかどうか確認する
	if (material->GetTextureCount(aiTextureType_DIFFUSE) != 0) { // あった場合読み込む
		// テクスチャ名格納用
		aiString aiTextureFileName;
		// マテリアルからテクスチャ名取得
		material->GetTexture(aiTextureType_DIFFUSE, 0, &aiTextureFileName);
		// std::string型に返還
		std::string textureFileName = aiTextureFileName.C_Str();

		// 取得したテクスチャとファイルパスを用いてテクスチャ読み込み
		tex_ = TextureManager::Load(filePath, textureFileName);
	}

	// マテリアルカラーの取得
	aiColor4D diffuseColor;
	// マテリアル色を取得できるか確認する
	if (AI_SUCCESS == aiGetMaterialColor(material, AI_MATKEY_COLOR_DIFFUSE, &diffuseColor)) {
		// マテリアルから色を取得する
		color_ = { diffuseColor.r, diffuseColor.g, diffuseColor.b, diffuseColor.a };
	}

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
