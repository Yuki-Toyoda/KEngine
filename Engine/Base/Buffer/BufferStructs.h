#pragma once
#include <stdint.h>
#include <DirectXMath.h>
#include "../../GameObject/WorldTransform.h"
#include "../../../Externals/DirectXMesh/DirectXMesh.h"
#include "../../Math/Vector2.h"
#include "../../Math/Vector3.h"
#include "../../Math/Vector4.h"
#include "../../Math/Matrix4x4.h"

/*==============================================
	リソース生成の際に参照する構造体管理用ヘッダー
 =============================================*/

/// <summary>
/// 共通で参照するデータ構造体
/// </summary>
struct CommonData {
	Matrix4x4 WorldViewProj;		 // 射影変換行列
	Matrix4x4 ViewProjection2D;		 // 2D用ViewProjection
	uint32_t  DrawMeshlets;			 // メッシュレット描画フラグ
};

/// <summary>
/// 頂点データ構造体
/// </summary>
struct VertexData {
	DirectX::XMFLOAT4 position; // 頂点座標
	DirectX::XMFLOAT2 texCoord;	// テクスチャ座標
	DirectX::XMFLOAT3 normal;   // 法線
};