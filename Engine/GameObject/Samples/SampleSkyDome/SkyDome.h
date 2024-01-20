#pragma once
#include "../../BaseObject.h"

/// <summary>
/// 天球サンプル
/// </summary>
class SkyDome : public BaseObject
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
	/// テクスチャ変更関数
	/// </summary>
	/// <param name="tex">変更後のテクスチャ</param>
	void ChageTexture(Texture* tex);

private: // メンバ変数
};

