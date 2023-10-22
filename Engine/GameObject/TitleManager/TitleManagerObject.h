#pragma once
#include "../BaseObject.h"

// クラスの前方宣言
class Camera;
class Input;

/// <summary>
/// タイトルのオブジェクト達
/// </summary>
class TitleManagerObject : public BaseObject {
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
	/// 使用するカメラのセッター
	/// </summary>
	/// <param name="camera">カメラ</param>
	void SetCamera(Camera* camera) { camera_ = camera; }

	/// <summary>
	/// ゲームシーンへのトリガーゲッター
	/// </summary>
	/// <returns>ゲームシーンに行くかどうか</returns>
	bool GetIsGoStageSelectSceneScene() { return isGoStageSelectScene_; }

public: // その他関数群

	/// <summary>
	/// グローバル変数に値を追加する関数
	/// </summary>
	void AddGlobalVariables() override;

	/// <summary>
	/// グローバル変数の値を適用する関数
	/// </summary>
	void ApplyGlobalVariables() override;

private: // メンバ変数

	// 入力取得用
	Input* input_ = nullptr;

	// タイトルで使用するカメラのポインタ
	Camera* camera_ = nullptr;

	// メインギア座標
	WorldTransform ｍainGearTransform_;
	// 中サイズギア座標
	WorldTransform mGearTransform_;
	// 中サイズギア座標
	WorldTransform mGearTransform2_;
	// 小サイズギア座標
	WorldTransform sGearTransform_;
	// タイトルギア座標
	WorldTransform titleGearTransform_;

	// タイトルロゴテクスチャ
	uint32_t textureHandleTitleLogo_ = 0u;
	// タイトルロゴスプライト
	std::unique_ptr<Sprite> titleLogo_;
	Vector2 logoPosition_;
	Vector2 logoSize_;
	Vector4 logoColor_;
	Vector2 logoAnchorPoint_;

	// タイトルボタンテクスチャ
	uint32_t textureHandleTitleButton_ = 0u;
	// タイトルボタンスプライト
	bool buttonIsActive_;
	std::unique_ptr<Sprite> titleButton_;
	Vector2 buttonPosition_;
	Vector2 buttonSize_;
	Vector4 buttonColor_;
	Vector2 buttonAnchorPoint_;

	// 演出中間地点
	int cameraStagingWayPoint_;
	// 演出t
	float rotateStagingT_;

	// イージング用カメラ始端座標
	Vector3 cameraStartTranslate_;
	// イージング用カメラ終端座標
	Vector3 cameraEndTranslate_;

	// カメラ演出用t
	float cameraStagingT_;
	// カメラ演出用tのループトリガー
	bool cameraStagingTReturn_;
	// カメラ演出用t2
	float cameraStagingT2_;
	// カメラ演出用t2のループトリガー
	bool cameraStagingT2Return_;
	// 演出時間
	float rotateStagingTime_;

	// タイトルロゴ演出スキップトリガー
	bool skipTitleStaging_;

	// カメラ手振れ演出トリガー
	bool enableCameraShake_;

	// ゲームシーンへのトリガー
	bool isGoStageSelectScene_;

};

