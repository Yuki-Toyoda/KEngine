#pragma once
#include "Action.h"
#include <list>

/// <summary>
/// コンボクラス
/// </summary>
class Combo
{
public: // コンストラクタ等

	/// <summary>
	/// コンストラクタ
	/// </summary>
	/// <param name="name">読み込むコンボ名称</param>
	Combo(const std::string& name);
	/// <summary>
	/// デストラクタ
	/// </summary>
	virtual ~Combo() = default;

public: // メンバ関数

	/// <summary>
	/// 初期化関数
	/// </summary>
	void Init();

	/// <summary>
	/// 更新関数
	/// </summary>
	void Update();

	/// <summary>
	/// ImGui表示関数
	/// </summary>
	void DisplayImGui();

	/// <summary>
	/// パラメータ追加関数
	/// </summary>
	void AddParam();

	/// <summary>
	/// パラメータ適用関数
	/// </summary>
	void ApplyParam();

public: // アクセッサ等

	/// <summary>
	/// プレイヤーセッター
	/// </summary>
	/// <param name="player">プレイヤー</param>
	void SetPlayer(Player* player) { player_ = player; }

	/// <summary>
	/// 現在のコンボ数ゲッター
	/// </summary>
	/// <returns>現在のコンボ数</returns>
	int32_t GetComboCount() { return comboCount_; }

	/// <summary>
	/// コンボの終了状態ゲッター
	/// </summary>
	/// <returns>終了しているか</returns>
	bool GetIsComboEnd() { return isComboEnd_; }

private: // プライベートメンバ関数

	/// <summary>
	/// アクション追加関数
	/// </summary>
	/// <param name="actionName">追加するアクション名</param>
	void AddAction(const std::string& actionName);

	/// <summary>
	/// アクション削除関数
	/// </summary>
	/// <param name="actionName">削除するアクション名</param>
	void DeleteAction(const std::string& actionName);

private: // メンバ変数

	// グローバル変数クラス上の識別用ID
	std::string comboID_ = "Combo 0";
	// コンボ名
	std::string name_ = "Combo";

	// プレイヤー本体
	Player* player_ = nullptr;

	// コンボ一つ一つのアクションを格納する配列
	std::list<Action> actions_{};

	// 現在行われているコンボ数
	int32_t comboCount_ = 0;

	// コンボの終了フラグ
	bool isComboEnd_ = false;
};

