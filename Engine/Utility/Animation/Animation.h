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

	template<typename Type>
	void AddAnimationKeys(const std::string name, Type* value);

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

template<typename Type>
inline void Animation::AddAnimationKeys(const std::string name, Type* value)
{
	// 配列内の全てのキーを更新する
	for (auto& keys : animationKeys_) {
		
	}
}
