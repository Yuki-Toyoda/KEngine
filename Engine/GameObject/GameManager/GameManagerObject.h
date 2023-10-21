#pragma once
#include "../BaseObject.h"

// クラスの前方宣言
class Camera;

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
	/// ゲーム中のカメラ演出関数
	/// </summary>
	void CameraStaging();

	/// <summary>
	/// クリアゲージのアニメーション
	/// </summary>
	void ClearGageAnimation();

private: // メンバ変数

	// プレイヤーがいるギアのワールドトランスフォーム
	const WorldTransform* mainGearTransform_;
	// 演出用ギアのワールド座標
	WorldTransform gearTransforms_[3];
	// クリアゲージのワールドトランスフォーム
	WorldTransform clearGageTransform_;

	// カメラ
	Camera* camera_ = nullptr;
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

	// ステージ内全てのアイテム個数
	int stageItemCount_;
	// 現在のステージ内のアイテム個数
	int stageNowItemCount_;

	// ステージのクリア進捗パーセント
	int stageClearPercent_;

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

};

