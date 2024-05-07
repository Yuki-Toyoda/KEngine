#include "IPrimitive.h"
#include "../GameObject/WorldTransform.h"

IPrimitive::IPrimitive()
{
}

void IPrimitive::ResizeVertices()
{
	// 頂点配列を一度クリア
	vertices_.clear();
	// 頂点配列サイズをリサイズ
	vertices_.resize(GetVertexCount());
}

void IPrimitive::ResizeIndexes()
{
	// インデックス情報配列を一度クリア
	indexes_.clear();
	// インデックス情報配列をリサイズ
	indexes_.resize(GetIndexCount());

	// インデックス情報配列に頂点の数分の情報を追加
	for (int i = 0; i < GetVertexCount() - 2; i++) {
		indexes_[i * 3] = 0;	 // 1番目の頂点
		indexes_[i * 3 + 1] = i + 1; // 2番目の頂点
		indexes_[i * 3 + 2] = i + 2; // 3番目の頂点
	}
}

void IPrimitive::Draw()
{
	// 表示状態でなければ表示しない
	if (!isActive_)
		return;

	//// メッシュレットバッファへのデータコピー
	//std::memcpy(meshletBuffer_->meshlet, meshlets_.data(), sizeof(DirectX::Meshlet) * meshlets_.size());

	//// 頂点バッファへのデータコピー
	//std::memcpy(vertexBuffer_->vertex, vertices_.data(), sizeof(Vertex) * vertices_.size());

	//// 固有頂点バッファへのデータコピー
	//std::memcpy(uniqueVertexBuffer_->uniqueVertex, uniqueVertices_.data(), sizeof(uint8_t) * uniqueVertices_.size());

	//// プリミティブインデックスバッファへのデータコピー
	//std::memcpy(primitiveIndexBuffer_->primitve, primitiveIndices_.data(), sizeof(DirectX::MeshletTriangle) * primitiveIndices_.size());

	//// コマンドリストの取得
	//ID3D12GraphicsCommandList6* cmdList = cmdManager_->GetRenderCommandList();

	//// コマンドリストにテーブルをセット
	//cmdList->SetGraphicsRootDescriptorTable(1, meshletBuffer_->View);
	//cmdList->SetGraphicsRootDescriptorTable(2, vertexBuffer_->View);
	//cmdList->SetGraphicsRootDescriptorTable(3, uniqueVertexBuffer_->View);
	//cmdList->SetGraphicsRootDescriptorTable(4, primitiveIndexBuffer_->View);

	//// メッシュレットのプリミティブ数分メッシュシェーダーを実行
	//cmdList->DispatchMesh(GetMeshletCount(), 1, 1);
	
}

void IPrimitive::DisplayImGui()
{
	
}

int IPrimitive::GetVertexCount() const
{
	return 3;
}

int IPrimitive::GetIndexCount() const
{
	return (GetVertexCount() - 2) * 3; // インデックス情報の数を求めて返す
}

int IPrimitive::GetMeshletCount() const
{
	return static_cast<int>(meshlets_.size());
}

int IPrimitive::GetPrimitiveCount(const int& i) const
{
	return static_cast<int>(meshlets_[i].PrimCount);
}
