#pragma once
#include "../BaseScene.h"
#include "SceneList.h"

/// <summary>
/// タイトルシーン
/// </summary>
class TitleScene final : public BaseScene {
public: // メンバ関数

	/// <summary>
	/// 初期化関数
	/// </summary>
	void Initialize() override;

	/// <summary>
	/// 更新関数
	/// </summary>
	void Update() override;

private: // メンバ変数

	// ImGui用ライト角度
	Vector3 setRotation_ = { 0.0f, -1.0f, 0.0f };
	// ImGui用ライト輝度
	float setIntensity_ = 1.0f;
	// ライト色
	Vector4 setLightColor = { 1.0f, 1.0f, 1.0f, 1.0f };
	// ImGui用ライト有効フラグ
	bool enableLighting_ = true;

};

