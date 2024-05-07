#include "PrimitiveManager.h"
#include "../Base/DirectXCommon.h"
#include "../GameObject/GameObjectManager.h"
#include "../GameObject/Core/Camera.h"

PrimitiveManager* PrimitiveManager::GetInstance()
{
	static PrimitiveManager instance;
	return &instance;
}

void PrimitiveManager::Init()
{
	// 全登録形状を削除
	primitives_.remove_if([](std::unique_ptr<IPrimitive>& primitive) {
		primitive;
		if (true) {
			return true;
		}
		return false;
	});

	// リストのクリア
	primitives_.clear();
}

void PrimitiveManager::Update()
{
	// 登録形状がNullだった場合その要素を削除
	primitives_.remove_if([](std::unique_ptr<IPrimitive>& primitive) {
		if (primitive->isDestroy_ || !primitive.get())
			return true;

		return false;
	});
}

void PrimitiveManager::Draw()
{
	// 全形状を描画
	for (std::unique_ptr<IPrimitive>& primitive : primitives_) {
		primitive->Draw();
	}	
}
