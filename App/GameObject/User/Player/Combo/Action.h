#pragma once
#include <string>
#include "Engine/GlobalVariables/GlobalVariables.h"
#include "Engine/Utility/Timer/DeltaTimer.h"
#include "Engine/Input/Input.h"

// クラスの前方宣言
class Player;
class ComboManager;

/// <summary>
/// コンボ１つ１つのデータクラス
/// </summary>
class Action
{
public: // コンストラクタ等

	// <summary>
	/// コンストラクタ
	/// </summary>
	/// <param name="comboName">読み込まれるコンボ名</param>
	/// <param name="name">読み込まれるアクション名</param>
	/// <param name="count">何番目のアクションか</param>
	Action(const std::string& comboName, const std::string& actionName, const int32_t count);

	/// <summary>
	/// デストラクタ
	/// </summary>
	~Action() = default;

public: // メンバ関数

	/// <summary>
	/// 初期化関数
	/// </summary>
	/// <param name="manager">コンボマネージャー本体</param>
	void Init(ComboManager* manager);

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
	/// パラメーターセット関数
	/// </summary>
	void SetValue();

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
	/// コンボ名セッター
	/// </summary>
	/// <param name="comboName">コンボ名</param>
	void SetComboName(const std::string& comboName) { comboName_ = comboName; };

	/// <summary>
	/// アクション名ゲッター
	/// </summary>
	/// <returns>アクション名</returns>
	std::string GetActionName() { return name_; }

	/// <summary>
	/// 何コンボ目のアクションかのセッター
	/// </summary>
	/// <param name="count">何コンボ目のアクションにするか</param>
	void SetComboCount(const int32_t count) { comboCount_ = count; }
	/// <summary>
	/// 何コンボ目のアクションかのゲッター
	/// </summary>
	/// <returns>何コンボ目のアクションか</returns>
	int32_t GetComboCount() { return comboCount_; }

	/// <summary>
	/// ダメージ量ゲッター
	/// </summary>
	/// <returns>ダメージ量</returns>
	int32_t GetDamage() { return damage_; };

	/// <summary>
	/// ヒットストップ秒数ゲッター
	/// </summary>
	/// <returns>ヒットストップの秒数</returns>
	float GetHitStopTime() { return hitStopTime_; }

	/// <summary>
	/// 移動した際に該当アクションを終了するか
	/// </summary>
	/// <returns>フラグ状態</returns>
	bool GetIsActionEndFromMove() { return isActionEndFromMove_; }

	/// <summary>
	/// 遷移可能状態ゲッター
	/// </summary>
	/// <returns>遷移可能か</returns>
	bool GetIsTransitionReady() { return isTransitionReady_; }

	/// <summary>
	/// 次のアクション移行フラグゲッター
	/// </summary>
	/// <returns>次のアクションに移行するか</returns>
	bool GetIsGoNextAciton() { return isGoNextAction_; }

	/// <summary>
	/// コンボ変更フラグゲッター
	/// </summary>
	/// <returns>コンボ変更を行うか</returns>
	bool GetIsChangeCombo() { return isChangeCombo_; }

	/// <summary>
	/// 次のコンボ名ゲッター
	/// </summary>
	/// <returns>次のコンボ名</returns>
	std::string GetNextComboName() { return nextComboName_; }

	/// <summary>
	/// アクションの終了状態ゲッター
	/// </summary>
	/// <returns>終了しているか</returns>
	bool GetIsActionEnd() { return isEndAction_; }

private: // 機能関数群

	/// <summary>
	/// アニメーションが終了しているか検証する関数
	/// </summary>
	void AnimationCheck();

	/// <summary>
	/// 攻撃判定更新関数
	/// </summary>
	void AttackJudgeUpdate();

	/// <summary>
	/// スタン確認関数
	/// </summary>
	void StunCheck();

	/// <summary>
	/// 次のコンボに移る条件を満たしているか検証する関数
	/// </summary>
	void CheckCondition();

private: // メンバ変数

	// コンボ名
	std::string comboName_ = "";
	// アクション名
	std::string name_ = "Action";

	// 何コンボ目のアクションか
	int32_t comboCount_ = 0;

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
	// スタン終了フラグ
	bool stunEnd_ = false;

	// 受付時間タイマー
	KLib::DeltaTimer acceptTimer_{};
	// 次のコンボ受付時間
	float acceptTime_ = 0.5f;

	// 行動中に攻撃判定が存在するか
	bool isAttack_ = false;
	// このアクションが命中した際に与えるダメージ
	int32_t damage_ = 1;
	// 攻撃判定の長さ
	float attackLength_ = 1.0f;

	// 攻撃開始秒数
	float attackStartTime_ = 0.0f;
	// 攻撃開始時間タイマー
	KLib::DeltaTimer attackStartTimer_{};
	// 攻撃開始フラグ
	bool attackStart_ = true;
	// 攻撃終了秒数
	float attackEndTime_ = 0.5f;
	// 攻撃終了時間タイマー
	KLib::DeltaTimer attackEndTimer_{};

	// ヒットストップ秒数
	float hitStopTime_ = 0.0f;

	// 移動した際にアクション終了するか
	bool isActionEndFromMove_ = false;

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

	// アクション名
	char imGuiActionName_[64] = "";
	// 再生アニメーション名
	char imGuiAnimName_[64] = "";
	// 特殊条件名
	char imGuiSpecialConditionName_[64] = "";
	// 次のコンボ名
	char imGuiNextComboName_[64] = "";

	int32_t imGuiInputCondition_ = TRIGGER;

	#pragma endregion

};

