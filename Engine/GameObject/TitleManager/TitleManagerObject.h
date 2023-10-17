#pragma once
#include "../BaseObject.h"

// クラスの前方宣言
class Camera;

/// <summary>
/// タイトルのオブジェクト達
/// </summary>
class TitleManagerObject : public BaseObject {
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
	std::unique_ptr<Sprite> titleLogo_;
	Vector2 logoPosition_;
	Vector2 logoSize_;
	Vector4 logoColor_;
	Vector2 logoAnchorPoint_;

};

