#pragma once
#include <vector>
#include "../../Debug/Debug.h"
#include "MainCommand.h"
#include "ParticleCommand.h"
#include "SpriteCommand.h"

#include <wrl.h>
#include <dxcapi.h>
#pragma comment(lib, "dxcompiler.lib")

/// <summary>
/// 描画コマンドマネージャー
/// </summary>
class CommandManager
{
public: // メンバ関数

	// コンストラクタ
	CommandManager() = default;
	// デストラクタ
	~CommandManager() = default;

	/// <summary>
	/// 初期化関数
	/// </summary>
	/// <param name="device">DirectX12のデバイス</param>
	void Init(ID3D12Device* device);

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

public: // アクセッサ等

	/// <summary>
	/// ディスクリプタヒープのセッター
	/// </summary>
	/// <param name="rtv">レンダーターゲットビュー</param>
	/// <param name="srv">シェーダーリソースビュー</param>
	/// <param name="dsv">深度ステンシルビュー</param>
	void SetHeaps(RTV* rtv, SRV* srv, DSV* dsv, std::wstring vs, std::wstring ps);


	Matrix4x4* const GetViewProjection() const;

	/// <summary>
	/// 描画データ登録関数
	/// </summary>
	/// <param name="primitive">頂点データ</param>
	void SetDrawData(BasePrimitive* primitive);

	/// <summary>
	/// コマンドキューのゲッター
	/// </summary>
	/// <returns>コマンドキュー</returns>
	ID3D12CommandQueue* GetQueue() { return commandQueue_.Get(); }

	/// <summary>
	/// コマンドリストゲッター
	/// </summary>
	/// <returns>コマンドリスト</returns>
	ID3D12GraphicsCommandList* GetRenderCommandList() { return commandList_.Get(); }

public: // その他関数

	/// <summary>
	/// テクスチャリソース生成関数
	/// </summary>
	/// <param name="image">テクスチャデータ</param>
	/// <returns>テクスチャリソース</returns>
	int createTextureResource(const DirectX::ScratchImage& image);

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
	void CreateStructuredBuffer();
	/// <summary>
	/// 任意サイズのバッファ生成関数
	/// </summary>
	/// <param name="size"バッファサイズ></param>
	/// <returns>バッファ本体</returns>
	ID3D12Resource* CreateBuffer(size_t size);
	/// <summary>
	/// 任意のサイズのバッファ生成関数(テクスチャ用)
	/// </summary>
	/// <param name="metaData">テクスチャデータ</param>
	/// <returns>バッファ本体</returns>
	ID3D12Resource* CreateTextureBuffer(const DirectX::TexMetadata& metaData);

	/// <summary>
	/// テクスチャのアップロード関数
	/// </summary>
	/// <param name="mipImages">ミップマップ付きテクスチャ</param>
	void UploadTextureData(const DirectX::ScratchImage& mipImages);

private: // メンバ変数

	// DirectX12のデバイス
	ID3D12Device* device_ = nullptr;

	// ディスクリプタヒープの本体
	RTV* rtv_ = nullptr; // レンダーターゲットビュー
	SRV* srv_ = nullptr; // シェーダーリソースビュー
	DSV* dsv_ = nullptr; // 深度ステンシルビュー

	// コマンドキュー
	Microsoft::WRL::ComPtr<ID3D12CommandQueue> commandQueue_;
	// コマンドアロケーター
	Microsoft::WRL::ComPtr<ID3D12CommandAllocator> commandAllocator_;
	// コマンドリスト
	Microsoft::WRL::ComPtr<ID3D12GraphicsCommandList>commandList_;

	// 描画コマンド本体
	// シャドウマップ等を行うと数が増減する可能性あり
	std::vector<BaseCommand*> commands_;

	// フェンス
	Microsoft::WRL::ComPtr<ID3D12Fence> fence_; // 本体
	UINT64 fenceVal_ = 0;						// フェンス値

	// DXCコンパイラ
	std::unique_ptr<DXC> dxc_;

	// ルートシグネチャ
	// 以下はテーブル番号
	// 0 : バッファのインデックス情報
	// 1 : 光源情報
	// 2 : 頂点データ
	// 3 : viewProjection行列
	// 4 : worldTransform
	// 5 : マテリアル情報
	// 6 : テクスチャ
	Microsoft::WRL::ComPtr<ID3D12RootSignature> rootSignature_;

	// 平行光源バッファ
	std::unique_ptr<LightBuffer> lightBuffer_;

	// 頂点データ
	std::unique_ptr<VertexBuffer> vertexBuffer_; // 本体
	const UINT kMaxVertex = 655360;				 // 頂点情報の最大数(今回は受け売りで設定)

	// ビュープロジェクション行列バッファ
	std::unique_ptr<MatrixBuffer> viewProjectionBuffer_; // 本体
	const UINT kMaxVP = 2;
	// ワールドトランスフォームバッファ
	std::unique_ptr<MatrixBuffer> worldTransformBuffer_; // 本体
	const UINT kMaxWorldTransform = 128000;				 // 行列最大数
	// マテリアルバッファ
	std::unique_ptr<MaterialBuffer> materialBuffer_; // 本体
	const UINT kMaxMaterial = 12800;				 // マテリアル最大数
	// テクスチャバッファ
	std::unique_ptr<TextureBuffer> textureBuffer_; // 本体
	const UINT kMaxTexture = 128;				   // テクスチャ最大数

	// デフォルトテクスチャ
	Texture* defaultTexture_;

};

