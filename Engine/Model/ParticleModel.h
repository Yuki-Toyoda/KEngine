#pragma once
#include "IModel.h"

/// <summary>
/// パーティクル用モデル
/// </summary>
class ParticleModel : public IModel
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

	/// <summary>
	/// バッファ生成関数
	/// </summary>
	/// <param name="maxDrawCount">描画最大数</param>
	void CreateBuffer(int32_t maxDrawCount);

	/// <summary>
	/// 描画モデル追加関数
	/// </summary>
	/// <param name="matWorld">ワールド行列</param>
	/// <param name="material">マテリアル</param>
	void AddDrawModel(const Matrix4x4& matWorld, const MaterialData& material);
	
	/// <summary>
	/// ImGui表示関数
	/// </summary>
	void DisplayImGui() override;

public: // パブリックメンバ変数

	// 最大描画数
	int32_t maxDrawCount_ = 0;

	// モデルの中心座標
	WorldTransform transform_;
	// モデルの基礎マテリアル
	Material material_;

	// トランスフォーム配列
	std::vector<Matrix4x4> transforms_;
	// マテリアル配列
	std::vector<MaterialData> materials_;

	// インスタンス数カウント用定数バッファ
	std::unique_ptr<ConstantBuffer<int32_t>> instanceCountBuffer_;
	// トランスフォーム用定数バッファ
	std::unique_ptr<StructuredBuffer<Matrix4x4>> transformsBuffer_;
	// マテリアル用バッファ
	std::unique_ptr<StructuredBuffer<MaterialData>> materialsBuffer_;
};

