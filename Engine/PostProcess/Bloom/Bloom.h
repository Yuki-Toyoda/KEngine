#pragma once
#include "Engine/PostProcess/IPostProcess.h"

#include "Engine/Base/PSO/PSO.h"

/// <summary>
/// ブルームエフェクト
/// </summary>
class Bloom : public IPostProcess
{
private: // サブクラス

	/// <summary>
	/// ブルームエフェクトに必要な一時データ
	/// </summary>
	struct TempData {
		RenderResource rResource;
		IRootSignature signature;
		PSO			   pso;
	};

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
	/// 事前処理関数
	/// </summary>
	/// <param name="list">コマンドリスト</param>
	/// <param name="target">ポストプロセス対象のリソース</param>
	void PreCommand(ID3D12GraphicsCommandList6* list, RenderResource* target) override;


	/// <summary>
	/// ImGui表示関数
	/// </summary>
	void DisplayImGui() override;

public: // アクセッサ等

	/// <summary>
	/// 閾値セッター
	/// </summary>
	/// <param name="threshold">閾値</param>
	void SetThreshold(const float threshold) { threshold_ = threshold; }

	/// <summary>
	/// 閾値ゲッター
	/// </summary>
	/// <returns>閾値</returns>
	float GetThreshold() const { return threshold_; }

private: // メンバ変数

	// ブルーム閾値
	float threshold_ = 0.0f;

	// 定数バッファ
	ConstantBuffer<float> buffer_;

	// 一時データ変数群
	TempData brightnessData_; // 輝度抽出
	TempData gaussX;		  // X軸ガウシアンブラー
	PSO		 gaussY;		  // Y軸ガウシアンブラー

};

