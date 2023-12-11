#include "Plane.h"

void Plane::ResizeVertices()
{
	// 頂点リストクリア
	vertices_.clear();
	// 頂点リストのリサイズ
	vertices_.resize(GetVertexCount());

	// 頂点座標設定
	vertices_[0].position = {-0.5f, 0.5f, 0.0f }; // 左上
	vertices_[1].position = { 0.5f, 0.5f, 0.0f }; // 右上
	vertices_[2].position = { 0.5f, -0.5f, 0.0f }; // 右下
	vertices_[3].position = { -0.5f, -0.5f, 0.0f }; // 左下

	// テクスチャ座標の設定
	vertices_[0].texCoord = { 0.0f, 0.0f }; // 左上
	vertices_[1].texCoord = { 1.0f, 0.0f }; // 右上
	vertices_[2].texCoord = { 1.0f, 1.0f }; // 右下
	vertices_[3].texCoord = { 0.0f, 1.0f }; // 左下

	// 法線の設定
	for (int i = 0; i < GetVertexCount(); i++) {
		vertices_[i].normal = { 0.0f, 0.0f, -1.0f };
	}
}
