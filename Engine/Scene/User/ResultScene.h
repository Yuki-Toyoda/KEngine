#pragma once
#include "../BaseScene.h"
#include "../SceneList.h"

/// <summary>
/// リザルトシーン
/// </summary>
class ResultScene : public BaseScene
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

private: // メンバ変数

	// リザルトマネージャ
	ResultManager* rm_ = nullptr;

};

