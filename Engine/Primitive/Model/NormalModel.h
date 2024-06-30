#pragma once
#include "IModel.h"

/// <summary>
/// 通常モデル
/// </summary>
class NormalModel : public IModel
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

public: // パブリックメンバ変数

	// トランスフォーム
	WorldTransform transform_;
	// マテリアル
	std::vector<Material> materials_;

	// トランスフォーム用定数バッファ
	std::unique_ptr<ConstantBuffer<Matrix4x4>> transformBuffer_;
	// マテリアル用バッファ
	std::unique_ptr<StructuredBuffer<MaterialData>> materialsBuffer_;

};

