#pragma once
#include "../../BaseObject.h"
#include "../../../Audio/Audio.h"
#include "../../../Input/Input.h"
#include "../../../Sprite/SpriteList.h"

/// <summary>
/// 描画テスト用の箱オブジェクト
/// </summary>
class SampleBox : public BaseObject
{
public: // メンバ関数

	// 固有の初期化処理
	void Initialize(std::string name, Tag tag) override;
	// 更新処理
	void Update() override;
	// 描画処理
	void Draw() override;
	// スプライト描画処理
	void SpriteDraw()override;

public: // その他関数

	/// <summary>
	/// グローバル変数に値を追加する関数
	/// </summary>
	void AddGlobalVariables() override;

	/// <summary>
	/// グローバル変数の値を適用する関数
	/// </summary>
	void ApplyGlobalVariables() override;

	/// <summary>
	/// 衝突時コールバック関数
	/// </summary>
	/// <param name="object">衝突したオブジェクト</param>
	void OnCollision(BaseObject* object) override;

	/// <summary>
	/// 非衝突時コールバック関数
	/// </summary>
	/// <param name="object">前フレーム衝突していたオブジェクト</param>
	void OnCollisionExit(BaseObject* object)override;

private: // メンバ変数

	// サンプルサウンド
	uint32_t testSound_ = 0u;
	int voiceHundle = -1;

	// 入力状態検知
	Input* input_ = nullptr;

	bool isInput_ = false;

	// 衝突判定半径
	Vector3 colliderRadius_;

	// 衝突しているか
	bool isCollision_;

	// 参照テスト用値
	float intNum_;
	int testNum_;

	float min_;
	float max_;

	// テスト用番号シート
	uint32_t textureHandleNumberSheets_ = 0u;
	// ゲージ
	std::unique_ptr<Gage> gage_;

	std::unique_ptr<Counter> counter_;

	Vector4 backGroundColor_;
	Vector4 gageColor_;

};