#pragma once
#include <Windows.h>
#include "../../../../Externals/nlohmann/json.hpp"
#include <variant>
#include <map>
#include <string>
#include <fstream>
#include "../../../Math/Math.h"

#ifdef _DEBUG
#include "../../../../externals/imgui/imgui.h"
#endif // _DEBUG

struct HierarchicalName
{
    std::string kGroup;
    std::string kSection;
};

class GameDataManager
{
public:
    static GameDataManager* GetInstance() {
        static GameDataManager instance;
        return &instance;
    }

private:
    GameDataManager() = default;
    ~GameDataManager() = default;
    GameDataManager(const GameDataManager& variable) = default;
    GameDataManager& operator=(const GameDataManager& variable);

public:
    /// <summary>
    /// 全ファイルロード
    /// </summary>
    void LoadFiles();

    /// <summary>
    /// 指定ファイルロード
    /// </summary>
    /// <param name="groupName"></param>
    void LoadFile(const std::string& groupName);

    /// <summary>
    /// 一つのグループを保存する関数
    /// ※データを書き換える為情報はまとめて保存すること
    /// </summary>
    /// <param name="groupName">kLevelNameから選択</param>
    void SaveData(const std::string& groupName/*, int32_t groupNum*/);

    /// <summary>
    /// グループ作成
    /// </summary>
    /// <param name="names"></param>
    void CreateGroup(const HierarchicalName names) {
        gameDatas_[names.kGroup][names.kSection];
    }

    /// <summary>
    /// データクリア
    /// </summary>
    void ClearGroup() {
        gameDatas_.clear();
    }

private: // 配列の最大サイズ
    static const int kEnemyMaxValue = 5;
    static const int kObstacleMaxValue = 3;

public: // 呼び出し時に使用する配列
    // ステージごとの名前
    std::array<std::string, 4> kLevelNames = { "Easy","Normal","Hard","Expert" };
    // 内部に保存しているオブジェクトの名前
    std::array<std::string, 3> kObjectNames = { "Enemy","Obstacle","Meteor" };

    // 保存するアイテム名
    std::array<std::string, kEnemyMaxValue> kEnemyItems = { "Position","Type","Speed","RespownTime", "MaxCount" };
    // 障害物用
    std::array<std::string, kObstacleMaxValue> kObstacleItems = { "Position","Size", "MaxCount" };

private:
    /// 項目
    using Item = std::variant<int32_t, float, Vector2, Vector3, std::string>;
    using Section = std::map<std::string, Item>;
    using Group = std::map<std::string, Section>;

    // 現在の設定中の番号
    int kLoadObjectNumber_ = 0;

    // 全データ
    std::map<std::string, Group> gameDatas_;

    // 保存先のファイルパス
    const std::string kDirectoryPath = "Resources/StageData/";

    // ImGuiの最大値
    const float kFabsValue_f = 2000.0f;
    const int kFabsValue_i = 2000;

public:
#pragma region 設定
    void SetValue(const HierarchicalName& names, const std::string& key, int32_t value);
    void SetValue(const HierarchicalName& names, const std::string& key, float value);
    void SetValue(const HierarchicalName& names, const std::string& key, Vector2 value);
    void SetValue(const HierarchicalName& names, const std::string& key, Vector3 value);
    void SetValue(const HierarchicalName& names, const std::string& key, std::string value);
#pragma endregion

#pragma region 追加
    void AddItem(const HierarchicalName& names, const std::string& key, int32_t value);
    void AddItem(const HierarchicalName& names, const std::string& key, float value);
    void AddItem(const HierarchicalName& names, const std::string& key, Vector2 value);
    void AddItem(const HierarchicalName& names, const std::string& key, Vector3 value);
    void AddItem(const HierarchicalName& names, const std::string& key, std::string value);
#pragma endregion

#pragma region 取得
    int GetIntValue(const HierarchicalName& names, const std::string& key);
    float GetFloatValue(const HierarchicalName& names, const std::string& key);
    Vector2 GetVector2Value(const HierarchicalName& names, const std::string& key);
    Vector3 GetVector3Value(const HierarchicalName& names, const std::string& key);
    std::string GetStringValue(const HierarchicalName& names, const std::string& key);
#pragma endregion

};

