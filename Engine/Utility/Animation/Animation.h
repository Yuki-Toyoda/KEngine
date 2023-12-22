#pragma once
#include <variant>
#include "AnimationKeys.h"
#include <wrl/client.h>

/// <summary>
/// アニメーションクラス
/// </summary>
class Animation
{
public: // コンストラクタ等

	// コンストラクタ
	Animation() = default;
	// デストラクタ
	~Animation() = default;

public: // メンバ関数

	/// <summary>
	/// 初期化関数
	/// </summary>
	/// <param name="name">アニメーション名</param>
	void Init(const std::string& name);
	
	/// <summary>
	/// 更新関数
	/// </summary>
	void Update();

	/// <summary>
	/// アニメーションキー配列を追加する関数
	/// </summary>
	/// <typeparam name="T">追加するキーの型名</typeparam>
	/// <param name="name">追加するキー配列の名前</param>
	/// <param name="value">アニメーションさせる値のポインタ</param>
	template<typename T>
	void AddAnimationKeys(const std::string name, T* value);

	void AddAnimationKey(const std::string keynam)

public: // その他関数群

	/// <summary>
	/// 調整項目クラスに値を追加する関数
	/// </summary>
	void AddItem();

	/// <summary>
	/// 調整項目クラスに値をセットする関数
	/// </summary>
	void SetItem();

	/// <summary>
	/// 調整項目クラスから値を取得する関数
	/// </summary>
	void ApplyItem();

private: // メンバ変数

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

};

template<typename T>
inline void Animation::AddAnimationKeys(const std::string name, T* value)
{
	// アニメーションキー配列を新しく追加する処理
	for (auto& keys : animationKeys_) {
		// 格納しているキー名と同一のキーがあった場合にはこのキーの追加は行わない
		std::visit([](auto& key) {
			// 同一キー名を発見した場合
			if (key.keyName_ == name) {
				// 追加を行わず処理を抜ける
				return;
			}
		}, keys);
	}

	// 同名キー配列が見つからなかった場合、キーの追加処理を行う
	AnimationKeys newKeys = AnimationKeys<Type>(name_, name, value); // 新しいキーの生成
	animationKeys_.push_back(newKeys);								 // 配列に追加
}
