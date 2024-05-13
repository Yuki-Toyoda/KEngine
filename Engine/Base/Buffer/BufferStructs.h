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
///	カメラデータ構造体
/// </summary>
struct CameraData {
	Matrix4x4 WorldViewProj;		 // 射影変換行列
	uint32_t  DrawMeshlets = false;	 // メッシュレット描画フラグ
};
