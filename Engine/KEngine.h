#pragma once
#include "Base/WinApp.h"
#include "Base/DirectXCommon.h"
#include "Base/SafeDelete.h"
#include "Resource/Texture/TextureManager.h"
#include "Externals/imgui/ImGuiManager.h"
#include "Input/Input.h"
#include "Audio/Audio.h"
#include "GlobalVariables/GlobalVariables.h"
#include "Scene/SceneManager.h"
#include "Collider/CollisionManager.h"
#include "Particle/CPU/CPUParticleEmitterManager.h"
#include "Particle/ParticleManager.h"
#include "Engine/GameObject/GameObjectManager.h"
#include "Engine/Sprite/SpriteManager.h"
#include "Engine/Particle/CPU/CPUParticleEmitterManager.h"
#include "Engine/Utility/Animation/AnimationManager.h"
#include <dxgidebug.h>

/// <summary>
/// フレームワーク
/// </summary>
class KEngineFrameWork
{
public: // 仮想デストラクタ

	// 仮想デストラクタ
	virtual ~KEngineFrameWork() = default;

public: // メンバ関数

	/// <summary>
	/// 初期化関数
	/// </summary>
	virtual void Init();

	/// <summary>
	/// 更新処理関数
	/// </summary>
	virtual void Update();

	/// <summary>
	/// 描画処理関数
	/// </summary>
	virtual void Draw() = 0;

	/// <summary>
	/// 終了処理
	/// </summary>
	virtual void Finalize();

	/// <summary>
	/// エンジン実行関数
	/// </summary>
	void Run();

public: // アクセッサ等

	/// <summary>
	/// ゲーム終了フラグのゲッター
	/// </summary>
	/// <returns>ゲームを終了するかどうか</returns>
	virtual bool GetIsEndGameRequest() { return endGameRequest_; }

protected: // メンバ変数

	// WinAppクラスのインスタンス格納用
	WinApp* winApp_ = nullptr;

	// DirectX汎用クラスのインスタンス格納用
	DirectXCommon* dxCommon_ = nullptr;

	// 入力関連クラスのインスタンス格納用
	Input* input_ = nullptr;

	// 音再生関連クラスのインスタンス格納用
	Audio* audio_ = nullptr;

	// ImGuiマネージャのインスタンス格納用
	ImGuiManager* imGuiManager_ = nullptr;

	// オブジェクトマネージャーのインスタンス格納用
	GameObjectManager* gameObjectManager_ = nullptr;

	// スプライトマネージャのインスタンス格納用
	SpriteManager* spriteManager_ = nullptr;

	// アニメーションマネージャのインスタンス格納用
	AnimationManager* animManager_ = nullptr;

	// パーティクルマネージャのインスタンス格納用
	ParticleManager* particleManager_ = nullptr;

	// 衝突マネージャーのインスタンス格納用
	CollisionManager* collisionManager_ = nullptr;

	// シーンマネージャーのインスタンス格納用
	SceneManager* sceneManager_ = nullptr;

	// ゲーム終了フラグ
	bool endGameRequest_ = false;
};

