#pragma once
#include <vector>
#include "../../../Externals/imgui/imgui.h"
#include "../../Debug/Debug.h"
#include "MainCommand.h"
#include "../../Resource/Texture/Texture.h"
#include "../../Math/Vector2.h"
#include "../../Math/Vector3.h"
#include "../../Math/Vector4.h"
#include "../../Math/Matrix4x4.h"
#include "../../GameObject/WorldTransform.h"
#include "../RootSignature/ModelRootSignature.h"

#include <wrl.h>
#include <dxcapi.h>
#pragma comment(lib, "dxcompiler.lib")

// クラスの前方宣言
class Mesh;

/// <summary>
/// 描画コマンドマネージャー
/// </summary>
class CommandManager
{
private: // サブクラス

	/// <summary>
	/// 汎用データ
	/// </summary>
	struct GeneralData {
		Matrix4x4 WorldViewProj;		 // 射影変換行列
		uint32_t  DrawMeshlets = false;  // メッシュレット描画フラグ
	};

	/// <summary>
	/// 汎用定数バッファ
	/// </summary>
	struct GeneralCBuffer
	{
		Microsoft::WRL::ComPtr<ID3D12Resource> Resource; // バッファリソース
		D3D12_GPU_VIRTUAL_ADDRESS View;					 // GPU上のアドレス
		GeneralData* Data;								 // データ本体
	};

	/// <summary>
	/// テクスチャバッファ構造体
	/// </summary>
	struct TextureBuffer {
		std::vector<Microsoft::WRL::ComPtr<ID3D12Resource>> Resource; // バッファリソース
		D3D12_GPU_DESCRIPTOR_HANDLE View{};							  // GPU上のハンドルを格納
		UINT UsedCount = 0;											  // 使用中のインデックスバッファの数
	};

public: // メンバ関数

	// コンストラクタ
	CommandManager() = default;
	// デストラクタ
	~CommandManager() = default;

	/// <summary>
	/// 初期化関数
	/// </summary>
	/// <param name="device">DirectX12のデバイス</param>
	void Init(ID3D12Device2* device);

	/// <summary>
	/// 描画関数
	/// </summary>
	void DrawCall();

	/// <summary>
	/// 描画後処理関数
	/// </summary>
	void PostDraw();

	/// <summary>
	/// 描画数リセット
	/// </summary>
	void Reset();

	/// <summary>
	/// 終了処理
	/// </summary>
	void Finalize();

public: // アクセッサ等

	/// <summary>
	/// デバイスゲッター
	/// </summary>
	ID3D12Device* GetDevice() { return device_; }

	/// <summary>
	/// ディスクリプタヒープのセッター
	/// </summary>
	/// <param name="rtv">レンダーターゲットビュー</param>
	/// <param name="srv">シェーダーリソースビュー</param>
	/// <param name="dsv">深度ステンシルビュー</param>
	void SetHeaps(RTV* rtv, SRV* srv, DSV* dsv);
	/// <summary>
	/// SRVヒープゲッター
	/// </summary>
	SRV* GetSRV() { return srv_; }

	/// <summary>
	/// ワールド行列の書き込みアドレスゲッター
	/// </summary>
	/// <returns>ビュープロジェクション行列の書き込み用アドレス</returns>
	Matrix4x4* const GetWorldMatrixAddress() const;

	/// <summary>
	/// ビュー行列の書き込みアドレスゲッター
	/// </summary>
	/// <returns>ビュープロジェクション行列の書き込み用アドレス</returns>
	Matrix4x4* const GetViewMatrixAddress() const;

	/// <summary>
	/// ビュープロジェクション行列の書き込みアドレスゲッター
	/// </summary>
	/// <returns>ビュープロジェクション行列の書き込み用アドレス</returns>
	Matrix4x4* const GetViewProjectionAddress() const;

	/// <summary>
	/// 描画データ登録関数
	/// </summary>
	void SetDrawData();

	/// <summary>
	/// コマンドキューのゲッター
	/// </summary>
	/// <returns>コマンドキュー</returns>
	ID3D12CommandQueue* GetQueue() { return commandQueue_.Get(); }

	/// <summary>
	/// コマンドリストゲッター
	/// </summary>
	/// <returns>コマンドリスト</returns>
	ID3D12GraphicsCommandList6* GetRenderCommandList() { return commandList_.Get(); }

public: // その他関数

	/// <summary>
	/// ImGuiを表示させる関数
	/// </summary>
	void DisplayImGui();

private: // プライベートなメンバ関数

	/// <summary>
	/// DXCの初期化関数
	/// </summary>
	void InitializeDXC();
	/// <summary>
	/// ルートシグネチャ生成関数
	/// </summary>
	void CreateRootSignature();
	/// <summary>
	/// ストラクチャーバッファ生成関数
	/// </summary>
	void CreateBuffers();
	/// <summary>
	/// 任意サイズのバッファ生成関数
	/// </summary>
	/// <param name="size"バッファサイズ></param>
	/// <returns>バッファ本体</returns>
	ID3D12Resource* CreateBuffer(size_t size);

	/// <summary>
	/// テクスチャ用リソースを生成する関数
	/// </summary>
	/// <param name="metaData">テクスチャデータ</param>
	/// <returns>リソース</returns>
	ID3D12Resource* CreateTextureResource(const DirectX::TexMetadata& metaData);
	/// <summary>
	/// 読み込み済みテクスチャデータをGPUにアップロードする関数
	/// </summary>
	/// <param name="texture">テクスチャリソース</param>
	/// <param name="mipImages">ミップマップ付きテクスチャ</param>
	ID3D12Resource* UploadTextureData(ID3D12Resource* texture, const DirectX::ScratchImage& mipImages);

private: // メンバ変数

	// DirectX12のデバイス
	ID3D12Device2* device_ = nullptr;

	// ディスクリプタヒープの本体
	RTV* rtv_ = nullptr; // レンダーターゲットビュー
	SRV* srv_ = nullptr; // シェーダーリソースビュー
	DSV* dsv_ = nullptr; // 深度ステンシルビュー

	// コマンドキュー
	Microsoft::WRL::ComPtr<ID3D12CommandQueue> commandQueue_;
	// コマンドアロケーター
	Microsoft::WRL::ComPtr<ID3D12CommandAllocator> commandAllocator_;
	// コマンドリスト
	Microsoft::WRL::ComPtr<ID3D12GraphicsCommandList6>commandList_;

	// 描画コマンド本体
	// シャドウマップ等を行うと数が増減する可能性あり
	std::vector<ICommand*> commands_;

	// フェンス
	Microsoft::WRL::ComPtr<ID3D12Fence> fence_; // 本体
	UINT64 fenceVal_ = 0;						// フェンス値

	// DXCコンパイラ
	std::unique_ptr<DXC> dxc_;

	// ルートシグネチャマネージャ
	//Microsoft::WRL::ComPtr<ID3D12RootSignature>	rootSignature_;
	std::unique_ptr<ModelRootSignature> rootSignature_;

	// 汎用定数バッファデータ
	std::unique_ptr<GeneralCBuffer> generalCBuffer_;

	// デフォルトテクスチャ
	Texture* defaultTexture_ = nullptr;

	// テスト用メッシュ
	Mesh* mesh_ = nullptr;

};

