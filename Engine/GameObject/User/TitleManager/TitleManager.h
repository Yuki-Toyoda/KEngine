#pragma once
#include "../../BaseObject.h"
#include "../../../Input/Input.h"
#include "../../../Utility/Animation/AnimationManager.h"
#include "../../../Audio/Audio.h"

// クラスの前方宣言
class FadeManager;
class Camera;
class PlayerAnimManager;

/// <summary>
/// タイトル画面マネージャ
/// </summary>
class TitleManager : public BaseObject
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
	/// ImGuiを表示させる関数
	/// </summary>
	void DisplayImGui() override;

	/// <summary>
	/// パラメータ作成関数
	/// </summary>
	/// <param name="parameterName">作成するパラメータ名</param>
	void CreateParameter(const std::string& parameterName);

	/// <summary>
	/// アニメーション作成関数
	/// </summary>
	void CreateAnimation();

public: // アクセッサ等

	/// <summary>
	/// カメラセッター
	/// </summary>
	/// <param name="camera">使用中カメラ</param>
	void SetCamera(Camera* camera) { camera_ = camera; }

	/// <summary>
	/// プレイヤーモデルマネージャのセッター
	/// </summary>
	/// <param name="pam">マネージャ本体</param>
	void SetPlayerAnimManager(PlayerAnimManager* pam) { pam_ = pam; }

	/// <summary>
	/// シーン遷移トリガーの状態ゲッター
	/// </summary>
	/// <returns>シーン遷移を行うか</returns>
	bool GetIsSceneChange() { return isSceneChange_; }

private: // メンバ変数

	// 音再生クラス
	Audio* audio_ = nullptr;

	// 決定音
	int32_t decisionSE_ = 0u;

	// 入力検知用
	Input* input_ = nullptr;
	// コントローラー入力
	XINPUT_STATE joyState_; // 現在フレーム用
	XINPUT_STATE preJoyState_; // 前フレーム用

	// アニメーションマネージャ
	AnimationManager* animManager_ = nullptr;
	// アニメーション
	Animation* anim_ = nullptr;

	// フェード演出マネージャ
	FadeManager* fadeManager_ = nullptr;

	// 演出用カメラ
	Camera* camera_ = nullptr;
	// プレイヤーモデル
	PlayerAnimManager* pam_ = nullptr;

	// フェンス用トランスフォーム
	WorldTransform fenceTransform_;

	// スプライトの透明度
	float spriteAlpha_ = 1.0f;

	// フェード演出トリガー
	bool isFade_ = false;
	// シーン遷移トリガー
	bool isSceneChange_ = false;
};

