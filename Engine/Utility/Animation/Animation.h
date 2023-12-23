#pragma once
#include <variant>
#include "AnimationKeys.h"
#include <wrl/client.h>

/// <summary>
/// アニメーションクラス
/// </summary>
class Animation
{

public: // メンバ関数

	/// <summary>
	/// 初期化関数
	/// </summary>
	/// <param keyName="keyName">アニメーション名</param>
	void Init(const std::string& name);
	
	/// <summary>
	/// 更新関数
	/// </summary>
	void Update();

	/// <summary>
	/// アニメーションキー配列を追加する関数
	/// </summary>
	/// <typeparam keyName="T">追加するキーの型名</typeparam>
	/// <param keyName="keyName">追加するキー配列の名前</param>
	/// <param keyName="value">アニメーションさせる値のポインタ</param>
	template<typename T>
	void AddAnimationKeys(const std::string keyName, T* value);

	/// <summary>
	/// アニメーションキー配列を追加する関数
	/// </summary>
	/// <typeparam keyName="T">追加するキーの型名</typeparam>
	/// <param keyName="keyName">追加するキー配列の名前</param>
	template<typename T>
	void AddAnimationKeys(const std::string keyName);

public: // その他関数群

	/// <summary>
	/// ImGui表示関数
	/// </summary>
	void DisplayImGui();

	/// <summary>
	/// 呼び出した際、アニメーションを保存する
	/// </summary>
	void SaveAnimation();

public: // メンバ変数

	// アニメーション名
	std::string name_;

	// アニメーションキー配列をまとめた配列
	std::vector<std::variant<
		AnimationKeys<int>,
		AnimationKeys<int32_t>,
		AnimationKeys<float>,
		AnimationKeys<Vector2>,
		AnimationKeys<Vector3>,
		AnimationKeys<WorldTransform>
		>> animationKeys_;

	// ImGui用変数
	int imGuiSelectKey_; // 選択中キー

};

template<typename T>
inline void Animation::AddAnimationKeys(const std::string keyName, T* value)
{
	// アニメーションキー配列を新しく追加する処理
	for (auto& keys : animationKeys_) {
		// 格納しているキー名と同一のキーがあった場合にはこのキーの追加は行わない
		std::visit([keyName](auto& key) {
			// 同一キー名を発見した場合
			if (key.keysName_ == keyName) {
				// 追加を行わず処理を抜ける
				return;
			}
		}, keys);
	}

	// 同名キー配列が見つからなかった場合、キーの追加処理を行う
	AnimationKeys newKeys = AnimationKeys<T>(name_, keyName, value); // 新しいキーの生成
	animationKeys_.push_back(newKeys);								    // 配列に追加
}

template<typename T>
inline void Animation::AddAnimationKeys(const std::string keyName)
{
	// アニメーションキー配列を新しく追加する処理
	for (auto& keys : animationKeys_) {
		// 格納しているキー名と同一のキーがあった場合にはこのキーの追加は行わない
		std::visit([keyName](auto& key) {
			// 同一キー名を発見した場合
			if (key.keysName_ == keyName) {
				// 追加を行わず処理を抜ける
				return;
			}
			}, keys);
	}

	// 同名キー配列が見つからなかった場合、キーの追加処理を行う
	AnimationKeys newKeys = AnimationKeys<T>(name_, keyName); // 新しいキーの生成
	animationKeys_.push_back(newKeys);						  // 配列に追加
}
