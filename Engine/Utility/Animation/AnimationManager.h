#pragma once
#include "Animation.h"

/// <summary>
/// アニメーションマネージャ
/// </summary>
class AnimationManager
{
private: // コンストラクタ等

	// シングルトンパターンの設定
	AnimationManager() = default;
	~AnimationManager() = default;
	AnimationManager(const AnimationManager&) = delete;
	const AnimationManager& operator=(const AnimationManager&) = delete;

public: // メンバ関数

	/// <summary>
	/// シングルトンインスタンスの取得
	/// </summary>
	/// <returns>シングルトンインスタンス</returns>
	static AnimationManager* GetInstance() {
		static AnimationManager instance;
		return &instance;
	}

	/// <summary>
	/// 初期化関数
	/// </summary>
	void Init();

	/// <summary>
	/// 更新関数
	/// </summary>
	void Update();

	template<typename T>
	Animation& CreateAnimation(const std::string);

private: // メンバ変数

	// アニメーション配列
	std::vector<Animation> animations_;

};

template<typename T>
inline Animation& AnimationManager::CreateAnimation(const std::string)
{
	
}
