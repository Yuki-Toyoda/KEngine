#pragma once
#include "../../BaseObject.h"
#include "../../../Utility/KLib.h"

/// <summary>
/// オブジェクトの描画を行うサンプル
/// </summary>
class SampleBox : public BaseObject
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
	
public: // その他関数

	/// <summary>
	/// 衝突した瞬間にコールバックされる関数
	/// </summary>
	/// <param name="object">衝突したオブジェクト</param>
	void OnCollisionEnter(BaseObject* object) override;

	/// <summary>
	/// 衝突していたオブジェクトから離れた時のコールバック関数
	/// </summary>
	/// <param name="object">衝突していたオブジェクト</param>
	void OnCollisionExit(BaseObject* object) override;

private: // メンバ変数


	// ブレンドモード設定用
	int imGuiSelectBlendMode_ = 0;

};

