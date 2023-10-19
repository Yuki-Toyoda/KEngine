#pragma once
#include "BaseStage.h"
#include <memory>


/// <summary>
/// 全てのステージの管理マネージャ
/// </summary>
class StageManager
{
private: // コンストラクタ等

	// コピーコンストラクタ、代入演算子無効化
	StageManager() = default;
	~StageManager() = default;
	StageManager(const StageManager&) = delete;
	StageManager& operator=(const StageManager&) = delete;

public: // メンバ関数

	/// <summary>
	/// インスタンス取得関数
	/// </summary>
	/// <returns>インスタンス</returns>
	static StageManager* GetInstance();

	/// <summary>
	/// 初期化関数
	/// </summary>
	void Initialize();
	
	/// <summary>
	/// リセット
	/// </summary>
	void Reset();

	/// <summary>
	/// 更新関数
	/// </summary>
	void Update();

	// ステージを設定
	void SetStage(size_t commitIndex);

	size_t GetStageMaxIndex() { return infos_.size(); }

	std::vector<Item*>& GetItems() { return items_; }

	void AddStageInfo(const BaseStage::StageInfo& info);

	const int& GetItemMax() { return kMaxItem_; }

private: // メンバ関数

	void AddlyGloavalVariables();

	/// <summary>
	/// ステージの情報を読み込む
	/// </summary>
	void LoadStages();

	BaseStage::StageInfo LoadInfo(size_t num);


	void SaveStages();

	void SaveInfo(size_t num);

	// デバッグ用GUIを出す
	void DebugGUI();


private: // メンバ変数

	GlobalVariables* globalVariables_ = nullptr;

	// 現在のシーン
	BaseStage* currentStage_;

	// アイテムの最大数
	static const int kMaxItem_ = 10;

	// 使用されるアイテムを保存
	std::vector<Item*> items_;

	// 計算する
	int32_t kMaxStageNum_ = 0;

	// ステージ毎の情報
	std::vector<BaseStage::StageInfo> infos_;

	int nowStageNum_ = 0;
	int nextStageNum_ = 0;

	int commitIndex_ = 0;
	int loadIndex_ = 0;
};

/*
* ・もしかしたら GlobalVariables の方でステージ内容読み込むかも
* ・いまはエンジン作ってる人が放置してるから放置
*/