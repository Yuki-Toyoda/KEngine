#pragma once
#include "Data/ModelData.h"
#include "../GameObject/WorldTransform.h"
#include "../Base/Resource/Data/ConstantBuffer.h"
#include "../Base/Resource/Data/StructuredBuffer.h"

/// <summary>
/// モデル基底クラス
/// </summary>
class IModel
{
public: // コンストラクタ等

	/// <summary>
	/// コンストラクタ
	/// </summary>
	IModel() = default;
	/// <summary>
	/// デストラクタ
	/// </summary>
	virtual ~IModel() = default;

public: // 純粋仮想関数

	/// <summary>
	/// 初期化関数
	/// </summary>
	/// <param name="modelData">読み込み済みモデルデータ</param>
	virtual void Init(ModelData* modelData) 
	{
		// モデルデータを取得する
		modelData_ = std::move(modelData);
	};

	/// <summary>
	/// 更新関数
	/// </summary>
	virtual void Update() = 0;

	/// <summary>
	/// 描画関数
	/// </summary>
	/// <param name="cmdList">コマンドリスト</param>
	virtual void Draw(ID3D12GraphicsCommandList6* cmdList) = 0;

public: // パブリックメンバ変数

	// 破棄用トリガー
	bool isDestroy_ = false;

protected: // メンバ変数

	// 描画するモデルデータ
	ModelData* modelData_ = nullptr;

};

