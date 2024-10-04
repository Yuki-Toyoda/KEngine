#pragma once
#include "Engine/PostProcess/IPostProcess.h"
#include "Engine/Math/Matrix4x4.h"
#include "Engine/Math/Vector4.h"

/// <summary>
/// アウトラインクラス
/// </summary>
class OutLine : public IPostProcess
{
private: // サブクラス

	/// <summary>
	/// アウトライン用データ構造体
	/// </summary>
	struct Data {
		Matrix4x4 projectionInverse;
		Vector4 color;
		float threshold;
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

public: // アクセッサ等

	/// <summary>
	/// ビュープロジェクションの逆行列セッター
	/// </summary>
	/// <param name="mat">ビュープロジェクションの逆行列</param>
	void SetViewProjectionInverse(const Matrix4x4& mat) { buffer_.data_->projectionInverse = mat; }

public: // パブリックメンバ変数

	// アウトラインのしきい値
	float threshold_ = 0.5f;

	// アウトライン色
	Vector4 color_ = { 1.0f, 1.0f, 1.0f, 1.0f };

private: // メンバ変数

	// 定数バッファ
	ConstantBuffer<Data> buffer_;

};

