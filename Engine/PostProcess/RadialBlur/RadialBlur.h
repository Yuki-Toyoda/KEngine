#pragma once
#include "Engine/PostProcess/IPostProcess.h"

/// <summary>
/// ラジアルブラー
/// </summary>
class RadialBlur : public IPostProcess
{
private: // サブクラス

	/// <summary>
	/// データ構造体
	/// </summary>
	struct RadialData {
		Vector2 center{};		// 中心点
		float	blurStrength{}; // ブラー強さ
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
	/// ImGui表示関数
	/// </summary>
	void DisplayImGui() override;

public: // パブリックメンバ変数

	// ラジアルブラー用データ
	RadialData data_{};

private: // メンバ変数

	// 定数バッファ
	ConstantBuffer<RadialData> buffer_;

};

