#pragma once
#include <string>
#include "../../../Externals/imgui/imgui.h"
#include "../../Math/Vector2.h"
#include "../../Math/Vector3.h"
#include "../../Math/Vector4.h"

/// <summary>
/// ライトの基底クラス
/// </summary>
class ILight
{
public: // コンストラクタ等

	/// <summary>
	/// コンストラクタ
	/// </summary>
	/// <param name="name">ライト名</param>
	ILight(const std::string& name) {
		// 引数の値をメンバ変数に代入
		name_ = name;
	}

	/// <summary>
	/// 仮想デストラクタ
	/// </summary>
	virtual ~ILight() = default;

public: // 純粋仮想関数

	/// <summary>
	/// 更新関数
	/// </summary>
	virtual void Update() = 0;

	/// <summary>
	/// ImGui表示関数
	/// </summary>
	virtual void DisplayImGui() = 0;

protected: // 継承先メンバ変数

	// ライト名
	std::string name_;

	// ライトの汎用データ
	Vector4 color_	   = { 1.0f, 1.0f, 1.0f, 1.0f }; // 色
	Vector3 rotation_  = { 0.0f, 1.0f, 0.0f };		 // 回転角
	float	intensity_ = 1.0f;						 // 輝度
};

