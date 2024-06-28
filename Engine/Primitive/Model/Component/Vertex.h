#pragma once
#include <array>
#include "../../../Math/Vector2.h"
#include "../../../Math/Vector3.h"
#include "../../../Math/Vector4.h"

#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>

/// <summary>
/// 頂点クラス
/// </summary>
class Vertex final {
public: // コンストラクタ

	/// <summary>
	/// コンストラクタ
	/// </summary>
	Vertex() = default;
	/// <summary>
	/// デストラクタ
	/// </summary>
	~Vertex() = default;

public: // パブリックメンバ変数

	// 頂点が影響を受けるジョイントの最大数
	static const uint32_t kNumMaxInfluence = 4;

	// 情報用変数群
	Vector3 position_ = { 0.0f, 0.0f, 0.0f };		// 頂点座標
	Vector2 texCoord_ = { 0.0f, 0.0f };				// テクスチャ座標
	Vector3 normal_   = { 0.0f, 0.0f, 0.0f };		// 法線
	Vector4 color_	  = { 1.0f, 1.0f, 1.0f, 1.0f }; // 頂点色

	// スキンアニメーション用変数群
	std::array<float, kNumMaxInfluence> weights_{};		   // 頂点のウェイト配列
	std::array<int32_t, kNumMaxInfluence> jointIndices_{}; // ジョイント番号配列

	// 頂点が使用するマテリアル番号
	uint32_t materialIndex_ = 0;

};

/// <summary>
/// 頂点用構造体
/// </summary>
struct VertexData {

	// 情報用変数群
	Vector4 position_ = { 0.0f, 0.0f, 0.0f, 1.0f }; // 頂点座標
	Vector2 texCoord_ = { 0.0f, 0.0f };				// テクスチャ座標
	Vector3 normal_ = { 0.0f, 0.0f, 0.0f };			// 法線
	Vector4 color_ = { 1.0f, 1.0f, 1.0f, 1.0f };	// 頂点色

	// スキンアニメーション用変数群
	std::array<float, Vertex::kNumMaxInfluence>   weights_{};	   // 頂点のウェイト配列
	std::array<int32_t, Vertex::kNumMaxInfluence> jointIndices_{}; // ジョイント番号配列

	// 頂点が使用するマテリアル番号
	uint32_t materialIndex_ = 0;

	/// <summary>
	/// コンストラクタ
	/// </summary>
	VertexData() = default;
	/// <summary>
	/// コピーコンストラクタ
	/// </summary>
	/// <param name="v"></param>
	VertexData(const Vertex& v) {
		// 頂点情報をそのままコピーする
		*this = v;
	}

	/// <summary>
	/// =代入演算子オーバーロード
	/// </summary>
	/// <param name="v">他の頂点情報</param>
	/// <returns>頂点データ</returns>
	VertexData& operator=(const Vertex& v) {
		// 各情報の代入を行う
		position_		= { v.position_.x, v.position_.y, v.position_.z, 1.0f };
		texCoord_		= v.texCoord_;
		normal_			= v.normal_;
		color_			= v.color_;
		weights_		= v.weights_;
		jointIndices_	= v.jointIndices_;
		materialIndex_	= v.materialIndex_;

		// 代入後返す
		return *this;
	}

};
