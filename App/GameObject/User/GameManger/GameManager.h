#pragma once
#include "Engine/GameObject/IObject.h"
#include "Engine/Utility/Animation/AnimationManager.h"

// クラスの前方宣言
class Camera;
class Player;
class Enemy;

/// <summary>
/// ゲームシーンのマネージャー
/// </summary>
class GameManager : public IObject
{
public: // サブクラス

	// フェード状態列挙子
	enum FadeState {
		FADEIN, // フェードイン
		FADEOUT // フェードアウト
	};

public: // メンバ関数

	/// <summary>
	/// 初期化関数
	/// </summary>
	void Init() override;

	/// <summary>
	/// 更新関数
	/// </summary>
	void Update() override;

	// ImGui表示関数
	// ImGuiを表示させたい場合はこの関数に処理を追記
	void DisplayImGui() override;

	/// <summary>
	/// フェード演出開始関数
	/// </summary>
	/// <param name="fadeState">フェードイン、アウトを選択</param>
	/// <param name="fadeTime">何秒かけてフェード演出をするか</param>
	void StartFade(const int fadeState, const float fadeTime);

public: // アクセッサ等

	/// <summary>
	/// プレイヤーセッター
	/// </summary>
	/// <param name="player">プレイヤー</param>
	void SetPlayer(Player* player) { player_ = player; }

	/// <summary>
	/// 敵セッター
	/// </summary>
	/// <param name="enemy">敵</param>
	void SetEnemy(Enemy* enemy) { enemy_ = enemy; }

	/// <summary>
	/// ゲーム開始状態のゲッター
	/// </summary>
	/// <returns>ゲームが開始されているか</returns>
	bool GetIsGameStart() { return isGameStart_; }

	/// <summary>
	/// ゲーム終了状態ゲッター
	/// </summary>
	/// <returns>ゲームが終了しているか</returns>
	bool GetIsGameEnd() { return isGameEnd_; }
	/// <summary>
	/// ゲーム終了状態セッター
	/// </summary>
	/// <param name="isGameEnd">ゲーム終了状態</param>
	void SetIsGameEnd(const bool isGameEnd) { isGameEnd_ = isGameEnd; }

	/// <summary>
	/// フェード演出スプライトの値を直接指定するセッター
	/// </summary>
	/// <param name="fadeAlpha"></param>
	void SetFade(const float fadeAlpha);

	/// <summary>
	/// フェード演出実行状態ゲッター
	/// </summary>
	/// <returns>フェード演出の実行状態</returns>
	bool GetIsFadeStaging() { return isFadeStaging_; }

public: // デバッグ機能用アクセッサ

	/// <summary>
	/// デバッグ用のタイムスケールゲッター
	/// </summary>
	/// <returns>デバッグ用タイムスケール</returns>
	float GetDebugTimeScale() { return d_TimeScale_; }

private: // 機能関数群

	/// <summary>
	/// タイトル演出更新関数
	/// </summary>
	void TitleStagingUpdate();

	/// <summary>
	/// タイトル演出パラメータを作成する
	/// </summary>
	/// <param name="name">作成するパラメータ名</param>
	void CreateTitleCameraParameter(const std::string& name);

	/// <summary>
	/// ゲーム状態の更新関数
	/// </summary>
	void GameStateUpdate();

	/// <summary>
	/// フェード演出更新関数
	/// </summary>
	void FadeUpdate();

private: // メンバ変数

	// 入力検知用
	Input* input_ = nullptr;

	// プレイヤー
	Player* player_ = nullptr;
	// 敵
	Enemy* enemy_ = nullptr;

	// ゲーム開始フラグ
	bool isGameStart_ = false;
	// ゲーム終了フラグ
	bool isGameEnd_ = false;

	// タイトル演出用カメラ
	Camera* titleCamera_ = nullptr;
	// アニメーションマネージャ
	AnimationManager* animManager_;
	// タイトル演出アニメーション
	MyAnimation* titleAnim_ = nullptr;
	// タイトル演出中フラグ
	bool isGameStarting_ = false;

	// ゲーム終了演出フラグ
	bool isEndStaging_ = false;

	// フェード演出中フラグ
	bool isFadeStaging_ = false;
	// フェード管理フラグ
	int fadeState_ = FADEOUT;
	// フェード演出開始時の透明度
	float currentFadeAlpha_ = 0.0f;
	// フェード演出用タイマー
	KLib::DeltaTimer fadeTimer_{};

#pragma region デバッグ用変数

	// デバッグ状態の有効化フラグ
	bool enableDebug_ = false;
	// デバッグ用タイムスケール
	float d_TimeScale_ = 1.0f;

#pragma endregion

	// デバッグ用変数群
};

