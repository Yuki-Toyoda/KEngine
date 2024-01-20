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

void PrimitiveManager::Update()
{
	// 登録形状がNullだった場合その要素を削除
	primitives_.remove_if([](std::unique_ptr<BasePrimitive>& primitive) {
		if (primitive->isDestroy_ || !primitive.get())
			return true;

		return false;
	});

	// レイヤー番号で昇順にソート
	//primitives_.sort([](std::unique_ptr<BasePrimitive>& primitive1, std::unique_ptr<BasePrimitive>& primitive2) {return primitive1->layerNo_ > primitive2->layerNo_; });
}

void PrimitiveManager::Draw()
{
	// 全形状を描画
	for (std::unique_ptr<BasePrimitive>& primitive : primitives_) {
		primitive->Draw(commandManager_);
	}	
	
	// フェード演出用形状描画
	fadePlane_->Draw(commandManager_);
}

Plane* PrimitiveManager::CreateFadePlaneInstance()
{
	// 形状のインスタンス
	std::unique_ptr<Plane> newPrimitive = std::make_unique<Plane>(commandManager_);
	newPrimitive->ResizeVertices();
	newPrimitive->ResizeIndexes();

	// 生成した形状をインスタンスにセット
	fadePlane_ = std::move(newPrimitive);

	// 生成した形状を返す
	return fadePlane_.get();
}
