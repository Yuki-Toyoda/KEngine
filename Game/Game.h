#pragma once
#include "../Engine/Scene/SceneManager.h"
#include "../Engine/GameObject/GameObjectManager.h"
#include "../Engine/Sprite/SpriteManager.h"
#include "../Engine/Particle/ParticleEmitterManager.h"
#include "../Engine/Utility/Animation/AnimationManager.h"
#include "../Engine/KEngine.h"
#include "../Engine/Scene/SceneFactory.h"

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

	// オブジェクトマネージャーのインスタンス格納用
	GameObjectManager* gameObjectManager_ = nullptr;

	// スプライトマネージャのインスタンス格納用
	SpriteManager* spriteManager_ = nullptr;

	// パーティクルマネージャー
	ParticleEmitterManager* particleManager_ = nullptr;

	// アニメーションマネージャのインスタンス格納用
	AnimationManager* animManager_ = nullptr;

};

