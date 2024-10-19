#include "GlobalVariables.h"

#pragma region private

GlobalVariables::GlobalVariables() {}
GlobalVariables::~GlobalVariables() {}

#pragma endregion

using namespace nlohmann;

GlobalVariables* GlobalVariables::GetInstance() { 
	static GlobalVariables instance;
	return &instance;
}

void GlobalVariables::Update() { 
	// デバック時にのみ有効
	#ifdef _DEBUG
	// メニューバーが使用可能なウィンドウを開く
	if (!ImGui::Begin("Global Variables", nullptr, ImGuiWindowFlags_MenuBar)) {
		ImGui::End();
		return;
	}
	if (!ImGui::BeginMenuBar())
		return;

	// 全てのグループを取得
	for (std::map<std::string, Group>::iterator itGroup = datas_.begin(); itGroup != datas_.end();
	     itGroup++) {
		// グループ名(キー)を取得
		const std::string& groupName = itGroup->first;
		// グループの参照(値)を取得
		Group& group = itGroup->second;

		if (!ImGui::BeginMenu(groupName.c_str()))
			continue;

		// 各項目を取得
		for (std::map<std::string, Item>::iterator itItem = group.items.begin();
		     itItem != group.items.end(); itItem++) {
			// 項目名(キー)を取得
			const std::string& itemName = itItem->first;
			// 項目の参照(値)を取得
			Item& item = itItem->second;

			// int型の値を保持している場合
			if (std::holds_alternative<int32_t>(item.value)) {
				// int32_t型の値を取得
				int32_t* ptr = std::get_if<int32_t>(&item.value);
				// ImGuiのUIを出す
				ImGui::DragInt(itemName.c_str(), ptr, 1.0f);
			}
			// float型の値を保持している場合
			else if (std::holds_alternative<float>(item.value)) {
				// float型の値を取得
				float* ptr = std::get_if<float>(&item.value);
				// ImGuiのUIを出す
				ImGui::DragFloat(itemName.c_str(), ptr, 0.01f);
			}
			// Vector2型の値を保持している場合
			else if (std::holds_alternative<Vector2>(item.value)) {
				// Vector2型の値を取得
				Vector2* ptr = std::get_if<Vector2>(&item.value);
				// ImGuiのUIを出す
				ImGui::DragFloat2(
				    itemName.c_str(), reinterpret_cast<float*>(ptr), 0.1f);
			}
			// Vector3型の値を保持している場合
			else if (std::holds_alternative<Vector3>(item.value)) {
				// Vector3型の値を取得
				Vector3* ptr = std::get_if<Vector3>(&item.value);
				// ImGuiのUIを出す
				ImGui::DragFloat3(
				    itemName.c_str(), reinterpret_cast<float*>(ptr), 0.1f);
			}
			// string型の値を保持している場合
			else if (std::holds_alternative<std::string>(item.value)) {
				// string型の値を取得
				std::string* ptr = std::get_if<std::string>(&item.value);
				// ImGuiのUIを出す
				ImGui::Text(ptr->c_str());
			}
		}

		// 改行する
		ImGui::Text("\n");
		// セーブするボタンを出す
		if (ImGui::Button("Save")) {
			// ファイル保存を行う
			SaveFile(groupName);
			// セーブが完了したことをメッセージボックスで出す
			std::string message = std::format("{}.json saved.", groupName);
			MessageBoxA(nullptr, message.c_str(), "GlobalVariables", 0);
		}

		ImGui::EndMenu();
	}

	ImGui::EndMenuBar();
	ImGui::End();
#endif // _DEBUG
}

void GlobalVariables::CreateGroup(const std::string& groupName) { 
	// 引数名のオブジェクトがなければ追加する
	datas_[groupName]; 
}

void GlobalVariables::SaveFile(const std::string& groupName) {
	// 指定されたグループが登録されているか検索
	std::map<std::string, Group>::iterator itGroup = datas_.find(groupName);
	// 未登録だった場合はassertでゲームを落とす
	assert(itGroup != datas_.end());
	
	// json型のインスタンスを生成
	json root;
	// コンテナを作成（std::map のようなもの）
	root = json::object();
	// 1グループ分のjsonオブジェクトを登録
	root[groupName] = json::object();

	for (std::map<std::string, Item>::iterator itItem = itGroup->second.items.begin();
	     itItem != itGroup->second.items.end(); itItem++) {
		// 項目名(キー)を取得
		const std::string& itemName = itItem->first;
		// 項目の参照(値)を取得
		Item& item = itItem->second;

		// int型の値を保持している場合
		if (std::holds_alternative<int32_t>(item.value)) {
			// int32_t型の値を登録
			root[groupName][itemName] = std::get<int32_t>(item.value);
		}
		// float型の値を保持している場合
		else if (std::holds_alternative<float>(item.value)) {
			// float型の値を登録
			root[groupName][itemName] = std::get<float>(item.value);
		}
		// Vector2型の値を保持している場合
		else if (std::holds_alternative<Vector2>(item.value)) {
			// Vector2型の値を登録
			Vector2 value = std::get<Vector2>(item.value);
			// jsonの配列でVector2の要素一つ一つを格納する
			root[groupName][itemName] = json::array({value.x, value.y});
		}
		// Vector3型の値を保持している場合
		else if (std::holds_alternative<Vector3>(item.value)) {
			// Vector3型の値を登録
			Vector3 value = std::get<Vector3>(item.value);
			// jsonの配列でVector3の要素一つ一つを格納する
			root[groupName][itemName] = json::array({value.x, value.y, value.z});
		}
		// std::stirng型の値を保持している場合
		else if (std::holds_alternative<std::string>(item.value)) {
			// Vector3型の値を登録
			std::string value = std::get<std::string>(item.value);
			// std::string型の値を登録
			root[groupName][itemName] = json::string_t(value);
		}
	}

	// ディレクトリがなければ新規で作成する
	std::filesystem::path dir(kDirectorypath);
	if (!std::filesystem::exists(kDirectorypath)) {
		std::filesystem::create_directory(kDirectorypath);
	}

	// 書き込むJsonファイルのフルパスを合成して生成
	std::string filePath = kDirectorypath + groupName + ".json";
	// 書き込む用のファイルストリーム
	std::ofstream ofs;
	// ファイルを書き込み用に開く
	ofs.open(filePath);

	// ファイルオープンが失敗した場合ログを出して停止
	if (ofs.fail()) {
		// 読み込み失敗メッセージ
		std::string message = "Failed open data file for write.";
		// メッセージボックスを出す
		MessageBoxA(nullptr, message.c_str(), "GlobalVariables", 0);
		// ゲーム停止
		assert(0);
		return;
	}

	// ファイルにjson文字列を書き込む インデント幅4
	ofs << std::setw(4) << root << std::endl;
	// ファイルを閉じる
	ofs.close();
}

void GlobalVariables::LoadFiles() {
	// 保存先ディレクトリのパスをローカル変数で宣言する
	std::filesystem::path dir(kDirectorypath);

	// ディレクトリがなければスキップする
	if (!std::filesystem::exists(kDirectorypath)) {
		return;
	}

	// ディレクトリ内のファイルのポインタの型を用意
	std::filesystem::directory_iterator dir_it(kDirectorypath);
	// ディレクトリ内の1要素を取り出し、entry変数に格納する
	for (const std::filesystem::directory_entry& entry : dir_it) {
		// ファイルパスを取得する
		const std::filesystem::path& filePath = entry.path();
		// ファイルの拡張子を取得
		std::string extension = filePath.extension().string();

		// .jsonファイル以外は処理を飛ばす
		if (extension.compare(".json") != 0) {
			// 処理を続ける
			continue;
		}

		// ファイルを読み込む
		LoadFile(filePath.stem().string());

	}
}

void GlobalVariables::LoadFile(const std::string& groupName) {
	// 読み込むJsonファイルのフルパスを合成する
	std::string filePath = kDirectorypath + groupName + ".json";
	// 読み込み用ファイルストリーム
	std::ifstream ifs;
	// ファイル読み込み用に開く
	ifs.open(filePath);

	// ファイルオープンが失敗した場合ログを出して停止
	if (ifs.fail()) {
		// 読み込み失敗メッセージ
		std::string message = "Failed open data file for write.";
		// メッセージボックスを出す
		MessageBoxA(nullptr, message.c_str(), "GlobalVariables", 0);
		// ゲーム停止
		assert(0);
		return;
	}

	// json型のインスタンスを生成
	json root;
	// json文字列からjsonのデータ構造に展開
	ifs >> root;
	// 開いているファイルを閉じる
	ifs.close();

	// 引数で指定したグループがJsonオブジェクト内に登録されているか確認
	json::iterator itGroup = root.find(groupName);
	// 未登録だった場合ゲームを停止させる
	assert(itGroup != root.end());

	// 各アイテムについて
	for (json::iterator itItem = itGroup->begin(); itItem != itGroup->end(); itItem++) {
		// アイテム名取得
		const std::string& itemName = itItem.key();

		// int32_t型の値を保持している場合
		if (itItem->is_number_integer()) {
			// int型の値を登録する
			int32_t value = itItem->get<int32_t>();
			// 値をセットする
			SetValue(groupName, itemName, value);
		}
		// float型の値を保持している場合
		else if (itItem->is_number_float()) {
			// float型の値を登録する
			float value = itItem->get<float>();
			// 値をセットする
			SetValue(groupName, itemName, value);
		}
		// Vector2型の値を保持している場合
		else if (itItem->is_array() && itItem->size() == 2) {
			// Vector3型の値を登録する
			Vector2 value = {itItem->at(0), itItem->at(1)};
			// 値をセットする
			SetValue(groupName, itemName, value);
		}
		// Vector3型の値を保持している場合
		else if (itItem->is_array() && itItem->size() == 3) {
			// Vector3型の値を登録する
			Vector3 value = {itItem->at(0), itItem->at(1), itItem->at(2)};
			// 値をセットする
			SetValue(groupName, itemName, value);
		}
		// string型の値を保持している場合
		else if (itItem->is_string()) {
			// std::string型の値を登録する
			std::string value = itItem->get<std::string>();
			// 値をセットする
			SetValue(groupName, itemName, value);
		}
	}
}

void GlobalVariables::SetValue(
    const std::string& groupName, const std::string& key, int32_t value) {
	// グループの参照を取得する
	Group& group = datas_[groupName];
	// 新しい項目のデータを設定
	Item newItem{};
	newItem.value = value;
	// 設定を行った項目をデータコンテナに追加
	group.items[key] = newItem;
}

void GlobalVariables::SetValue(const std::string& groupName, const std::string& key, float value) {
	// グループの参照を取得する
	Group& group = datas_[groupName];
	// 新しい項目のデータを設定
	Item newItem{};
	newItem.value = value;
	// 設定を行った項目をデータコンテナに追加
	group.items[key] = newItem;
}

void GlobalVariables::SetValue(
    const std::string& groupName, const std::string& key, const Vector2& value) {
	// グループの参照を取得する
	Group& group = datas_[groupName];
	// 新しい項目のデータを設定
	Item newItem{};
	newItem.value = value;
	// 設定を行った項目をデータコンテナに追加
	group.items[key] = newItem;
}

void GlobalVariables::SetValue(
    const std::string& groupName, const std::string& key, const Vector3& value) {
	// グループの参照を取得する
	Group& group = datas_[groupName];
	// 新しい項目のデータを設定
	Item newItem{};
	newItem.value = value;
	// 設定を行った項目をデータコンテナに追加
	group.items[key] = newItem;
}

void GlobalVariables::SetValue(const std::string& groupName, const std::string& key, const std::string& value)
{
	// グループの参照を取得する
	Group& group = datas_[groupName];
	// 新しい項目のデータを設定
	Item newItem{};
	newItem.value = value;
	// 設定を行った項目をデータコンテナに追加
	group.items[key] = newItem;
}

void GlobalVariables::AddItem(const std::string& groupName, const std::string& key, int32_t value) {
	// 同名のキーを見つけたら処理を抜ける
	if (datas_[groupName].items.find(key) == datas_[groupName].items.end()) {
		// 関数呼び出し
		SetValue(groupName, key, value);
	}

}

void GlobalVariables::AddItem(const std::string& groupName, const std::string& key, float value) {
	// 同名のキーを見つけたら処理を抜ける
	if (datas_[groupName].items.find(key) == datas_[groupName].items.end()) {
		// 関数呼び出し
		SetValue(groupName, key, value);
	}
}

void GlobalVariables::AddItem(const std::string& groupName, const std::string& key, Vector2 value) {
	// 同名のキーを見つけたら処理を抜ける
	if (datas_[groupName].items.find(key) == datas_[groupName].items.end()) {
		// 関数呼び出し
		SetValue(groupName, key, value);
	}
}

void GlobalVariables::AddItem(const std::string& groupName, const std::string& key, Vector3 value) {
	// 同名のキーを見つけたら処理を抜ける
	if (datas_[groupName].items.find(key) == datas_[groupName].items.end()) {
		// 関数呼び出し
		SetValue(groupName, key, value);
	}
}

void GlobalVariables::AddItem(const std::string& groupName, const std::string& key, const std::string& value)
{
	// 同名のキーを見つけたら処理を抜ける
	if (datas_[groupName].items.find(key) == datas_[groupName].items.end()) {
		// 関数呼び出し
		SetValue(groupName, key, value);
	}
}

int32_t GlobalVariables::GetIntValue(const std::string& groupName, const std::string& key){
	// 指定グループがいない場合エラー
	assert(datas_.find(groupName) != datas_.end());

	// グループの参照を取得する
	Group& group = datas_[groupName];

	// 指定グループに指定のキーが存在しない場合エラー
	assert(group.items.find(key) != group.items.end());

	// 指定グループから指定のキーの値を取得
	return std::get<int32_t>(group.items[key].value);
}

float GlobalVariables::GetFloatValue(const std::string& groupName, const std::string& key){
	// 指定グループがいない場合エラー
	assert(datas_.find(groupName) != datas_.end());

	// グループの参照を取得する
	Group& group = datas_[groupName];

	// 指定グループに指定のキーが存在しない場合エラー
	assert(group.items.find(key) != group.items.end());

	// 指定グループから指定のキーの値を取得
	return std::get<float>(group.items[key].value);
}

Vector2 GlobalVariables::GetVector2Value(const std::string& groupName, const std::string& key) {
	// 指定グループがいない場合エラー
	assert(datas_.find(groupName) != datas_.end());

	// グループの参照を取得する
	Group& group = datas_[groupName];

	// 指定グループに指定のキーが存在しない場合エラー
	assert(group.items.find(key) != group.items.end());

	// 指定グループから指定のキーの値を取得
	return std::get<Vector2>(group.items[key].value);
}

Vector3 GlobalVariables::GetVector3Value(const std::string& groupName, const std::string& key){
	// 指定グループがいない場合エラー
	assert(datas_.find(groupName) != datas_.end());

	// グループの参照を取得する
	Group& group = datas_[groupName];

	// 指定グループに指定のキーが存在しない場合エラー
	assert(group.items.find(key) != group.items.end());

	// 指定グループから指定のキーの値を取得
	return std::get<Vector3>(group.items[key].value);
}

std::string GlobalVariables::GetStringValue(const std::string& groupName, const std::string& key)
{
	// 指定グループがいない場合エラー
	assert(datas_.find(groupName) != datas_.end());

	// グループの参照を取得する
	Group& group = datas_[groupName];

	// 指定グループに指定のキーが存在しない場合エラー
	assert(group.items.find(key) != group.items.end());

	// 指定グループから指定のキーの値を取得
	return std::get<std::string>(group.items[key].value);
}
