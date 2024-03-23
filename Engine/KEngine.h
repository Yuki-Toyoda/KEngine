#pragma once
#include "Base/WinApp.h"
#include "Base/DirectXCommon.h"
#include "Base/SafeDelete.h"
#include "Resource/Texture/TextureManager.h"
#include "../Externals/imgui/ImGuiManager.h"
#include "Input/Input.h"
#include "Audio/Audio.h"
#include "GlobalVariables/GlobalVariables.h"
#include "Primitive/PrimitiveManager.h"
#include "Scene/SceneManager.h"
#include "Collider/CollisionManager.h"
#include "Particle/ParticleEmitterManager.h"
#include <dxgidebug.h>

/// <summary>
/// フレームワーク
/// </summary>
class KEngine
{
public: // 仮想デストラクタ

	// 仮想デストラクタ
	virtual ~KEngine() = default;

public: // メンバ関数

	/// <summary>
	/// 初期化関数
	/// </summary>
	virtual void Init();

	/// <summary>
	/// 初期化後処理関数
	/// </summary>
	void PostInit();

	/// <summary>
	/// 更新処理関数
	/// </summary>
	virtual void Update();

	/// <summary>
	/// 更新後処理関数
	/// </summary>
	void PostUpdate();

	/// <summary>
	/// 描画処理関数
	/// </summary>
	virtual void Draw() = 0;

	/// <summary>
	/// 終了前処理関数
	/// </summary>
	void PreFinalize();

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

	// 形状マネージャのインスタンス格納用
	//PrimitiveManager* primitiveManager_ = nullptr;

	// ゲーム終了フラグ
	bool endGameRequest_ = false;

	// パーティクルマネージャのインスタンス格納用
	//ParticleEmitterManager* particleManager_ = nullptr;

	// 衝突マネージャーのインスタンス格納用
	//CollisionManager* collisionManager_ = nullptr;

	// シーンマネージャーのインスタンス格納用
	SceneManager* sceneManager_ = nullptr;

	// メモリリークチェック用構造体
	struct D3DResourceLeakChecker {
		~D3DResourceLeakChecker() {
			// リソースリークチェック
			Microsoft::WRL::ComPtr<IDXGIDebug1> debug;
			if (SUCCEEDED(DXGIGetDebugInterface1(0, IID_PPV_ARGS(&debug)))) {
				debug->ReportLiveObjects(DXGI_DEBUG_ALL, DXGI_DEBUG_RLO_ALL);
				debug->ReportLiveObjects(DXGI_DEBUG_APP, DXGI_DEBUG_RLO_ALL);
				debug->ReportLiveObjects(DXGI_DEBUG_D3D12, DXGI_DEBUG_RLO_ALL);
			}
		}
	};
	std::unique_ptr<D3DResourceLeakChecker> memoryLeakChecker_;
};

