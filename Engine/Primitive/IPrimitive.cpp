#include "IPrimitive.h"
#include "../Base/DirectXCommon.h"
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

void IPrimitive::Draw(ID3D12GraphicsCommandList6* cmdList)
{
	// 変化する可能性のあるデータをコピー
	*transformBuffer_->data_ = transform_->GetMatWorld();

	// コマンドリストに各種バッファのアドレスをセット
	cmdList->SetGraphicsRootConstantBufferView(1, transformBuffer_->GetGPUView());		  // ワールドトランスフォーム
	cmdList->SetGraphicsRootDescriptorTable(2, meshletBuffer_->GetGPUView());			  // メッシュレット情報
	cmdList->SetGraphicsRootDescriptorTable(3, vertexBuffer_->GetGPUView());			  // 頂点情報
	cmdList->SetGraphicsRootDescriptorTable(4, uniqueVertexIndicesBuffer_->GetGPUView()); // 固有頂点インデックス
	cmdList->SetGraphicsRootDescriptorTable(5, primitiveIndicesBuffer_->GetGPUView());	  // プリミティブインデックス

	// メッシュレットのプリミティブ数分メッシュシェーダーを実行
	cmdList->DispatchMesh(GetMeshletCount(), 1, 1);
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
