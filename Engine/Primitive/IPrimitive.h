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

#include "../Base/Resource/Data/ConstantBuffer.h"
#include "../Base/Resource/Data/StructuredBuffer.h"

/// <summary>
/// 頂点データ構造体
/// </summary>
struct OldVertex {
	DirectX::XMFLOAT4 position;
	DirectX::XMFLOAT2 texCoord;
	DirectX::XMFLOAT3 normal;
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
	IPrimitive();

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
	/// <param name="cmdList">コマンドリスト</param>
	virtual void Draw(ID3D12GraphicsCommandList6* cmdList);

	/// <summary>
	/// ImGui表示関数
	/// </summary>
	virtual void DisplayImGui();

public: // アクセッサ等

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

	// 形状名称
	std::string name_ = "primitive";

	// インデックス情報
	std::vector<uint32_t> indexes_;

	// 頂点座標配列
	std::vector<OldVertex> vertices_;
	// メッシュレット変換出力後情報配列群
	std::vector<DirectX::Meshlet>		  meshlets_;
	std::vector<uint8_t>				  uniqueVertices_;
	std::vector<DirectX::MeshletTriangle> primitiveIndices_;

	// 描画中心座標
	WorldTransform* transform_ = nullptr;

	// データバッファ群
	std::unique_ptr<ConstantBuffer<Matrix4x4>>			transformBuffer_;			// ワールドトランスフォーム
	std::unique_ptr<StructuredBuffer<DirectX::Meshlet>> meshletBuffer_;				// メッシュレット
	std::unique_ptr<StructuredBuffer<OldVertex>>			vertexBuffer_;				// 頂点
	std::unique_ptr<StructuredBuffer<uint32_t>>			uniqueVertexIndicesBuffer_; // 固有頂点インデックス
	std::unique_ptr<StructuredBuffer<uint32_t>>			primitiveIndicesBuffer_;	// プリミティブインデックス

	// マテリアル
	std::unique_ptr<Material> material_;

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

	// スキンアニメーションを行うか
	bool isAnimated_ = false;

};

/// <summary>
/// BasePrimitiveを継承したクラスを選択できるテンプレート
/// </summary>
template <class SelectPrimitive>
concept IsBasePrimitive = std::is_base_of<IPrimitive, SelectPrimitive>::value;