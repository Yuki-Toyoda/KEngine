#pragma once
#include "IModel.h"

/// <summary>
/// スプライト用モデル
/// </summary>
class SpriteModel : public IModel
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

public: // パブリックメンバ変数

	// トランスフォーム
	WorldTransform transform_;
	// マテリアル
	Material material_;

	// 頂点配列
	std::vector<VertexData> vertices_;

	// トランスフォーム用定数バッファ
	std::unique_ptr<ConstantBuffer<Matrix4x4>> transformBuffer_;
	// マテリアル用定数バッファ
	std::unique_ptr<ConstantBuffer<MaterialData>> materialBuffer_;
	// 頂点情報用構造体バッファ
	std::unique_ptr<StructuredBuffer<VertexData>> vertexBuffer_;
};

