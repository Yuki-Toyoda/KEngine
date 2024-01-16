#include "GameDataManager.h"

using namespace nlohmann;

void GameDataManager::LoadFiles()
{
	std::string saveDirectryPath = kDirectoryPath;
	// ディレクトリがなければスキップする
	if (!std::filesystem::exists(saveDirectryPath)) {
		return;
	}
	std::filesystem::directory_iterator dir_it(saveDirectryPath);
	for (const std::filesystem::directory_entry& entry : dir_it) {
		// ファイルパスを取得
		const std::filesystem::path& filePath = entry.path();

		// ファイル拡張子を取得
		std::string extension = filePath.extension().string();
		// .jsonファイル以外はスキップ
		if (extension.compare(".json") != 0) {
			continue;
		}

		// ファイル読み込み
		LoadFile(filePath.stem().string());
	}
}

void GameDataManager::LoadFile(const std::string& groupName)
{
	// 読み込むJSONファイルのフルパスを合成する
	std::string filePath = kDirectoryPath + groupName + ".json";
	// 読み込み用ファイルストリーム
	std::ifstream ifs;
	// ファイルを読み込み用に開く
	ifs.open(filePath);
	// ファイルオープン失敗
	if (!std::filesystem::exists(filePath)) {
		std::string message = "Failed open data file for write.";
		MessageBoxA(nullptr, message.c_str(), "GameData", 0);
		assert(0);
	}
	json root;

	// json文字列からjsonのデータ構造に展開
	ifs >> root;
	// ファイルを閉じる
	ifs.close();

	kLoadObjectNumber_ = 0;

	// グループ名作成
	std::string newGroupName = groupName;

	// グループを検索
	json::iterator itGroup = root.find(newGroupName);

	// 未登録チェック
	assert(itGroup != root.end());

	kLoadObjectNumber_ = 0;

	// セクション区画
	for (json::iterator itSection = itGroup->begin(); itSection != itGroup->end(); ++itSection) {
		// 名を取得
		const std::string& sectionName = itSection.key();

		// アイテム区画
		for (json::iterator itItem = itSection->begin();
			itItem != itSection->end(); ++itItem) {
			// アイテム名を取得
			const std::string& itemName = itItem.key();

			HierarchicalName prevNames = { groupName ,sectionName };

			// int32_t型
			if (itItem->is_number_integer()) {
				// int型の値を登録
				int32_t value = itItem->get<int32_t>();
				SetValue(prevNames, itemName, value);
			}
			// float型
			else if (itItem->is_number_float()) {
				// int型の値を登録
				double value = itItem->get<double>();
				SetValue(prevNames, itemName, static_cast<float>(value));
			}
			// 要素数が2の配列であれば
			else if (itItem->is_array() && itItem->size() == 2) {
				// float型のjson配列登録
				Vector2 value = { itItem->at(0), itItem->at(1) };
				SetValue(prevNames, itemName, value);
			}
			// 要素数が3の配列であれば
			else if (itItem->is_array() && itItem->size() == 3) {
				// float型のjson配列登録
				Vector3 value = { itItem->at(0), itItem->at(1), itItem->at(2) };
				SetValue(prevNames, itemName, value);
			}
		}

		// オブジェクトナンバー更新
		kLoadObjectNumber_++;
	}
}

void GameDataManager::SaveData(const std::string& groupName)
{
	// グループを検索
	std::map<std::string, Group>::iterator itGroup = gameDatas_.find(groupName);
	// オブジェクト内のグループ名作成
	const std::string& fullPath = itGroup->first/* + std::to_string(groupNum)*/;

	// 未登録チェック
	assert(itGroup != gameDatas_.end());

	json root;
	root = json::object();

	// jsonオブジェクト登録
	root[fullPath] = json::object();
	kLoadObjectNumber_ = 0;
	kLoadObjectNumber_ = 0;

	// 各項目について
	for (std::map<std::string, Section>::iterator itSection = itGroup->second.begin();
		itSection != itGroup->second.end(); ++itSection) {

		// 項目名を取得
		const std::string& sectionName = itSection->first;

		// jsonオブジェクト
		root[fullPath][sectionName];

		for (std::map<std::string, Item>::iterator itItem = itSection->second.begin();
			itItem != itSection->second.end(); ++itItem) {
			// 項目名を取得
			const std::string& itemName = itItem->first;
			// 項目の参照を取得
			Item& item = itItem->second;

			// int32_t型の値を保持していれば
			if (std::holds_alternative<int32_t>(item)) {
				// int32_t型の値を登録
				root[fullPath][sectionName][itemName] = std::get<int32_t>(item);
			}
			else if (std::holds_alternative<float>(item)) {
				// float型の値を登録
				root[fullPath][sectionName][itemName] = std::get<float>(item);
			}
			else if (std::holds_alternative<Vector2>(item)) {
				// float型のjson配列登録
				Vector2 value = std::get<Vector2>(item);
				root[fullPath][sectionName][itemName] = json::array({ value.x, value.y });
			}
			else if (std::holds_alternative<Vector3>(item)) {
				// float型のjson配列登録
				Vector3 value = std::get<Vector3>(item);
				root[fullPath][sectionName][itemName] = json::array({ value.x, value.y, value.z });
			}
		}
	}
	// ディレクトリがなければ作成する
	std::filesystem::path dir(kDirectoryPath);
	if (!std::filesystem::exists(kDirectoryPath)) {
		std::filesystem::create_directories(kDirectoryPath);
	}
	// 書き込むJSONファイルのフルパスを合成する
	std::string filePath = kDirectoryPath + groupName + ".json";
	// 書き込み用ファイルストリーム
	std::ofstream ofs;
	// ファイルを書き込み用に開く
	ofs.open(filePath);

	// ファイルオープン失敗？
	if (ofs.fail()) {
		std::string message = "Failed open data file for write.";
		MessageBoxA(nullptr, message.c_str(), "Editors", 0);
		assert(0);
		return;
	}
	// ファイルにjson文字列を書き込む(インデント幅4)
	ofs << std::setw(4) << root << std::endl;
	// ファイルを閉じる
	ofs.close();

}

#pragma region 設定
void GameDataManager::SetValue(const HierarchicalName& names, const std::string& key, int32_t value)
{
	// グループの参照を取得
	Section& section = gameDatas_[names.kGroup][names.kSection];
	// 新しい項目のデータを設定
	Item newItem = value;
	// 設定した項目をstd::mapに追加
	section[key] = newItem;
}

void GameDataManager::SetValue(const HierarchicalName& names, const std::string& key, float value)
{
	// グループの参照を取得
	Section& section = gameDatas_[names.kGroup][names.kSection];
	// 新しい項目のデータを設定
	Item newItem = value;
	// 設定した項目をstd::mapに追加
	section[key] = newItem;
}

void GameDataManager::SetValue(const HierarchicalName& names, const std::string& key, Vector2 value)
{
	// グループの参照を取得
	Section& section = gameDatas_[names.kGroup][names.kSection];
	// 新しい項目のデータを設定
	Item newItem = value;
	// 設定した項目をstd::mapに追加
	section[key] = newItem;
}

void GameDataManager::SetValue(const HierarchicalName& names, const std::string& key, Vector3 value)
{
	// グループの参照を取得
	Section& section = gameDatas_[names.kGroup][names.kSection];
	// 新しい項目のデータを設定
	Item newItem = value;
	// 設定した項目をstd::mapに追加
	section[key] = newItem;
}
#pragma endregion

#pragma region 追加
void GameDataManager::AddItem(const HierarchicalName& names, const std::string& key, int32_t value)
{
	// 項目が未登録なら
	if (gameDatas_[names.kGroup][names.kSection].find(key) ==
		gameDatas_[names.kGroup][names.kSection].end()) {
		SetValue(names, key, value);
	}
}

void GameDataManager::AddItem(const HierarchicalName& names, const std::string& key, float value)
{
	// 項目が未登録なら
	if (gameDatas_[names.kGroup][names.kSection].find(key) ==
		gameDatas_[names.kGroup][names.kSection].end()) {
		SetValue(names, key, value);
	}
}

void GameDataManager::AddItem(const HierarchicalName& names, const std::string& key, Vector2 value)
{
	// 項目が未登録なら
	if (gameDatas_[names.kGroup][names.kSection].find(key) ==
		gameDatas_[names.kGroup][names.kSection].end()) {
		SetValue(names, key, value);
	}
}

void GameDataManager::AddItem(const HierarchicalName& names, const std::string& key, Vector3 value)
{
	// 項目が未登録なら
	if (gameDatas_[names.kGroup][names.kSection].find(key) ==
		gameDatas_[names.kGroup][names.kSection].end()) {
		SetValue(names, key, value);
	}
}
#pragma endregion

#pragma region 取得
int GameDataManager::GetIntValue(const HierarchicalName& names, const std::string& key)
{
	// 指定グループが存在するか
	assert(gameDatas_.find(names.kGroup) != gameDatas_.end());
	// セクション探し
	assert(gameDatas_[names.kGroup].find(names.kSection) != gameDatas_[names.kGroup].end());
	// セクションの参照を取得
	Section& section = gameDatas_[names.kGroup][names.kSection];

	// 指定グループに指定キーが存在するか
	assert(section.find(key) != section.end());
	// 指定グループから指定のキーの値を取得
	return std::get<0>(section[key]);
}
float GameDataManager::GetFloatValue(const HierarchicalName& names, const std::string& key)
{
	// 指定グループが存在するか
	assert(gameDatas_.find(names.kGroup) != gameDatas_.end());
	// セクション探し
	assert(gameDatas_[names.kGroup].find(names.kSection) != gameDatas_[names.kGroup].end());
	// セクションの参照を取得
	Section& section = gameDatas_[names.kGroup][names.kSection];

	// 指定グループに指定キーが存在するか
	assert(section.find(key) != section.end());
	// 指定グループから指定のキーの値を取得
	return std::get<1>(section[key]);
}
Vector2 GameDataManager::GetVector2Value(const HierarchicalName& names, const std::string& key)
{
	// 指定グループが存在するか
	assert(gameDatas_.find(names.kGroup) != gameDatas_.end());
	// セクション探し
	assert(gameDatas_[names.kGroup].find(names.kSection) != gameDatas_[names.kGroup].end());
	// セクションの参照を取得
	Section& section = gameDatas_[names.kGroup][names.kSection];

	// 指定グループに指定キーが存在するか
	assert(section.find(key) != section.end());
	// 指定グループから指定のキーの値を取得
	return std::get<2>(section[key]);
}
Vector3 GameDataManager::GetVector3Value(const HierarchicalName& names, const std::string& key)
{
	// 指定グループが存在するか
	assert(gameDatas_.find(names.kGroup) != gameDatas_.end());
	// セクション探し
	assert(gameDatas_[names.kGroup].find(names.kSection) != gameDatas_[names.kGroup].end());
	// セクションの参照を取得
	Section& section = gameDatas_[names.kGroup][names.kSection];

	// 指定グループに指定キーが存在するか
	assert(section.find(key) != section.end());
	// 指定グループから指定のキーの値を取得
	return std::get<3>(section[key]);
}
#pragma endregion
