#pragma once
#include "../Primitive/BasePrimitive.h"

#pragma region インデックスデータ
/// <summary>
/// 使用するインデックス情報構造体
/// </summary>
struct IndexInfoStruct {
	uint32_t vertex;
	uint32_t cameraVP;	  // 使用するビュープロジェクション行列のインデックス
	uint32_t worldMatrix; // 使用するワールド行列のインデックス
	uint32_t material;	  // 使用するマテリアルのインデックス
	uint32_t tex2d;		  // 使用するテクスチャのインデックス
};
/// <summary>
/// インデックスバッファ構造体
/// </summary>
struct IndexBuffer {
	Microsoft::WRL::ComPtr<ID3D12Resource> resource;  // バッファリソース
	D3D12_GPU_DESCRIPTOR_HANDLE view{};				  // GPU上のハンドルを格納
	IndexInfoStruct* indexData;						  // 使用するインデックスデータ
	UINT usedCount = 0;							      // 使用中のインデックスバッファの数
};
#pragma endregion

#pragma region 頂点データ
/// <summary>
/// 頂点データ構造体
/// </summary>
struct VertexData {
	Vector4 position; // 座標
	Vector2 texCoord; // テクスチャ座標
	Vector3 normal;	  // 法線
	Vector4 color;	  // 色

	// VertexDataを=演算子で代入できるようにオーバーロード
	VertexData& operator=(const Vertex& vertex) {
		position = { vertex.position.x, vertex.position.y, vertex.position.z, 1.0f }; // 座標
		texCoord = vertex.texCoord;													  // テクスチャ座標
		normal = vertex.normal;														  // 法線
		color = color;																  // 色
		// 自身のポインタを返す
		return *this;
	}
};
/// <summary>
/// 頂点バッファ構造体
/// </summary>
struct VertexBuffer {
	Microsoft::WRL::ComPtr<ID3D12Resource> resource;  // バッファリソース
	D3D12_GPU_DESCRIPTOR_HANDLE view{};				  // GPU上のハンドルを格納
	VertexData* vertex;							      // 頂点データ本体
	UINT usedCount = 0;							      // 使用中のインデックスバッファの数
};
#pragma endregion

#pragma region マテリアルデータ
/// <summary>
/// マテリアルデータ構造体
/// </summary>
struct MaterialData {
	Matrix4x4 uvTransform;  // UVトランスフォーム
	int32_t enableLighting; // ライトの有効トリガー

	// MaterialDataを=演算子で代入できるようにオーバーロード
	MaterialData& operator=(const Material& material) {
		uvTransform = material.uvTransform_.GetMatWorld();				 // ワールド行列の取得
		enableLighting = static_cast<int32_t>(material.enableLighting_); // ライティング有効トリガー
		// 自身のポインタを返す
		return *this;
	}
};

/// <summary>
/// マテリアルバッファ構造体
/// </summary>
struct MaterialBuffer {
	Microsoft::WRL::ComPtr<ID3D12Resource> resource;  // バッファリソース
	D3D12_GPU_DESCRIPTOR_HANDLE view{};				  // GPU上のハンドルを格納
	MaterialData* material;							  // マテリアル本体
	UINT usedCount = 0;							      // 使用中のインデックスバッファの数
};
#pragma endregion

#pragma region 行列データ
/// <summary>
///  行列バッファ構造体
/// </summary>
struct MatrixBuffer {
	Microsoft::WRL::ComPtr<ID3D12Resource> resource;  // バッファリソース
	D3D12_GPU_DESCRIPTOR_HANDLE view{};				  // GPU上のハンドルを格納
	Matrix4x4* mat;							          // 行列本体
	UINT usedCount = 0;							      // 使用中のインデックスバッファの数

	// WorldTransformを=演算子で代入できるようにオーバーロード
	MatrixBuffer& operator=(const WorldTransform& transform) {
		// ワールド行列を取得
		*mat = transform.GetMatWorld();
		// 自身のポインタを返す
		return *this;
	}
};
#pragma endregion

#pragma region テクスチャ
/// <summary>
/// テクスチャバッファ構造体
/// </summary>
struct TextureBuffer {
	std::vector<Microsoft::WRL::ComPtr<ID3D12Resource>> resource; // バッファリソース
	D3D12_GPU_DESCRIPTOR_HANDLE view{};							  // GPU上のハンドルを格納
	UINT usedCount = 0;											  // 使用中のインデックスバッファの数
};
#pragma endregion

#pragma region 平行光源
/// <summary>
/// 平行光源構造体
/// </summary>
struct DirectionalLight {
	Matrix4x4 viewProjection; // ビュープロジェクション行列
	Vector4 color;			  // ライト色
	Vector3 direction;		  // ライト向き
	float intensity;		  // ライト輝度
};

struct LightBuffer
{
	Microsoft::WRL::ComPtr<ID3D12Resource> resource;  // バッファリソース
	D3D12_GPU_VIRTUAL_ADDRESS view{};				  // GPU上のハンドルを格納
	DirectionalLight* light = nullptr;				  // ライト本体
	Vector3 translate;								  // 光源座標
	Vector3 rotate;									  // 光源向き
};
#pragma endregion

