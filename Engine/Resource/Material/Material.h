#pragma once
#include "../../GameObject/WorldTransform.h"
#include "../../Math/Vector4.h"
#include "../../Base/Resource/Data/ConstantBuffer.h"
#include "../../Resource/Texture/Texture.h"

#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>

/// <summary>
/// マテリアルクラス
/// </summary>
class Material final
{
public: // コンストラクタ等

	// コンストラクタ
	Material();
	// デストラクタ
	~Material() = default;

public: // メンバ関数

	/// <summary>
	/// ImGui表示関数
	/// </summary>
	void DisplayImGui();

	/// <summary>
	/// マテリアル情報をassimpから読み込む関数
	/// </summary>
	/// <param name="material">assimpから読み込んだマテリアル</param>
	/// <param name="filePath">テクスチャ読み込みに使用するファイルパス</param>
	void LoadMaterial(aiMaterial* material, const std::string& filePath);

public: // パブリックなメンバ変数

	// マテリアル名
	std::string name_;

	// テクスチャ
	Texture tex_;

	// マテリアル色
	Vector4 color_ = { 1.0f, 1.0f, 1.0f, 1.0f };

	// uvトランスフォーム
	WorldTransform uvTransform_;

	// 環境マップ移りこみ強度
	float environmentCoefficient_ = 0.0f;

	// ライティングを行うか
	bool enableLighting_ = true;

};

/// <summary>
/// マテリアルデータ構造体
/// </summary>
struct MaterialData {
	// uvTransform
	Matrix4x4 uvTransform_;
	// マテリアル色
	Vector4   color_;
	// ライティング有効トリガー
	int32_t   enableLighting_;
	// テクスチャ番号
	int32_t   textureIndex_;
	// 環境マップ移りこみ強度
	float environmentCoefficient_;

	/// <summary>
	/// コンストラクタ
	/// </summary>
	MaterialData() = default;
	/// <summary>
	/// コピーコンストラクタ
	/// </summary>
	/// <param name="mat">コピー対象マテリアル</param>
	MaterialData(const Material& mat) {
		*this = mat;
	}

	/// <summary>
	/// =演算子オーバーロード
	/// </summary>
	/// <param name="mat">他のマテリアル</param>
	/// <returns>マテリアルデータ</returns>
	MaterialData& operator=(const Material& mat);
};
