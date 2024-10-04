#pragma once
#include <Windows.h>
#include "Externals/nlohmann/json.hpp"
#include <variant>
#include <map>
#include <string>
#include <fstream>
#include "Engine/Math/Vector2.h"
#include "Engine/Math/Vector3.h"

#ifdef _DEBUG
#include "Externals/imgui/imgui.h"
#endif // _DEBUG

/// <summary>
/// 調整項目クラス
/// </summary>
class GlobalVariables {
private: // コンストラクタ等を無効

	/// <summary>
	/// コンストラクタ
	/// </summary>
	GlobalVariables();
	/// <summary>
	/// デストラクタ
	/// </summary>
	~GlobalVariables();

public: // コピーコンストラクタ、代入演算子無効

	// コピーコンストラクタ無効
	GlobalVariables(const GlobalVariables& obj) = delete;
	// 代入演算子無効
	GlobalVariables& operator=(const GlobalVariables& obj) = delete;

public: // メンバ関数

	/// <summary>
	/// インスタンス取得
	/// </summary>
	/// <returns>インスタンス</returns>
	static GlobalVariables* GetInstance();

	/// <summary>
	/// 更新処理
	/// </summary>
	void Update();

	/// <summary>
	/// グループの作成関数
	/// </summary>
	/// <param name="groupName">グループ名</param>
	void CreateGroup(const std::string& groupName);

	/// <summary>
	/// ファイルに書き出しを行う関数
	/// </summary>
	/// <param name="groupName">保存するグループ</param>
	void SaveFile(const std::string& groupName);
	/// <summary>
	/// ディレクトリ内の全ファイルを読み込む
	/// </summary>
	void LoadFiles();
	/// <summary>
	/// ファイルを読み込む
	/// </summary>
	/// <param name="groupName">読み込むグループ</param>
	void LoadFile(const std::string& groupName);

	/// <summary>
	/// 値のセット関数（int）
	/// </summary>
	/// <param name="groupName">セットするグループ</param>
	/// <param name="key">セットするキー</param>
	/// <param name="value">セットする値</param>
	void SetValue(const std::string& groupName, const std::string& key, int32_t value);
	/// <summary>
	/// 値のセット関数（float）
	/// </summary>
	/// <param name="groupName">セットするグループ</param>
	/// <param name="key">セットするキー</param>
	/// <param name="value">セットする値</param>
	void SetValue(const std::string& groupName, const std::string& key, float value);
	/// <summary>
	/// 値のセット関数（Vector2）
	/// </summary>
	/// <param name="groupName">セットするグループ</param>
	/// <param name="key">セットするキー</param>
	/// <param name="value">セットする値</param>
	void SetValue(const std::string& groupName, const std::string& key, const Vector2& value);
	/// <summary>
	/// 値のセット関数（Vector3）
	/// </summary>
	/// <param name="groupName">セットするグループ</param>
	/// <param name="key">セットするキー</param>
	/// <param name="value">セットする値</param>
	void SetValue(const std::string& groupName, const std::string& key, const Vector3& value);

	/// <summary>
	/// 項目追加関数(int)
	/// </summary>
	/// <param name="groupName">追加する対象のグループ</param>
	/// <param name="key">追加するキー</param>
	/// <param name="value">追加する値</param>
	void AddItem(const std::string& groupName, const std::string& key, int32_t value);
	/// <summary>
	/// 項目追加関数(float)
	/// </summary>
	/// <param name="groupName">追加する対象のグループ</param>
	/// <param name="key">追加するキー</param>
	/// <param name="value">追加する値</param>
	void AddItem(const std::string& groupName, const std::string& key, float value);
	/// <summary>
	/// 項目追加関数(Vector2)
	/// </summary>
	/// <param name="groupName">追加する対象のグループ</param>
	/// <param name="key">追加するキー</param>
	/// <param name="value">追加する値</param>
	void AddItem(const std::string& groupName, const std::string& key, Vector2 value);
	/// <summary>
	/// 項目追加関数(Vector3)
	/// </summary>
	/// <param name="groupName">追加する対象のグループ</param>
	/// <param name="key">追加するキー</param>
	/// <param name="value">追加する値</param>
	void AddItem(const std::string& groupName, const std::string& key, Vector3 value);

	/// <summary>
	/// 値を取得する関数(int)
	/// </summary>
	/// <param name="groupName">取得するグループ名</param>
	/// <param name="key">取得するキー</param>
	/// <returns>値</returns>
	int32_t GetIntValue(const std::string& groupName, const std::string& key);
	/// <summary>
	/// 値を取得する関数(float)
	/// </summary>
	/// <param name="groupName">取得するグループ名</param>
	/// <param name="key">取得するキー</param>
	/// <returns>値</returns>
	float GetFloatValue(const std::string& groupName, const std::string& key);
	/// <summary>
	/// 値を取得する関数(Vector2)
	/// </summary>
	/// <param name="groupName">取得するグループ名</param>
	/// <param name="key">取得するキー</param>
	/// <returns>値</returns>
	Vector2 GetVector2Value(const std::string& groupName, const std::string& key);
	/// <summary>
	/// 値を取得する関数(Vector3)
	/// </summary>
	/// <param name="groupName">取得するグループ名</param>
	/// <param name="key">取得するキー</param>
	/// <returns>値</returns>
	Vector3 GetVector3Value(const std::string& groupName, const std::string& key);

private: // メンバ変数

	// 項目列挙子
	struct Item {
		// 項目の値
		std::variant<int32_t, float, Vector2, Vector3> value;
	};

	// グループ列挙子
	struct Group {
		std::map<std::string, Item> items;
	};

	// 全データ
	std::map<std::string, Group> datas_;

	// グローバル変数の保存先ファイルパス
	const std::string kDirectorypath = "Resources/GlobalVariables/";

};
