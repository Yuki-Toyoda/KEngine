#pragma once
#include "Engine/PostProcess/IPostProcess.h"

/// <summary>
/// ガウシアンフィルター
/// </summary>
class GaussianFilter : public IPostProcess{
public: // メンバ関数

	/// <summary>
	/// 初期化関数
	/// </summary>
	void Init() override;

	/// <summary>
	/// 更新関数
	/// </summary>
	void Update() override;

	/// <summary>
	/// バインド書き込み関数
	/// </summary>
	/// <param name="stream">シェーダーファイル本体</param>
	/// <param name="root">ルートシグネチャ</param>
	/// <param name="i">バインド番号</param>
	void WriteBinding(std::ofstream* stream, IRootSignature* root, int* i) override;

	/// <summary>
	/// main関数内の処理を記述する関数
	/// </summary>
	/// <param name="stream">シェーダーファイル本体</param>
	void WriteProcess(std::ofstream* stream) override;

	/// <summary>
	/// コマンドリストに生成したリソースをセットする関数
	/// </summary>
	/// <param name="cmdList">セットするコマンドリスト</param>
	/// <param name="count">セットするバインド番号</param>
	void SetToCommandList(ID3D12GraphicsCommandList6* cmdList, int* offset) override;

	/// <summary>
	/// ImGui表示関数
	/// </summary>
	void DisplayImGui() override;

public: // アクセッサ等

	/// <summary>
	/// 閾値セッター
	/// </summary>
	/// <param name="intensity">閾値</param>
	void SetIntensity(const float intensity) { intensity_ = intensity; }

	/// <summary>
	/// 強度ゲッター
	/// </summary>
	/// <returns>強度</returns>
	float GetIntensity() const { return intensity_; }

private: // メンバ変数

	// ポストプロセスのかけ具合
	float intensity_ = 0.0f;

	// 定数バッファ
	ConstantBuffer<float> buffer_;

};

