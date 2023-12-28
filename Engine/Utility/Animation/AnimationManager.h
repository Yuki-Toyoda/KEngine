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

	/// <summary>
	/// ImGui表示関数
	/// </summary>
	void DisplayImGui();

	/// <summary>
	/// アニメーションパラメータ作成関数
	/// </summary>
	/// <param name="name">生成するパラメータ名</param>
	void CreateAnimationParameter(const std::string name);
	
	/// <summary>
	/// 引数で指定したアニメーションに新しくキー配列を追加する関数
	/// </summary>
	/// <typeparam name="T">追加するキー配列の型</typeparam>
	/// <param name="animationName">アニメーション名</param>
	/// <param name="keyName">追加するキー名</param>
	/// <param name="value">アニメーションさせる値</param>
	template<typename T>
	void AddSelectAnimationKeys(const std::string& animationName, const std::string& keyName, T* value);

private: // メンバ変数

	// アニメーション配列
	std::vector<Animation> animations_;

	// パラメーター格納用
	std::vector<Animation> parameters_;

	// imGui用変数
	int imGuiSelectAnimation_ = 0; // 選択中アニメーション

};

template<typename T>
inline void AnimationManager::AddSelectAnimationKeys(const std::string& animationName, const std::string& keyName, T* value)
{
	// 全てのパラメーターから一致するパラメーターを探す
	for (Animation& a : parameters_) {
		// 一致しているパラメーターを発見した場合それにキーを追加
		if (a.name_ == animationName) {
			a.AddAnimationKeys<T>(keyName);
		}
	}
	value;
}
