#pragma once
#include "../BaseObject.h"

/// <summary>
/// タイトルのオブジェクト達
/// </summary>
class TitleObjects : public BaseObject {
public: // メンバ関数

	// 固有の初期化処理
	void Initialize(std::string name, Tag tag) override;
	// 更新処理
	void Update() override;
	// 描画処理
	void Draw() override;

public: // アクセッサ等



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

	// メインギア座標
	WorldTransform ｍainGearTransform_;
	// 中サイズギア座標
	WorldTransform mGearTransform_;
	// 小サイズギア座標
	WorldTransform sGearTransform_;

};

