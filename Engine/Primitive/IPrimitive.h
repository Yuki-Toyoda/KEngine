#pragma once
#include "../GameObject/WorldTransform.h"
#include "../Math/Vector2.h"
#include "../Math/Vector3.h"
#include "../Math/Vector4.h"
#include "../Resource/Texture/Texture.h"
#include "../Resource/Material/Material.h"
#include "../Utility/Observer/Observer.h"
#include "../../Externals/DirectXMesh/DirectXMesh.h"
#include "../Utility/IndexList/IndexList.h"
#include <wrl.h>
#include <d3d12.h>

// クラスの前方宣言
class CommandManager;

// 頂点構造体
struct Vertex {
	Vector3 position;							// 座標
	Vector2 texCoord;							// uv座標
	Vector3 normal;								// 法線
	Vector4 color = { 1.0f, 1.0f, 1.0f, 1.0f }; // 色
};

/// <summary>
/// 頂点データ構造体
/// </summary>
struct VertexData {
	DirectX::XMFLOAT4 position;
	DirectX::XMFLOAT2 texCoord;
	DirectX::XMFLOAT3 normal;
};

/// <summary>
/// パックされた三角形構造体
/// </summary>
struct PackedTriangle {
	uint32_t i0;
	uint32_t i1;
	uint32_t i2;
};

/// <summary>
/// メッシュレットバッファ構造体
/// </summary>
struct MeshletBuffer {
	Microsoft::WRL::ComPtr<ID3D12Resource> Resource;	  // リソース
	Index								   index;		  // SRV上の配列番号
	D3D12_GPU_DESCRIPTOR_HANDLE			   View;		  // GPU上のアドレス
	DirectX::Meshlet*					   meshlet;		  // メッシュレットのデータ
};

/// <summary>
/// 頂点バッファ構造体
/// </summary>
struct VertexBuffer {
	Microsoft::WRL::ComPtr<ID3D12Resource> Resource;	  // リソース
	Index								   index;		  // SRV上の配列番号
	D3D12_GPU_DESCRIPTOR_HANDLE			   View;		  // GPU上のアドレス
	VertexData*							   vertex;		  // メッシュレットのデータ
};

/// <summary>
/// 固有頂点バッファ構造体
/// </summary>
struct UniqueVertexBuffer {
	Microsoft::WRL::ComPtr<ID3D12Resource> Resource;	  // リソース
	Index								   index;		  // SRV上の配列番号
	D3D12_GPU_DESCRIPTOR_HANDLE			   View;		  // GPU上のアドレス
	uint32_t*							   uniqueVertex;  // メッシュレットのデータ
};

/// <summary>
/// プリミティブ頂点バッファ構造体
/// </summary>
struct PrimitiveIndexBuffer {
	Microsoft::WRL::ComPtr<ID3D12Resource> Resource;	  // リソース
	Index								   index;		  // SRV上の配列番号
	D3D12_GPU_DESCRIPTOR_HANDLE			   View;		  // GPU上のアドレス
	PackedTriangle*						   primitve;      // メッシュレットのデータ
};

/// <summary>
/// 形状情報の基底クラス
/// </summary>
class IPrimitive
{
public: // パブリックなサブクラス

	/// <summary>
	/// ブレンドモード列挙子
	/// </summary>
	enum kBlendMode {
		kBlendNormal,	// ノーマルaブレンド
		kBlendAdd,		// 加算合成
		kBlendSubtract, // 減算合成
		kBlendMultiply, // 乗算合成
		kBlendScreen,	// スクリーン合成
	};

	/// <summary>
	/// 描画タイプ列挙子
	/// </summary>
	enum kPrimitiveType {
		kModelNormal,	// 通常
		kModelParticle, // パーティクル
		kModelSprite,   // スプライト
	};

public: // メンバ関数

	/// <summary>
	/// (呼び出し禁止)コンストラクタ
	/// </summary>
	/// <param name="manager">マネージャー</param>
	IPrimitive(CommandManager* manager);

	/// <summary>
	/// 仮想デストラクタ
	/// </summary>
	virtual ~IPrimitive() = default;

	/// <summary>
	/// (呼び出し禁止)頂点配列を形状の頂点数にリサイズする関数
	/// </summary>
	virtual void ResizeVertices();
	/// <summary>
	/// (呼び出し禁止)インデックス情報配列を形状の情報をもとにリサイズする関数
	/// </summary>
	virtual void ResizeIndexes();

	/// <summary>
	/// 描画関数
	/// </summary>
	virtual void Draw();

	/// <summary>
	/// ImGui表示関数
	/// </summary>
	virtual void DisplayImGui();

	/// <summary>
	/// 任意サイズのバッファ生成関数
	/// </summary>
	/// <param name="size"バッファサイズ></param>
	/// <returns>バッファ本体</returns>
	ID3D12Resource* CreateBuffer(size_t size);

public: // アクセッサ等

	/// <summary>
	/// コマンドマネージャーセッター
	/// </summary>
	/// <param name="manager">コマンドマネージャー</param>
	void SetCommandManager(CommandManager* manager) { cmdManager_ = manager; }

	/// <summary>
	/// 頂点数ゲッター
	/// </summary>
	/// <returns></returns>
	virtual int GetVertexCount() const;
	/// <summary>
	/// インデックス情報数ゲッター
	/// </summary>
	/// <returns></returns>
	virtual int GetIndexCount() const;
	/// <summary>
	/// メッシュレット数ゲッター
	/// </summary>
	/// <returns>メッシュレット数</returns>
	virtual int GetMeshletCount() const;
	/// <summary>
	/// 指定したメッシュレットのプリミティブ数ゲッター
	/// </summary>
	/// <param name="i">プリミティブ数を取得するメッシュレット</param>
	/// <returns>プリミティブ数</returns>
	virtual int GetPrimitiveCount(const int& i) const;

public: // パブリックなメンバ変数

	// コマンドマネージャー
	CommandManager* cmdManager_ = nullptr;

	// 形状名称
	std::string name_ = "primitive";

	// インデックス情報
	std::vector<uint32_t> indexes_;

	// 頂点座標配列
	std::vector<VertexData> vertices_;
	// メッシュレット変換出力後情報配列群
	std::vector<DirectX::Meshlet>		  meshlets_;
	std::vector<uint32_t>				  uniqueVertices_;
	std::vector<PackedTriangle>			  primitiveIndices_;

	// メッシュレットバッファ
	std::unique_ptr<MeshletBuffer> meshletBuffer_;
	// 頂点バッファ
	std::unique_ptr<VertexBuffer> vertexBuffer_;
	// 固有頂点バッファ
	std::unique_ptr<UniqueVertexBuffer> uniqueVertexBuffer_;
	// プリミティブ頂点バッファ
	std::unique_ptr<PrimitiveIndexBuffer> primitiveIndexBuffer_;

	// 描画中心座標
	WorldTransform* transform_ = nullptr;

	Matrix4x4 matWorld_;

	// マテリアル
	Material material_;
	// 使用するテクスチャ
	Texture* texture_ = nullptr;

	// 全頂点カラー
	// ここに情報が入っている場合全頂点の色にこの色を適用する
	Vector4* commonColor = nullptr;

	// モデルタイプ
	// kModelNormal ... 通常の3Dモデル描画
	// kModelParticle ... パーティクル描画
	// kModelSprite ... スプライト描画
	kPrimitiveType primitiveType_ = kModelNormal;

	// 表示状態
	bool isActive_ = true;
	// 破壊トリガー
	bool isDestroy_ = false;

	// ビルボード用行列格納用
	Matrix4x4 billboardMat_;

	// 描画順
	int32_t layerNo_ = 0;

	// UIとして使用するか
	bool isUI_ = false;

};

/// <summary>
/// BasePrimitiveを継承したクラスを選択できるテンプレート
/// </summary>
template <class SelectPrimitive>
concept IsBasePrimitive = std::is_base_of<IPrimitive, SelectPrimitive>::value;