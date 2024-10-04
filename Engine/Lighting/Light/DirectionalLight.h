#pragma once
#include "ILight.h"
#include "Engine/Base/Resource/Data/ConstantBuffer.h"

/// <summary>
/// 平行光源クラス
/// </summary>
class DirectionalLight : public ILight
{
public: // サブクラス
	
	// 平行光源データ構造体
	struct DirectionalLightData
	{
		Vector4 color;		// ライト色
		Vector3 rotation;	// ライト角度
		float	intensity;	// 輝度
	};

public: // コンストラクタ等

	// 基底クラスのコンストラクタを使用
	using ILight::ILight;

public: // メンバ関数

	/// <summary>
	/// 初期化関数
	/// </summary>
	/// <param name="color">ライト色</param>
	/// <param name="rotation">ライト角</param>
	/// <param name="intensity">輝度</param>
	void Init(
		const Vector4& color = { 1.0f, 1.0f, 1.0f, 1.0f }, 
		const Vector3& rotation = { 0.0f, -1.0f, 0.0f },
		const float intensity = 1.0f
	);

	/// <summary>
	/// 更新関数
	/// </summary>
	void Update() override;

	/// <summary>
	/// ImGui表示関数
	/// </summary>
	void DisplayImGui() override;

public: // アクセッサ等

	/// <summary>
	/// バッファのGPU上のアドレスゲッター
	/// </summary>
	/// <returns>GPU上のアドレス</returns>
	D3D12_GPU_VIRTUAL_ADDRESS view() { return buffer_->GetGPUView(); }

private: // メンバ変数

	// 平行光源用定数バッファ
	// 今回はライトを複数生成する想定はしていないため、定数バッファを用いる
	std::unique_ptr<ConstantBuffer<DirectionalLightData>> buffer_;

};

