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
		float padding[3];
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

	/// <summary>
	/// 色セッター
	/// </summary>
	/// <param name="color">色</param>
	void SetColor(const Vector4& color) { color_ = color; }
	/// <summary>
	/// 色ゲッター
	/// </summary>
	/// <returns>色</returns>
	Vector4 GetColor() const { return color_; }

private: // メンバ変数

	// アウトラインのしきい値
	float threshold_ = 0.5f;

	// アウトライン色
	Vector4 color_ = { 1.0f, 1.0f, 1.0f, 1.0f };

	// 定数バッファ
	ConstantBuffer<Data> buffer_;

};

