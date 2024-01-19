#include "BillboardPlane.h"
#include "../../GameObject/GameObjectManager.h"
#include "../../GameObject/Core/Camera.h"

void BillboardPlane::ResizeVertices()
{
	// 頂点リストクリア
	vertices_.clear();
	// 頂点リストのリサイズ
	vertices_.resize(GetVertexCount());

	// 頂点座標設定
	vertices_[0].position = { -0.5f, 0.5f, 0.0f }; // 左上
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

void BillboardPlane::UpdateBillboardMat()
{
	// 使用中カメラのビュー行列取得
	Matrix4x4 viewMat = Math::Inverse(GameObjectManager::GetInstance()->GetUseCamera()->GetViewMatrix());
	// ビュー行列の移動要素を消す
	viewMat.m[3][0] = 0.0f;
	viewMat.m[3][1] = 0.0f;
	viewMat.m[3][2] = 0.0f;

	// ワールド行列の計算
	Matrix4x4 worldMatrix = Math::MakeAffineMatrix(
		transform_->scale_,
		transform_->rotate_,
		transform_->translate_
	);
	// ビルボード行列を調べる
	billboardMat_ = viewMat * worldMatrix;

	// 計算したワールド行列を代入
	transform_->SetWorldMat(billboardMat_);
}
