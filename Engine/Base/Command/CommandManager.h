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
#include "../../Primitive/3d/Model/Model.h"

#include <wrl.h>
#include <dxcapi.h>
#pragma comment(lib, "dxcompiler.lib")

/// <summary>
/// 描画コマンドマネージャー
/// </summary>
class CommandManager
{
private: // サブクラス

	// <para>汎用定数バッファ</para>
	struct GeneralCBuffer
	{
		Matrix4x4 World;		 // ワールド行列
		Matrix4x4 WorldView;	 // ビュー行列
		Matrix4x4 WorldViewProj; // 射影変換行列
		uint32_t  DrawMeshlets;  // メッシュレット描画フラグ
	};

	/// <summary>
	/// テクスチャバッファ構造体
	/// </summary>
	struct TextureBuffer {
		std::vector<Microsoft::WRL::ComPtr<ID3D12Resource>> resource; // バッファリソース
		D3D12_GPU_DESCRIPTOR_HANDLE view{};							  // GPU上のハンドルを格納
		UINT usedCount = 0;											  // 使用中のインデックスバッファの数
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

public: // アクセッサ等

	/// <summary>
	/// ディスクリプタヒープのセッター
	/// </summary>
	/// <param name="rtv">レンダーターゲットビュー</param>
	/// <param name="srv">シェーダーリソースビュー</param>
	/// <param name="dsv">深度ステンシルビュー</param>
	void SetHeaps(RTV* rtv, SRV* srv, DSV* dsv);

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


	bool ReadDataFile(LPCWSTR filename, byte** data, UINT* size);

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

	// ルートシグネチャ
	// 以下はテーブル番号
	// 0 ... 汎用定数バッファ
	// 1 ... インデックスバッファ
	// 2 ... 頂点バッファ
	// 3 ... メッシュレットバッファ
	// 4 ... 固有頂点インデックスバッファ
	// 5 ... プリミティブインデックスバッファ
	Microsoft::WRL::ComPtr<ID3D12RootSignature> rootSignature_;

	// 汎用定数バッファ
	Microsoft::WRL::ComPtr<ID3D12Resource> generalCBuffer_;
	// 汎用定数バッファデータ
	std::unique_ptr<GeneralCBuffer> generalCBufferData_;
	// 定数バッファアクセス用のポインタ
	UINT8* cBufferBegin_ = nullptr;

	// テクスチャバッファ
	std::unique_ptr<TextureBuffer> textureBuffer_; // 本体
	const UINT kMaxTexture = 128;				   // テクスチャ最大数

	// デフォルトテクスチャ
	Texture* defaultTexture_;

	// 読み込み用テスト
	Model model_;

};

