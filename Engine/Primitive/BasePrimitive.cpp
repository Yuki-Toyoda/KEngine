#include "BasePrimitive.h"
#include "../Base/Command/CommandManager.h"

BasePrimitive::BasePrimitive(CommandManager* manager)
{
	// 基底クラスでは使用しない
	manager;
}

void BasePrimitive::ResizeVertices()
{
	// 頂点配列を一度クリア
	vertices_.clear();
	// 頂点配列サイズをリサイズ
	vertices_.resize(GetVertexCount());
}

void BasePrimitive::ResizeIndexes()
{
	// インデックス情報配列を一度クリア
	indexes_.clear();
	// インデックス情報配列をリサイズ
	indexes_.reserve(GetIndexCount());

	// インデックス情報配列に頂点の数分の情報を追加
	for (int i = 0; i < GetVertexCount() - 2; i++) {
		indexes_[i * 3] = 0;	 // 1番目の頂点
		indexes_[i * 3 + 1] = 1; // 2番目の頂点
		indexes_[i * 3 + 2] = 2; // 3番目の頂点
	}
}

void BasePrimitive::Draw(CommandManager* manager)
{
	// 表示状態でなければ表示しない
	if (!isActive_)
		return;

	// マネージャーに頂点情報を送る
	manager->SetDrawData(this);
}

int BasePrimitive::GetVertexCount() const
{
	return 3;
}

int BasePrimitive::GetIndexCount() const
{
	return (GetVertexCount() - 2) * 3; // インデックス情報の数を求めて返す
}
