#pragma once
#include "BaseStage.h"
#include <memory>


/// <summary>
/// 全てのシーンの管理マネージャ
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
	/// 更新関数
	/// </summary>
	void Update();

	// ステージを設定
	void SetStage(BaseStage* stage);

private: // メンバ関数

	/// <summary>
	/// ステージの情報を読み込む
	/// </summary>
	void LoadStage();

private: // メンバ変数

	// 現在のシーン
	BaseStage* currentStage_;

	// アイテムの最大数
	static const int kMaxItem_ = 10;

	// 使用されるアイテムを保存
	std::vector<std::unique_ptr<Item>> items_;

};

/*
* ・もしかしたら GlobalVariables の方でステージ内容読み込むかも
* ・いまはエンジン作ってる人が放置してるから放置
*/