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
	primitives_.remove_if([](BasePrimitive* primitive) {
		if (true) {
			delete primitive;
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
	primitives_.remove_if([](BasePrimitive* primitive) {
		if (primitive->isDestroy_ == true)
			return true;

		return false;
	});

	// 全形状を描画
	for (BasePrimitive* primitive : primitives_)
		primitive->Draw(commandManager_);
}
