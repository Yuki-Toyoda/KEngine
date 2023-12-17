#pragma once
#include <list>
#include "../Timer/DeltaTimer.h"
#include "AnimationKey.h"

// テンプレートの定義
template<typename T>

/// <summary>
/// アニメーション(キー配列クラス)
/// </summary>
class Animation
{
public: // コンストラクタ

	/// <summary>
	/// コンストラクタ
	/// </summary>
	/// <param name="animationName">アニメーション名</param>
	/// <param name="value">アニメーションで動かす値</param>
	/// <param name="endFlame">何フレームのアニメーションか</param>
	/// <param name="isLoop">ループ中か</param>
	Animation(const std::string& animationName, T* value, int32_t endFlame, bool isLoop);

public: // メンバ関数

	/// <summary>
	/// 更新関数
	/// </summary>
	void Update();

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
	std::stirng name_;

	// アニメーションキー配列
	std::list<AnimationKey<T>> keys_;

	// アニメーションさせる実体
	T* animateObject_ = nullptr;

	// 再生中フレーム
	int32_t playingFlame_ = 0;
	// 終端フレーム
	int32_t endFrame_ = 0;

	// 線形補間用タイマー
	KLib::DeltaTimer timer_;
	// アニメーション全体のタイマー
	KLib::DeltaTimer animTimer_;

	// ループトリガー
	bool isLoop_ = false;
	// 再生トリガー
	bool isPlay_ = false;

};



template<typename T>
inline Animation<T>::Animation(const std::string& animationName, T* value, int32_t endFrame, bool isLoop)
{
	// 名称設定
	name_ = animationName;
	// アニメーションさせるオブジェクトの取得
	animateObject_ = value;
	endFrame_ = endFrame;
}

template<typename T>
inline void Animation<T>::Update()
{
	// 再生中か
	if (isPlay_) {

	}
}

template<typename T>
inline void Animation<T>::AddItem()
{
}

template<typename T>
inline void Animation<T>::SetItem()
{
}

template<typename T>
inline void Animation<T>::ApplyItem()
{
}
