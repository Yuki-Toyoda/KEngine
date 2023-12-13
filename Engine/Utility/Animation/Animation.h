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
	//Animation(const std::string& animationName, int32_t endFlame, );

public: // メンバ関数

	//// 更新関数
	//void Update();

private: // メンバ変数

	// アニメーションキー配列
	std::list<AnimationKey<T>> keys_;

	// アニメーションさせる実体
	T* animateObject_ = nullptr;

	// 再生中フレーム
	int32_t playingFlame_ = 0;
	// 終端フレーム
	int32_t endFlame_ = 0;

	// 線形補間用タイマー
	KLib::DeltaTimer timer_;
	// アニメーション全体のタイマー
	KLib::DeltaTimer animTimer_;

	// ループトリガー
	bool isLoop_ = false;
	// 

};
