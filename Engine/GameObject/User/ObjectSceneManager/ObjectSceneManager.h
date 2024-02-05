#pragma once
#include "../../BaseObject.h"

/// <summary>
/// オブジェクトシーンのマネージャー
/// </summary>
class ObjectSceneManager : public BaseObject
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
	/// ImGuiを表示させる関数
	/// </summary>
	void DisplayImGui() override;

	/// <summary>
	/// シーンによって表示内容を変える
	/// </summary>
	/// <param name="scene">変更シーン</param>
	void SetSceneNo(bool scene);
};

