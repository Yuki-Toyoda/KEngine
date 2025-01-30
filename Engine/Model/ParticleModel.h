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
	/// ImGui表示関数
	/// </summary>
	void DisplayImGui() override;

	/// <summary>
	/// モデルデータゲッター
	/// </summary>
	/// <returns>モデルデータ</returns>
	ModelData* GetModelData() { return modelData_; }

public: // パブリックメンバ変数

	// マテリアル配列
	std::vector<Material> materials_;

	// マテリアル用バッファ
	std::unique_ptr<StructuredBuffer<MaterialData>> materialsBuffer_;
};

