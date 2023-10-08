#pragma once
#include <array>
#include <d3d12.h>
#include <string>
#include <wrl.h>

/// <summary>
/// テクスチャ管理クラス
/// </summary>
class TextureManager
{
public: // サブクラス

	// ディスクリプタサイズ
	static const size_t kDescriptorSize = 256;

	/// <summary>
	/// テクスチャ構造体
	/// </summary>
	struct Texture {
		Microsoft::WRL::ComPtr<ID3D12Resource> resource; // テクスチャリソース
		D3D12_CPU_DESCRIPTOR_HANDLE cpuDescriptorHandleSRV; // シェーダリソースビューのハンドル（CPU）
		D3D12_GPU_DESCRIPTOR_HANDLE gpuDescriptorHnadleSRV; // シェーダリソースビューのハンドル（GPU）
		std::string name; // ファイル名
	};

public: // メンバ関数

	/// <summary>
	/// 特定のインデックスのディスクリプタハンドルを取得する関数(CPU)
	/// </summary>
	/// <param name="descriptorHeap">ディスクリプタヒープ</param>
	/// <param name="descriptorSize">ディスクリプタサイズ</param>
	/// <param name="index">取得するヒープのインデックス</param>
	/// <returns>特定のインデックスのディスクリプタハンドル</returns>
	D3D12_CPU_DESCRIPTOR_HANDLE GetCPUDescriptorHandle(ID3D12DescriptorHeap* descriptorHeap, uint32_t descriptorSize, uint32_t index);

	/// <summary>
	/// 特定のインデックスのディスクリプタハンドルを取得する関数(GPU)
	/// </summary>
	/// <param name="descriptorHeap">ディスクリプタヒープ</param>
	/// <param name="descriptorSize">ディスクリプタサイズ</param>
	/// <param name="index">取得するヒープのインデックス</param>
	/// <returns>特定のインデックスのディスクリプタハンドル</returns>
	D3D12_GPU_DESCRIPTOR_HANDLE GetGPUDescriptorHandle(ID3D12DescriptorHeap* descriptorHeap, uint32_t descriptorSize, uint32_t index);

	/// <summary>
	/// テクスチャのロード関数
	/// </summary>
	/// <param name="fileName">テクスチャまでのファイルパス</param>
	/// <returns>テクスチャハンドル</returns>
	static uint32_t Load(const std::string& fileName);

	/// <summary>
	/// インスタンス取得関数
	/// </summary>
	/// <returns>インスタンス</returns>
	static TextureManager* GetInstance();

	/// <summary>
	/// 初期化関数
	/// </summary>
	/// <param name="device">デバイス</param>
	/// <param name="directoryPath">リソースファイルまでのファイルパス</param>
	void Intialize(ID3D12Device* device, std::string directoryPath = "Resources/");

	/// <summary>
	/// 全テクスチャのリセット関数
	/// </summary>
	void ResetAll();

	/// <summary>
	/// リソース情報取得
	/// </summary>
	/// <param name="textureHandle">情報を取得したいテクスチャハンドル</param>
	/// <returns>リソース情報</returns>
	const D3D12_RESOURCE_DESC GetResourceDesc(uint32_t textureHandle);

	/// <summary>
	/// テクスチャ名を取得する関数
	/// </summary>
	const std::string GetTextureName(uint32_t textureHandle);

	/// <summary>
	/// テクスチャのGPUハンドルを取得する関数
	/// </summary>
	/// <param name="textureHandle">取得するテクスチャ</param>
	/// <returns>テクスチャGPUハンドル</returns>
	const D3D12_GPU_DESCRIPTOR_HANDLE GetTextureGPUHandle(uint32_t textureHandle);

	/// <summary>
	/// ディスクリプタテーブルセット関数
	/// </summary>
	/// <param name="commandList">コマンドリスト</param>
	/// <param name="rotParamIndex">ルートパラメータの番号</param>
	/// <param name="textureHandle">テクスチャハンドル</param>
	void SetGraphicsDescriptorTable(
		ID3D12GraphicsCommandList* commandList, UINT rotParamIndex, uint32_t textureHandle);

private: // コンストラクタ等

	// コピーコンストラクタ、代入演算子無効化
	TextureManager() = default;
	~TextureManager() = default;
	TextureManager(const TextureManager&) = delete;
	TextureManager& operator=(const TextureManager&) = delete;

private: // メンバ変数

	// デバイス
	ID3D12Device* device_;
	// ディスクリプタサイズ
	UINT sDescriptorHandleIncrementSize_ = 0u;
	// ディレクトリパス
	std::string directoryPath_;
	// ディスクリプタヒープ
	Microsoft::WRL::ComPtr<ID3D12DescriptorHeap> descriptorHeap_;
	// 次に使うディスクリプタヒープの番号
	uint32_t indexNextDescriptorHeap_ = 0u;
	// テクスチャを格納しているコンテナ
	std::array<Texture, kDescriptorSize> textures_;

	/// <summary>
	/// ポインタでロードしたテクスチャを静的なテクスチャに変更する関数
	/// </summary>
	/// <param name="fileName">テクスチャまでのファイルパス</param>
	/// <returns>静的なテクスチャ</returns>
	uint32_t LoadInternal(const std::string& fileName);

};

