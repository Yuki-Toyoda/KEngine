#pragma once
#include <string>
#include "Engine/GlobalVariables/GlobalVariables.h"
#include "Engine/Utility/Timer/DeltaTimer.h"
#include "Engine/Input/Input.h"

// クラスの前方宣言
class Player;

/// <summary>
/// コンボ１つ１つのデータクラス
/// </summary>
class Action
{
public: // コンストラクタ等

	/// <summary>
	/// コンストラクタ
	/// </summary>
	/// <param name="comboName">読み込まれるコンボ名</param>
	/// <param name="name">読み込まれるアクション名</param>
	Action(const std::string& comboName, const std::string& actionName);

	/// <summary>
	/// デストラクタ
	/// </summary>
	~Action() = default;

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
	/// パラメーター追加関数
	/// </summary>
	void AddParam();

	/// <summary>
	/// パラメーター適用関数
	/// </summary>
	void ApplyParam();

public: // アクセッサ等

	/// <summary>
	/// プレイヤーセッター
	/// </summary>
	/// <param name="player">プレイヤー本体</param>
	void SetPlayer(Player* player) { player_ = player; }

	/// <summary>
	/// 遷移可能状態ゲッター
	/// </summary>
	/// <returns>遷移可能か</returns>
	bool GetIsTransitionReady() { return isTransitionReady_; }

	/// <summary>
	/// アクションの終了状態ゲッター
	/// </summary>
	/// <returns>終了しているか</returns>
	bool GetIsActionEnd() { return isEndAction_; }

private: // 機能関数群
	
	/// <summary>
	/// 次のコンボに移る条件を満たしているか検証する関数
	/// </summary>
	void CheckCondition();

private: // メンバ変数

	// コンボ名
	std::string comboName_ = "";
	// アクション名
	std::string name_ = "";

	// プレイヤー
	Player* player_ = nullptr;
	// コンボ中再生されるアニメーション名
	std::string animName_ = "";
	// アニメーション終了した時に遷移可能にするかのフラグ
	bool isStandByfromAnimEnd_ = false;

	// 硬直時間タイマー
	KLib::DeltaTimer stunTimer_{};
	// 硬直時間
	float stunTime_ = 0.1f;

	// 受付時間タイマー
	KLib::DeltaTimer acceptTimer_{};
	// 次のコンボ受付時間
	float acceptTime_ = 0.65f;

	// 行動中に攻撃判定が存在するか
	bool isAttack_ = false;
	// このアクションが命中した際に与えるダメージ
	int32_t damage_ = 1;
	// 攻撃判定の長さ
	float attackLength_ = 1.0f;

	// 次のコンボに移るボタンの条件
	int32_t inputCondition_ = TRIGGER;

	// 遷移準備完了フラグ
	bool isTransitionReady_ = false;

	// 次のアクションに移るかフラグ
	bool isGoNextAction_ = false;

	// 特殊条件名
	std::string specialConditionName_ = "";
	// コンボ変更時の特殊条件
	bool* specialCondition_ = nullptr;
	// コンボ変更フラグ
	bool isChangeCombo_ = false;
	// 次に遷移するコンボ名
	std::string nextComboName_ = "";

	// アクション終了フラグ
	bool isEndAction_ = false;

	#pragma region ImGui用変数

	int32_t imGuiInputCondition_ = TRIGGER;

	#pragma endregion

};

