#include "GameObjectManager.h"

GameObjectManager* GameObjectManager::GetInstance()
{
	static GameObjectManager gameObjectManager;
	return &gameObjectManager;
}

void GameObjectManager::Initialize()
{
	// 全オブジェクトを削除
	for (BaseObject* object : objects_)
		object->Destroy();
	// 破壊フラグの立ったオブジェクトを削除
	objects_.remove_if([](BaseObject* object) {
		if (object->GetIsDestroy()) {
			delete object;
			return true;
		}
		return false;
	});
}

void GameObjectManager::Update()
{
	// 全オブジェクトを更新
	for (BaseObject* object : objects_)
		object->Update();

	// 破壊フラグの立ったオブジェクトを削除
	objects_.remove_if([](BaseObject* object) {
		if (object->GetIsDestroy()) {
			delete object;
			return true;
		}
		return false;
	});
}

void GameObjectManager::Draw()
{
	// コマンドリストの取得
	ID3D12GraphicsCommandList* commandList = DirectXCommon::GetInstance()->GetCommandList();

	// objの描画前処理
	OBJ::PreDraw(commandList);

	// 全オブジェクトのモデル描画
	for (BaseObject* object : objects_)
		object->Draw();

	// objの描画後処理
	OBJ::PostDraw();

	// スプライトの描画前処理
	Sprite::PreDraw(commandList);

	// 全オブジェクトのスプライト描画
	for (BaseObject* object : objects_)
		object->SpriteDraw();

	// スプライトの描画後処理
	Sprite::PostDraw();

}

void GameObjectManager::AddGameObject(BaseObject* gameObject)
{
	// 同名オブジェクト個数計算用
	int sameNameCount = 0;

	// 全オブジェクトから同名のオブジェクトがないか探す
	for (BaseObject* object : objects_) {
		// オブジェクト名を取得
		std::string objectName = object->GetObjectName();
		// オブジェクト名の末尾に数字が含まれている場合は末尾の文字を削除
		while (isdigit(objectName.at(objectName.size() - 1)))
		{
			// 末尾の文字を削除
			objectName.pop_back();
		}

		// 同名オブジェクトがあった場合は加算
		if (objectName == gameObject->GetObjectName()) {
			sameNameCount++;
		}
	}

	// 同名オブジェクトが一個でも存在する場合末尾に番号をつける
	if (sameNameCount > 0) {
		// 数字を文字列に変換
		std::string count = std::to_string((sameNameCount + 1));
		gameObject->SetObjectName(gameObject->GetObjectName() + count);
	}

	// オブジェクトのリストに生成したオブジェクトを追加する
	objects_.push_back(gameObject);
}

BaseObject* GameObjectManager::GetGameObject(std::string objectName)
{
	// 全オブジェクトから探す
	for (BaseObject* object : objects_) {
		// 引数のオブジェクト名と同じ名前のオブジェクトを見つけた場合それを返す
		if (object->GetObjectName() == objectName)
			return object;
	}
	
	// 発見できなかった場合はnullを返す
	return nullptr;

}

std::vector<BaseObject*> GameObjectManager::GetGameObject(BaseObject::Tag tag)
{
	// 取得したオブジェクトリスト
	std::vector<BaseObject*> objectlist;
	// 全オブジェクトから探す
	for (BaseObject* object : objects_) {
		// 引数のオブジェクトタグと同じタグのオブジェクトを見つけた場合それを返す
		if (object->GetObjectTag() == tag)
			objectlist.push_back(object);
	}

	return objectlist;
}

void GameObjectManager::DeleteGameObject(std::string objectName)
{
	// 全オブジェクトから探す
	for (BaseObject* object : objects_) {
		// 引数のオブジェクト名と同じ名前のオブジェクトを見つけた場合それを削除する
		if (object->GetObjectName() == objectName)
			object->Destroy();
	}
}

void GameObjectManager::DeleteGameObject(BaseObject::Tag tag)
{
	// 全オブジェクトから探す
	for (BaseObject* object : objects_) {
		// 引数のタグと同じタグのオブジェクトを見つけた場合それを削除
		if (object->GetObjectTag() == tag)
			object->Destroy();
	}
}

