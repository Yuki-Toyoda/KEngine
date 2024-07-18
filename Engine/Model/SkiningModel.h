#pragma once
#include "IModel.h"
#include "Component/Skelton.h"
#include "Component/SkinCluster.h"

/// <summary>
/// マトリックスのパレット
/// </summary>
struct WellForGPU {
	Matrix4x4 skeltonSpaceMatrix;
	Matrix4x4 skeltonSpaceTransposeMatrix;
};

/// <summary>
/// ボーンとの親子関係用構造体
/// </summary>
struct ParentData {
	WorldTransform* transform_{};   // 子となるワールドトランスフォーム
	std::string		boneName_ = ""; // 親子付けされるボーン名称
	Matrix4x4		localMat_{};    // 代入するローカル行列
};

/// <summary>
/// スキニングを行うモデル
/// </summary>
class SkiningModel : public IModel
{
public: // メンバ関数

	/// <summary>
	/// 初期化関数
	/// </summary>
	/// <param name="modelData">読み込み済みモデルデータ</param>
	void Init(ModelData* modelData) override;

	/// <summary>
	/// 更新関数
	/// </summary>
	void Update() override;

	/// <summary>
	/// 描画関数
	/// </summary>
	/// <param name="cmdList">コマンドリスト</param>
	void Draw(ID3D12GraphicsCommandList6* cmdList) override;

public: // アクセッサ等

	/// <summary>
	/// ボーンとの親子関係セッター
	/// </summary>
	/// <param name="boneName">親子関係を結ぶボーン名</param>
	/// <param name="transform">親子関係を結ぶトランスフォーム</param>
	void SetBoneParent(const std::string boneName, WorldTransform* transform);

	/// <summary>
	/// 引数で指定された名称のボーンの4x4行列のゲッター
	/// </summary>
	/// <param name="boneName">取得するボーン名</param>
	/// <returns>対象ボーンの4x4行列</returns>
	Matrix4x4 GetBoneMatrix(const std::string boneName);

	/// <summary>
	/// 引数で指定された名称のボーンのワールド座標のゲッター
	/// </summary>
	/// <param name="boneName">取得するボーン名</param>
	/// <returns>対象ボーンのワールド座標</returns>
	Vector3 GetBonePosition(const std::string boneName);

public: // パブリックメンバ変数

	// トランスフォーム
	WorldTransform transform_;
	// マテリアル
	std::vector<Material> materials_;

	// スケルトン
	Skelton skelton_{};
	// スキンクラスター
	SkinCluster* skinCluster_ = nullptr;

	// アニメーションマネージャー
	SkinAnimationManager animationManager_{};

	// ボーンとの親子関係配列
	std::vector<ParentData> parentDatas_;

	// トランスフォーム用定数バッファ
	std::unique_ptr<ConstantBuffer<Matrix4x4>> transformBuffer_;
	// マテリアル用バッファ
	std::unique_ptr<StructuredBuffer<MaterialData>> materialsBuffer_;
	// マトリックスパレットバッファ
	std::unique_ptr<StructuredBuffer<WellForGPU>> palletteBuffer_;

};