#pragma once
#include "IRootSignature.h"

/// <summary>
/// ルートシグネチャ管理用マネージャ
/// </summary>
class RootSignatureManager
{
public: // サブクラス

	// ルートシグネチャのタイプ
	enum RootSignatureType {
		kSignatureTypeModel, // モデル用
	};

private: // シングルトンパターンの設定

	// シングルトンパターンの設定
	RootSignatureManager() = default;
	~RootSignatureManager() = default;
	RootSignatureManager(const RootSignatureManager&) = delete;
	const RootSignatureManager& operator=(const RootSignatureManager&) = delete;

public: // メンバ関数

	/// <summary>
	/// シングルトンインスタンスの取得
	/// </summary>
	/// <returns>シングルトンインスタンス</returns>
	static RootSignatureManager* GetInstance() {
		// クラスのインスタンスを取得
		static RootSignatureManager instance;
		// インスタンスを返す
		return &instance;
	}

	/// <summary>
	/// 初期化関数
	/// </summary>
	void Init();

	/// <summary>
	/// テンプレート引数にて指定したRootSignatureを生成し、配列に追加する関数
	/// </summary>
	/// <typeparam name="SelectRootSignature">生成するRootSignature</typeparam>
	template<IsIRootSignature SelectRootSignature>
	inline void CreateRootSignature() {
		// 新規RootSignatureを生成
		std::unique_ptr<SelectRootSignature> newSignature = std::make_unique<SelectRootSignature>();
		// 生成したルートシグネチャにデバイスを渡す
		newSignature->SetDevice(device_);
		// 初期化を実行
		newSignature->Init();

		// RootSignatureの生成を行う
		newSignature->CreateRootSignature();

		// 生成したRootSignatureを配列に追加する
		rootSignatures_.push_back(std::move(newSignature));
	}

public: // アクセッサ等

	/// <summary>
	/// デバイスセッター
	/// </summary>
	/// <param name="device">デバイス</param>
	void SetDevice(ID3D12Device2* device) { device_ = device; }

	/// <summary>
	/// 引数で指定したインデックスのRootSignatureを取得するゲッター
	/// </summary>
	/// <param name="index">取得するRootSignatureのインデックス</param>
	/// <returns>指定したRootSignature</returns>
	ID3D12RootSignature* GetRootSignature(const int32_t& index);

private: // メンバ変数

	// デバイス
	ID3D12Device2* device_ = nullptr;

	// ルートシグネチャ配列
	std::vector<std::unique_ptr<IRootSignature>> rootSignatures_;

};