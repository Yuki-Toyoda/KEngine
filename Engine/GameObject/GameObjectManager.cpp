#include "GameObjectManager.h"
#include "Core/Camera.h"
#include "Engine/Base/DirectXCommon.h"

GameObjectManager* GameObjectManager::GetInstance()
{
	static GameObjectManager instance;
	return &instance;
}

void GameObjectManager::Init()
{
	// DirectX汎用クラスを取得
	dxCommon_ = DirectXCommon::GetInstance();

	// 全オブジェクトを破壊
	for (std::unique_ptr<IObject>& object : objects_)
		object->Destroy();
	// 破壊フラグの立ったオブジェクトを削除
	objects_.remove_if([](std::unique_ptr<IObject>& object) {
		if (object->GetIsDestroy()) {
			// 全通常モデルを削除
			for (std::map<std::string, NormalModel*>::const_iterator it = object->normalModels_.cbegin(); it != object->normalModels_.cend(); ++it) {
				it->second->isDestroy_ = true;
			}
			// 全スキニングモデルを削除
			for (std::map<std::string, SkiningModel*>::const_iterator it = object->skiningModels_.cbegin(); it != object->skiningModels_.cend(); ++it) {
				it->second->isDestroy_ = true;
			}
			// 全スプライトを削除
			for (std::map<std::string, Sprite*>::const_iterator it = object->sprites_.cbegin(); it != object->sprites_.cend(); ++it) {
				it->second->Destroy();
			}

			return true;
		}
		return false;
	});

#ifdef _DEBUG // ImGui用デバッグ変数

	// 選択オブジェクトリセット
	imGuiSelectObjects_ = 0;
	
#endif // _DEBUG // ImGui用デバッグ変数

}

void GameObjectManager::Update()
{
	// 破壊フラグの立ったオブジェクトを削除
	objects_.remove_if([](std::unique_ptr<IObject>& object) {
		if (object->GetIsDestroy()) {
			// 全通常モデルを削除
			for (std::map<std::string, NormalModel*>::const_iterator it = object->normalModels_.cbegin(); it != object->normalModels_.cend(); ++it) {
				it->second->isDestroy_ = true;
			}
			// 全スキニングモデルを削除
			for (std::map<std::string, SkiningModel*>::const_iterator it = object->skiningModels_.cbegin(); it != object->skiningModels_.cend(); ++it) {
				it->second->isDestroy_ = true;
			}
			// 全スプライトを削除
			for (std::map<std::string, Sprite*>::const_iterator it = object->sprites_.cbegin(); it != object->sprites_.cend(); ++it) {
				it->second->Destroy();
			}

			return true;
		}
		return false;
	});

	// 全オブジェクトを更新
	for (std::unique_ptr<IObject>& object : objects_) {
		object->PreUpdate();  // 共通更新を呼び出す
		object->Update();	  // 更新
		object->PostUpdate(); // 更新後処理
	}

	// メインカメラを毎フレームセットする
	dxCommon_->SetMainCamera(mainCamera_);

	// デバッグ時のみImGuiを描画
#ifdef _DEBUG
	ImGui::Begin("Objects");
	// 全オブジェクトのImGuiを描画
	ImGui::BeginChild(ImGui::GetID((void*)0), ImVec2(250, 100), ImGuiWindowFlags_NoTitleBar);
	// オブジェクトが1つでもあった場合
	if (objects_.size() > 0) {
		for (int i = 0; i < objects_.size(); i++) {
			ImGui::RadioButton(GetGameObject(i)->GetObjectName().c_str(), &imGuiSelectObjects_, i);
		}
	}
	else // 1つもオブジェクトがない場合テキストで表示
		ImGui::Text("No Objects!");
	ImGui::EndChild();
	
	// 選択されているImGuiのインデックスがオブジェクト配列のサイズを超過している場合
	if (imGuiSelectObjects_ > objects_.size()) {
		// 選択インデックスをサイズ内に限定する
		imGuiSelectObjects_ = static_cast<int32_t>(objects_.size() - 1);
	}

	// オブジェクトが1つでもある場合は変数で指定されたオブジェクトの情報を表示
	if (objects_.size() > 0) {
		GetGameObject(imGuiSelectObjects_)->DisplayImGui();
	}

	ImGui::End();
#endif // _DEBUG

}

IObject* GameObjectManager::GetGameObject(int index)
{
	// カウント用
	int count = 0;

	// オブジェクトリスト内のオブジェクト番号と一致するオブジェクトを探す
	for (std::unique_ptr<IObject>& object : objects_) {
		if (count == index) // 引数と一致した場合
			return object.get(); // そのオブジェクトを返す
		count++; // 一致していなかった場合カウントをインクリメント
	}
	// 発見できなかった場合nullptrを返す
	return nullptr;
}
