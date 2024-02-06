#pragma once
#include "../../BaseObject.h"
#include "../../../Input/Input.h"
#include "../../../Utility/Animation/AnimationManager.h"
#include "../../../Audio/Audio.h"

// クラスの前方宣言
class Camera;
class PlayerAnimManager;
class SkyDome;
class FadeManager;

/// <summary>
/// クリア画面マネージャ
/// </summary>
class ResultManager : public BaseObject
{
public: // メンバ関数

	/// <summary>
	/// 初期化関数
	/// </summary>
	void Init() override;

	/// <summary>
	/// 更新関数
	/// </summary>
	void Update() override;

	/// <summary>
	/// ImGui表示関数
	/// </summary>
	void DisplayImGui() override;

	/// <summary>
	/// 初期化後関数
	/// </summary>
	/// <param name="isClear">クリアしたか</param>
	void PostInit(bool isClear);

	/// <summary>
	/// パラメータ生成関数
	/// </summary>
	/// <param name="name">作成するパラメータ名</param>
	/// <param name="isClear">クリアフラグ</param>
	void CreateParameter(const std::string& name);

public: // アクセッサ等

	/// <summary>
	/// カメラセッター
	/// </summary>
	/// <param name="camera">カメラ</param>
	void SetCamera(Camera* camera) { camera_ = camera; }

	/// <summary>
	/// プレイヤーのモデルセッター
	/// </summary>
	/// <param name="pam">プレイヤーのモデル</param>
	void SetPlayerAnimManager(PlayerAnimManager* pam) { pam_ = pam; }

	/// <summary>
	/// 天球セッター
	/// </summary>
	/// <param name="skyDome">天球</param>
	void SetSkyDome(SkyDome* skyDome);

	/// <summary>
	/// シーン遷移トリガー
	/// </summary>
	/// <returns>シーン遷移を行うか</returns>
	bool GetIsSceneChange() { return isSceneChange_; }

	/// <summary>
	/// リトライトリガー
	/// </summary>
	/// <returns>リトライ状態</returns>
	bool GetIsRetry() { return isRetry_; }

private: // その他関数群

	void CreateClearAnimation();

private: // メンバ変数

	// 音再生クラス
	Audio* audio_ = nullptr;

	// BGM
	int32_t bgmHandle_ = 0u;
	int bgmVoiceHadle_ = -1;

	// 決定音
	int32_t decisionSE_ = 0u;

	int32_t selectSE = 0u;
	// 入力検知用
	Input* input_ = nullptr;
	// コントローラー入力
	XINPUT_STATE joyState_; // 現在フレーム用
	XINPUT_STATE preJoyState_; // 前フレーム用

	// フェード演出マネージャ
	FadeManager* fadeManager_ = nullptr;

	// クリアフラグ格納用
	bool isCleared_ = false;

#pragma region 共用

	// カメラ
	Camera* camera_ = nullptr;

	// プレイヤーのモデル
	PlayerAnimManager* pam_ = nullptr;

	// 天球モデル
	SkyDome* skyDome_ = nullptr;

	// アニメーション
	AnimationManager* animManager_ = nullptr;
	// アニメーション
	Animation* anim_ = nullptr;

	// リトライトリガー
	bool isRetry_ = false;

	// フェードトリガー
	bool isFade_ = false;
	// シーンチェンジトリガー
	bool isSceneChange_ = false;

#pragma endregion

#pragma region クリア用

	// 野菜の山用トランスフォーム
	WorldTransform vegetablesTransform_;

#pragma endregion

#pragma region 失敗用

#pragma endregion

};

