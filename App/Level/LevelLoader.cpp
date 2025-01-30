#include "LevelLoader.h"
#include "Engine/GameObject/SampleList.h"
#include "Engine/Utility/Angle/Angle.h"

void LevelLoader::Load(SampleLevelObjects* objects, const std::string& filePath, const std::string fileName)
{
	// ファイルまでのフルパスを求める
	std::string fullPath = filePath + "/" + fileName;

	// ファイル
	std::ifstream file;
	// ファイルを開く
	file.open(fullPath);
	// ファイルを開けなかった場合停止
	if (file.fail()) {
		assert(0);
	}

	// JSON文字列から解凍したデータ
	nlohmann::json deserialized;

	// 解凍
	file >> deserialized;

	// 正しいレベルデータファイルかチェック
	assert(deserialized.is_object());
	assert(deserialized.contains("name"));
	assert(deserialized["name"].is_string());

	// "name"を文字列として取得
	std::string name = deserialized["name"].get<std::string>();
	// 正しいレベルデータファイルかチェック
	assert(name.compare("scene") == 0);

	// "Objects" の全オブジェクトを走査する
	for (nlohmann::json& object : deserialized["objects"]) {
		assert(object.contains("type"));

		// 種類を取得
		std::string type = object["type"].get<std::string>();

		// ファイル名格納用
		std::string objName;

		// 種類ごとの処理
		if (type.compare("MESH") == 0) {
			// ファイル名を求める
			if (object.contains("file_name")) {
				objName = object["file_name"];
			}

			// トランスフォームパラメーター読み込み
			WorldTransform t; // 読み込み格納用

			nlohmann::json& transform = object["transform"];

			// 平行移動
			t.translate_.x = (float)transform["translation"][0];
			t.translate_.y = (float)transform["translation"][2];
			t.translate_.z = (float)transform["translation"][1];
			// 回転角
			t.rotate_.x = -(float)transform["rotation"][0];
			t.rotate_.y = -(float)transform["rotation"][2];
			t.rotate_.z = -(float)transform["rotation"][1];
			t.rotate_.x = KLib::degreeToRadian(t.rotate_.x);
			t.rotate_.y = KLib::degreeToRadian(t.rotate_.y);
			t.rotate_.z = KLib::degreeToRadian(t.rotate_.z);
			// スケール
			t.scale_.x = (float)transform["scaling"][0];
			t.scale_.y = (float)transform["scaling"][2];
			t.scale_.z = (float)transform["scaling"][1];

			// レベルオブジェクトにオブジェクト追加
			objects->AddObjects(filePath, objName, t);
		}
	}
}
