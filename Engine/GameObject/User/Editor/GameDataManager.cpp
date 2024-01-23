#include "GameDataManager.h"

using namespace nlohmann;

int GameDataManager::LoadNumber = 0;

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
			// std::string型
			else if (itItem->is_number_float()) {
				// std::string型の値を登録
				std::string value = itItem->get<std::string>();
				SetValue(prevNames, itemName, value);
			}
			// 要素数が2の配列であれば
			else if (itItem->is_array() && itItem->size() == 2 && itItem->at(0).is_number()) {
				// float型のjson配列登録
				Vector2 value = { itItem->at(0), itItem->at(1) };
				SetValue(prevNames, itemName, value);
			}
			// 要素数が3の配列であれば
			else if (itItem->is_array() && itItem->size() == 3 && itItem->at(0).is_number()) {
				// float型のjson配列登録
				Vector3 value = { itItem->at(0), itItem->at(1), itItem->at(2) };
				SetValue(prevNames, itemName, value);
			}
			else if (itItem->is_array() && itItem->at(0).is_string()) {
				const std::list<std::string>& valueList = itItem->get<std::list<std::string>>();
				SetValue(prevNames, itemName, valueList);
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
	const std::string& fullPath = itGroup->first;

	// 未登録チェック
	assert(itGroup != gameDatas_.end());

	json root;
	root = json::object();

	// jsonオブジェクト登録
	root[fullPath] = json::object();
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
			else if (std::holds_alternative<std::string>(item)) {
				// string型のj登録
				root[fullPath][sectionName][itemName] = std::get<std::string>(item);
			}
			else if (std::holds_alternative<std::list<std::string>>(item)) {
				const std::list<std::string>& valueList = std::get<std::list<std::string>>(item);
				std::list<std::string> jsonArray(valueList.begin(), valueList.end());

				root[fullPath][sectionName][itemName] = jsonArray;
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
