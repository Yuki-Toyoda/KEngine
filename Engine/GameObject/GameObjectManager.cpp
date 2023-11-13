#include "GameObjectManager.h"

GameObjectManager* GameObjectManager::GetInstance()
{
	static GameObjectManager instance;
	return &instance;
}

void GameObjectManager::Initialize()
{
	// 全オブジェクトを破壊
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
	// 破壊フラグの立ったオブジェクトを削除
	objects_.remove_if([](BaseObject* object) {
		if (object->GetIsDestroy()) {
			delete object;
			return true;
		}
		return false;
	});

	// 全オブジェクトを更新
	for (BaseObject* object : objects_) {
		object->PreUpdate(); // 共通更新を呼び出す
		object->Update();	 // 更新
	}

	// デバッグ時のみImGuiを描画
#ifdef _DEBUG
	ImGui::Begin("Objects");
	// 全オブジェクトのImGuiを描画
	for (BaseObject* object : objects_) {
		if (ImGui::TreeNode(object->GetObjectName().c_str())) {
			object->DisplayImGui();
			ImGui::TreePop();
		}		
	}
	ImGui::End();
#endif // _DEBUG

}
