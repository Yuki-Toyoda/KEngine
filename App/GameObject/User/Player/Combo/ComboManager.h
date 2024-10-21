#pragma once
#include "Combo.h"

/// <summary>
/// コンボ管理マネージャー
/// </summary>
class ComboManager
{
public: // コンストラクタ等

	/// <summary>
	/// コンストラクタ
	/// </summary>
	ComboManager() = default;

	/// <summary>
	/// デストラクタ
	/// </summary>
	~ComboManager() = default;

public: // メンバ関数

	/// <summary>
	/// 初期化関数
	/// </summary>
	/// <param name="player">プレイヤー</param>
	void Init(Player* player);

	/// <summary>
	/// 更新関数
	/// </summary>
	void Update();

	/// <summary>
	/// ImGui表示関数
	/// </summary>
	void DisplayImGui();

public: // 機能関数群

	/// <summary>
	/// 再生するコンボを変更する関数
	/// </summary>
	/// <param name="comboName"></param>
	void ChangeCombo(const std::string& comboName);

public: // アクセッサ等

	/// <summary>
	/// 遷移可能状態かのゲッター
	/// </summary>
	/// <returns>遷移可能状態か</returns>
	bool GetIsTransitonReady() { return combo_.GetIsTransitionReady(); }

	/// <summary>
	/// コンボが終了しているかのゲッター
	/// </summary>
	/// <returns>コンボが終了しているか</returns>
	bool GetIsComboEnd() { return combo_.GetIsComboEnd(); }

private: // デバッグ機能関数群

	/// <summary>
	/// コンボゲッター
	/// </summary>
	/// <param name="name">取得するコンボ名称</param>
	/// <returns>コンボ</returns>
	Combo* GetCombo(const std::string& name);

	/// <summary>
	/// コンボ追加関数
	/// </summary>
	/// <param name="comboID">コンボID</param>
	/// <param name="comboName">追加するコンボ名</param>
	void AddCombo(const std::string& comboID, const std::string& comboName = "Combo");

	/// <summary>
	/// コンボ削除関数
	/// </summary>
	/// <param name="name">削除するコンボ名</param>
	void DeleteCombo(const std::string& name);

	/// <summary>
	/// パラメーター追加関数
	/// </summary>
	void AddParam();

	/// <summary>
	/// パラメーターセット関数
	/// </summary>
	void SetValue();

	/// <summary>
	/// パラメーター適用関数
	/// </summary>
	void ApplyParam();

	/// <summary>
	/// ファイルメニュー関数
	/// </summary>
	void FileMenu();

	/// <summary>
	/// コンボメニュー関数
	/// </summary>
	void ComboMenu();

private: // メンバ変数

	// プレイヤー本体
	Player* player_ = nullptr;

	// 再生するコンボ
	Combo combo_ = Combo("");

	// コンボ配列
	std::list<Combo> combos_;

	#pragma region ImGui用変数

	// ImGuiを表示するコンボ名
	std::string imGuiDisplayComboName_ = "";

	#pragma endregion
};

