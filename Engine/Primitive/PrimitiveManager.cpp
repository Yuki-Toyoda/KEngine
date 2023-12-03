#include "PrimitiveManager.h"
#include "../Base/DirectXCommon.h"

PrimitiveManager* PrimitiveManager::GetInstance()
{
	static PrimitiveManager instance;
	return &instance;
}

void PrimitiveManager::Initialize()
{
	// コマンドマネージャーの取得
	commandManager_ = DirectXCommon::GetInstance()->GetCommandManager();

	// 全登録形状を削除
	primitives_.remove_if([](std::unique_ptr<BasePrimitive>& primitive) {
		primitive;
		if (true) {
			return true;
		}
		return false;
	});

	// リストのクリア
	primitives_.clear();
}

void PrimitiveManager::Draw()
{
	// 登録形状がNullだった場合その要素を削除
	primitives_.remove_if([](std::unique_ptr<BasePrimitive>& primitive) {
		if (primitive->isDestroy_)
			return true;

		return false;
	});

	// レイヤー番号で昇順にソート
	primitives_.sort([](std::unique_ptr<BasePrimitive>& primitive1, std::unique_ptr<BasePrimitive>& primitive2) {return primitive1->layerNo_ < primitive2->layerNo_; });

	// 全形状を描画
	for (std::unique_ptr<BasePrimitive>& primitive : primitives_) {
		primitive->Draw(commandManager_);
	}	
}
