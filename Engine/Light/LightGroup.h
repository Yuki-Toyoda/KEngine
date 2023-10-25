#pragma once
#include <Windows.h>
#include <wrl.h>
#include <d3d12.h>
 
#include "DirectionalLight.h"

#include "../externals/imgui/imgui.h"

/// <summary>
/// ライト全てを管理するクラス
/// </summary>
class LightGroup
{
public: // 定数

	// 平行光源最大数
	static const int kDirectionalLight = 1;
	
public: // サブクラス

	/// <summary>
	/// 定数バッファ構造体
	/// </summary>
	struct ConstBufferData {
		// 平行光源
		DirectionalLight::ConstBufferData directionalLights[kDirectionalLight];
	};

public: // 静的メンバ関数
	/// <summary>
	/// インスタンス生成関数
	/// </summary>
	/// <returns>インスタンス</returns>
	static LightGroup* Create();

	/// <summary>
	/// バッファリソース作成関数
	/// </summary>
	/// <param name="device">作成に使用するデバイス</param>
	/// <param name="sizeInBytes">サイズ</param>
	/// <returns></returns>
	static Microsoft::WRL::ComPtr<ID3D12Resource> CreateBufferResource(ID3D12Device* device, size_t sizeInBytes);

public: // メンバ関数
	/// <summary>
	/// 初期化関数
	/// </summary>
	void Initialize();

	/// <summary>
	/// 更新処理
	/// </summary>
	void Update();

	/// <summary>
	/// 描画処理
	/// </summary>
	void Draw(ID3D12GraphicsCommandList* commandList, UINT rootParameterIndex);

	/// <summary>
	/// 定数バッファ転送
	/// </summary>
	void TransferConstBuffer();

	/// <summary>
	/// 標準ライト設定を行う関数
	/// </summary>
	void DefaultLightSetting();

	/// <summary>
	/// 平行光源の有効状態のセッター
	/// </summary>
	/// <param name="index">ライト番号</param>
	/// <param name="acrive">有効状態</param>
	void SetDirectionalLightActive(int commitIndex, bool active);

	/// <summary>
	/// 平行光源の角度のセッター
	/// </summary>
	/// <param name="index">ライト番号</param>
	/// <param name="direction">角度</param>
	void SetDirectionalLightDirection(int commitIndex, Vector3 direction);

	/// <summary>
	/// 平行光源の色のセッター
	/// </summary>
	/// <param name="index">ライト番号</param>
	/// <param name="color">色</param>
	void SetDirectionalLightColor(int commitIndex, Vector4 color);

	/// <summary>
	/// 平行高原の輝度のセッター
	/// </summary>
	/// <param name="index"></param>
	/// <param name="intensity"></param>
	void SetDirectionalLightIntensity(int commitIndex, float intensity);

private: // メンバ変数

	// 定数バッファ
	Microsoft::WRL::ComPtr<ID3D12Resource> constBuff_;
	// 定数バッファのマップ
	ConstBufferData* constMap_ = nullptr;

	// 平行光源の配列
	DirectionalLight directionalLights_[kDirectionalLight];

	// 内容が変更された際にTrueになる変数
	bool dirty_ = false;
};

