#include "Material.h"
#include "../../Base/DirectXCommon.h"
#include "../../Resource/Texture/TextureManager.h"
#include "../../../Externals/imgui/imgui.h"

Material::Material()
{
	// uvトランスフォーム初期化
	uvTransform_.Init();
}

void Material::DisplayImGui()
{
	// ツリーノード開始
	if (ImGui::TreeNode(name_.c_str())) {
		// ライティング有効トリガー
		ImGui::Checkbox("EnableLighting", &enableLighting_);

		// 色調整
		if (ImGui::TreeNode("MaterialColor")) {
			// マテリアル色
			ImGui::ColorPicker4("Color", &color_.x);

			ImGui::TreePop();
		}

		// 環境マップ移りこみ強度
		ImGui::DragFloat("EnvironmentCoefficient", &environmentCoefficient_, 0.01f, 0.0f, 1.0f);

		// Dissolve関係のデバッグ
		ImGui::DragFloat("DissolveStrength", &dissolveStrength_, 0.01f, 0.0f, 1.0f);
		ImGui::DragFloat3("DissolveEdgeColor", &dissolveEdgeColor_.x, 0.01f, 0.0f, 1.0f);
		ImGui::DragFloat("DissolveEdgeThreshold", &dissolveEdgeThreshold_, 0.01f, 0.0f, 10.0f);

		// ツリーノード終了
		ImGui::TreePop();
	}
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
	uvTransform_			= mat.uvTransform_.GetMatWorld();
	color_					= mat.color_;
	enableLighting_			= mat.enableLighting_;
	environmentCoefficient_ = mat.environmentCoefficient_;
	dissolveStrength_		= mat.dissolveStrength_;
	dissolveEdgeColor_		= mat.dissolveEdgeColor_;
	dissolveEdgeThreshold_	= mat.dissolveEdgeThreshold_;

	// テクスチャインデックスの取得
	if (mat.tex_.GetView() == -1) {
		// テクスチャがない場合デフォルトテクスチャ読み込み
		textureIndex_ = TextureManager::GetInstance()->GetDefaultTexture().GetView();
	}
	else {
		// テクスチャ番号の取得
		textureIndex_ = mat.tex_.GetView();
	}

	// インデックスのずれを修正する
	textureIndex_			-= KEngine::Config::Rendering::kMaxBuffer;

	// 代入後の結果を返す
	return *this;
}
