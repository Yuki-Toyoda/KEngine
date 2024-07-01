#include "Material.h"
#include "../../Base/DirectXCommon.h"
#include "../../Resource/Texture/TextureManager.h"

Material::Material()
{
	// uvトランスフォーム初期化
	uvTransform_.Init();

	// マテリアル用バッファ生成
	//materialBuffer_ = std::make_unique<ConstantBuffer<MaterialData>>();		 // 生成
	//materialBuffer_->Init(DirectXCommon::GetInstance()->GetDirectXDevice()); // 初期化
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

MaterialData& MaterialData::operator=(const Material& mat)
{
	// 各情報の代入を行う
	uvTransform_	= mat.uvTransform_.GetMatWorld();
	color_			= mat.color_;
	enableLighting_ = mat.enableLighting_;

	// テクスチャインデックスの取得
	if (mat.tex_.GetView() == -1) {
		// テクスチャがない場合デフォルトテクスチャ読み込み
		textureIndex_ = TextureManager::GetInstance()->GetDefaultTexture().GetView();
	}
	else {
		// テクスチャ番号の取得
		textureIndex_ = mat.tex_.GetView();
	}

	textureIndex_ -= KEngine::Config::Rendering::kMaxBuffer;

	// 代入後の結果を返す
	return *this;
}
