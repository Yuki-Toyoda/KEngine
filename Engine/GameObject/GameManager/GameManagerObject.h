#pragma once
#include "../BaseObject.h"
#include "../../Audio/Audio.h"
#include "../../Input/Input.h"

// クラスの前方宣言
class Camera;
class StageManager;

/// <summary>
/// ゲームシーンのマネージャーオブジェクト
/// </summary>
class GameManagerObject : public BaseObject {
private: // サブクラス

	/// <summary>
	/// 演出中間地点列挙子
	/// </summary>
	enum StagingWayPoint {
		WayPoint1,
		WayPoint2,
		WayPoint3,
		WayPoint4,
		WayPoint5,
		WayPoint6,
		WayPoint7,
		WayPoint8,
		WayPoint9,
	};

public: // メンバ関数

	// 固有の初期化処理
	void Initialize(std::string name, Tag tag) override;
	// 更新処理
	void Update() override;
	// 描画処理
	void Draw() override;
	// スプライト描画
	void SpriteDraw() override;

public: // アクセッサ等

	/// <summary>
	/// カメラセッター
	/// </summary>
	/// <param name="camera">使用中のカメラ</param>
	void SetCamera(Camera* camera) { camera_ = camera; }

	/// <summary>
	/// ギアのワールドトランスフォームセッター
	/// </summary>
	/// <param name="transform">ギアのワールドトランスフォーム</param>
	void SetGearTransform(WorldTransform* transform) { mainGearTransform_ = transform; }

	/// <summary>
	/// ステージセレクトシーンへの遷移トリガーゲッター
	/// </summary>
	/// <returns>遷移するか</returns>
	bool GetIsGoStageSelectScene() { return isGoStageSelectScene_; }

	/// <summary>
	/// リトライトリガーの遷移トリガーゲッター
	/// </summary>
	/// <returns>遷移するか</returns>
	bool GetIsRetryThisScene() { return isRetryThisScene_; }

public: // その他関数群

	/// <summary>
	/// グローバル変数に値を追加する関数
	/// </summary>
	void AddGlobalVariables() override;

	/// <summary>
	/// グローバル変数の値を適用する関数
	/// </summary>
	void ApplyGlobalVariables() override;

	/// <summary>
	/// ゲーム中のカメラ演出関数
	/// </summary>
	void CameraStaging();

	/// <summary>
	/// クリアゲージのアニメーション
	/// </summary>
	void ClearGageAnimation();

private: // メンバ変数

	// ステージマネージャ
	StageManager* stageManager_ = nullptr;

	// 入力
	Input* input_ = nullptr;
	// 音再生
	Audio* audio_ = nullptr;

	// 音量
	float* bgmVolume_;
	float* seVolume_;

	// BGMハンドル
	uint32_t bgmHandle_;
	uint32_t soundHandleBack_ = 0u; // 戻る時の効果音
	uint32_t soundHandleRetry_ = 0u; // リトライ時の効果音
	uint32_t soundHandleClear_ = 0u; // クリア時効果音
	uint32_t soundHandlePerfectClear_ = 0u; // 完全クリア時効果音

	// 環境音ボイスハンドル
	int voiceHandleBGM_ = -1;

	// プレイヤーがいるギアのワールドトランスフォーム
	const WorldTransform* mainGearTransform_;
	// 演出用ギアのワールド座標
	WorldTransform gearTransforms_[3];
	// クリアゲージのワールドトランスフォーム
	WorldTransform clearGageTransform_;
	// クリア時のチェックマークワールド座標
	WorldTransform clearCheckTransform_;

	// カメラ
	Camera* camera_ = nullptr;
	// カメラ演出トリガー
	bool cameraIsStaging_;
	// カメラ演出始端座標
	Vector3 cameraStartTranslate_;
	// カメラ演出終端座標
	Vector3 cameraEndTranslate_;
	// カメラ演出中間地点
	int cameraStagingWayPoint_;
	// カメラ演出用t
	float cameraStagingT_;
	// カメラ演出時間
	float cameraStagingTime_;

	// ステージセレクトへ戻る演出中間地点
	int backStageSelectStagingWayPoint_;
	bool backStageSelectStaging_;
	bool isRetry_;

	// ステージ内全てのアイテム個数
	int stageItemCount_;
	// 現在のステージ内のアイテム個数
	int stageNowItemCount_;

	// ステージのクリア進捗パーセント
	int stageClearPercent_;

	// ステージセレクトシーンへのトリガー
	bool isGoStageSelectScene_;
	// リトライトリガー
	bool isRetryThisScene_;

	// 完全クリアトリガー
	bool isPerfectClear_;

	// UIテクスチャ群
	int32_t textureHandleNumberSheets_ = 0u; // 数字のシート
	int32_t textureHandleTextLeftItem_ = 0u; // 残りアイテム数テキスト
	int32_t textureHandleSlash_ = 0u; // /テクスチャ
	int32_t textureHandleTextClearPercent_ = 0u; // クリア進捗テキスト

	/// UIスプライト群

	// 残りアイテム数テキストUI
	Vector2 leftItemTextPosition_; // 座標
	Vector2 leftItemTextSize_; // 大きさ
	std::unique_ptr<Sprite> leftItemTextSprite_; // スプライト本体
	// /テキストUI
	Vector2 slashPosition_; // 座標
	Vector2 slashSize_; // 大きさ
	std::unique_ptr<Sprite> slashSprite_; // スプライト本体
	// クリア進捗テキストUI
	Vector2 clearPercentTextPosition_; // 座標
	Vector2 clearPercentTextSize_; // 大きさ
	std::unique_ptr<Sprite> clearPercentTextSprite_; // スプライト本体

	// アイテム個数カウンター
	std::unique_ptr<Counter> stageItemCounter_ = nullptr;
	// アイテム個数カウンター
	std::unique_ptr<Counter> stageNowItemCounter_ = nullptr;
	// 進捗％カウンター
	std::unique_ptr<Counter> stageClearCounter_ = nullptr;

	// UIスプライト色
	Vector4 spriteUIColor_;
	Vector4 spriteClearUIColor_;

};

