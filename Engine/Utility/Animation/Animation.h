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

public: // メンバ関数

private: // メンバ変数

	// アニメーションキー配列
	std::list<AnimationKey<T>> keys_;

	// アニメーションさせる実体
	T* animateObject_ = nullptr;

	// 線形補間用タイマー
	KLib::DeltaTimer timer_;
	// アニメーション全体の進捗タイマー
	KLib::DeltaTimer animTimer_;

};

