#pragma once
#include <vector>
#include "BufferStructs.h"
#include "../RootSignature/IRootSignature.h"
#include "../Resource/Data/ConstantBuffer.h"
#include "../Resource/Data/StructuredBuffer.h"

/// <summary>
/// バッファ管理クラス
/// </summary>
class BufferManager
{
public: // コンストラクタ

	/// <summary>
	/// コンストラクタ
	/// </summary>
	BufferManager() = default;

	/// <summary>
	/// デストラクタ
	/// </summary>
	~BufferManager() = default;

public: // メンバ関数

	/// <summary>
	/// 初期化関数
	/// </summary>
	/// <param name="device">デバイス</param>
	/// <param name="srv">SRVヒープ</param>
	void Init(DirectXDevice* device, SRV* srv);

	/// <summary>
	/// 各バッファのデータをリセットする
	/// </summary>
	void Reset();

public: // アクセッサ等

	/// <summary>
	/// メッシュレット数ゲッター
	/// </summary>
	/// <param name="meshletNum">取得するMeshletの番号</param>
	/// <returns>メッシュレット数</returns>
	uint32_t GetMeshletCounts(const uint32_t& meshletNum);

	/// <summary>
	/// 共通データのアドレスをコマンドリストにセットするセッター
	/// </summary>
	/// <param name="num">セットするRootSignatureの番号</param>
	/// <param name="list">コマンドリスト</param>
	void SetCommonDataView(int num, ID3D12GraphicsCommandList6* list);

	/// <summary>
	/// メッシュレットデータセッター
	/// </summary>
	/// <param name="num">セットするRootSignatureの番号</param>
	/// <param name="meshletNum">セットするMeshletの番号</param>
	/// <param name="list">コマンドリスト</param>
	void SetMeshletDataView(int num, int meshletNum, ID3D12GraphicsCommandList6* list);
	
	/// <summary>
	/// 頂点情報データセッター
	/// </summary>
	/// <param name="num">セットするRootSignatureの番号</param>
	/// <param name="meshletNum">セットするMeshletの番号</param>
	/// <param name="list">コマンドリスト</param>
	void SetVertexDataView(int num, int meshletNum, ID3D12GraphicsCommandList6* list);

	/// <summary>
	/// 固有頂点インデックス情報データセッター
	/// </summary>
	/// <param name="num">セットするRootSignatureの番号</param>
	/// <param name="meshletNum">セットするMeshletの番号</param>
	/// <param name="list">コマンドリスト</param>
	void SetUniqueVertexIndicesDataView(int num, int meshletNum, ID3D12GraphicsCommandList6* list);

	/// <summary>
	/// プリミティブインデックス情報データセッター
	/// </summary>
	/// <param name="num">セットするRootSignatureの番号</param>
	/// <param name="meshletNum">セットするMeshletの番号</param>
	/// <param name="list">コマンドリスト</param>
	void SetPrimitiveIndicesDataView(int num, int meshletNum, ID3D12GraphicsCommandList6* list);

	/// <summary>
	/// メッシュレットデータを追加する関数
	/// </summary>
	/// <param name="meshlets">メッシュレットのデータ</param>
	void AddMeshletsData(const std::vector<DirectX::Meshlet>& meshlets);
	
	/// <summary>
	/// 頂点データを追加する関数
	/// </summary>
	/// <param name="vertices">頂点のデータ</param>
	void AddVertexData(const std::vector<VertexData>& vertices);
	
	/// <summary>
	/// 固有頂点データを追加する関数
	/// </summary>
	/// <param name="uniqueVertexIndices">固有頂点インデックスのデータ</param>
	void AddUniqueVertexIndicesData(const std::vector<uint8_t>& uniqueVertexIndices);
	
	/// <summary>
	/// プリミティブインデックスデータを追加する関数
	/// </summary>
	/// <param name="primitiveIndices">プリミティブインデックスのデータ</param>
	void AddPrimitiveIndicesData(const std::vector<DirectX::MeshletTriangle>& primitiveIndices);

private: // メンバ変数

	// デバイス
	DirectXDevice* device_ = nullptr;
	// SRVヒープ保存用
	SRV* srv_ = nullptr;

	// 共通のバッファ
	std::unique_ptr<ConstantBuffer<CommonData>> commonBuffer_;

	// ワールドトランスフォーム情報配列
	std::vector<std::unique_ptr<ConstantBuffer<WorldTransform>>> worldTransforms_;

	// メッシュレット情報配列
	std::vector<std::unique_ptr<StructuredBuffer<DirectX::Meshlet>>> meshletsBuffers_;
	// 頂点情報配列
	std::vector<std::unique_ptr<StructuredBuffer<VertexData>>> verticesBuffers_;
	// 固有頂点インデックス情報配列
	std::vector<std::unique_ptr<StructuredBuffer<uint32_t>>> uniqueVertexIndicesBuffers_;
	// プリミティブインデックス情報配列
	std::vector<std::unique_ptr<StructuredBuffer<uint32_t>>> primitiveIndicesBuffers_;
};

