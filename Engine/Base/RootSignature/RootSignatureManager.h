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

private: // メンバ変数

	// ルートシグネチャ配列
	std::vector<IRootSignature> rootSignatures_;

};