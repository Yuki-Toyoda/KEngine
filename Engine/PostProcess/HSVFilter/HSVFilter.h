#pragma once
#include "../IPostProcess.h"

/// <summary>
/// HSV色変換フィルター
/// </summary>
class HSVFilter : public IPostProcess
{
private: // サブクラス

	/// <summary>
	/// HSV色空間用構造体
	/// </summary>
	struct HSV {
		float hue;		  // 色相
		float saturation; // 彩度
		float value;	  // 明度
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

	// HSV色変換
	HSV hsv_;

private: // メンバ変数


	ConstantBuffer<HSV> buffer_;

};

