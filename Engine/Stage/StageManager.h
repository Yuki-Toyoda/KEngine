#pragma once
#include "BaseStage.h"
#include <memory>

class Item;

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

	/// <summary>
	/// ステージの数を取得
	/// </summary>
	size_t GetStageMaxIndex() { return infos_.size(); }

	/// <summary>
	/// アイテムの情報を取得
	/// </summary>
	std::vector<Item*>& GetItems() { return items_; }

	/// <summary>
	/// マネージャーにステージを登録する
	/// </summary>
	void AddStageInfo(const BaseStage::StageInfo& info);

	/// <summary>
	/// アイテムが配置されているか取得する
	/// </summary>
	const int& GetItemMax()const { return kMaxItem_; }

	/// <summary>
	/// アイテムがいくつ使われているかを取得する
	/// </summary>
	int GetUsedItem()const;

	/// <summary>
	/// クリアしているか判定
	/// </summary>
	bool GetIsClear() const { return currentStage_->GetIsClear(); };

	/// <summary>
	/// ゲームを開始してからの経過時間
	/// </summary>
	int GetGameTIme() const { return gameTime_; }

private: // メンバ関数

	/// <summary>
	/// 設定を作る
	/// </summary>
	void AddGloavalVariables();

	/// <summary>
	/// ステージの情報を読み込む
	/// </summary>
	void LoadStages();

	BaseStage::StageInfo LoadInfo(size_t num);

	BaseStage::ItemInfo LoadItem(const std::string& indexNum, size_t i);

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

	// ステージの経過時間
	int gameTime_ = 0;

	// 今選んでいるステージの添え字
	int nowStageNum_ = 0;

#ifdef _DEBUG

	int nextStageNum_ = 0;


	int commitIndex_ = 0;
	int loadIndex_ = 0;


#endif // _DEBUG

};

/*
* ・もしかしたら GlobalVariables の方でステージ内容読み込むかも
* ・いまはエンジン作ってる人が放置してるから放置
*/