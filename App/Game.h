#pragma once
#include "Engine/Scene/SceneManager.h"
#include "Engine/KEngine.h"
#include "App/Scene/SceneFactory.h"

/// <summary>
/// ゲームクラス
/// </summary>
class Game : public KEngineFrameWork
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
	/// 描画関係処理
	/// </summary>
	void Draw() override;

	/// <summary>
	/// 終了処理
	/// </summary>
	void Finalize() override;

private: // メンバ変数

};

