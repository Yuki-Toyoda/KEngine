#include "DeltaTimer.h"

void KLib::DeltaTimer::Start()
{
	// 各種項目のリセット
	nowFlame_ = 0.0f; // フレーム
	isFinish_ = false; // 終了トリガー
	isActive_ = true; // 有効トリガー
}

void KLib::DeltaTimer::Start(float time)
{
	// 項目リセット
	Start();
	// 終了秒数取得
	finishFlame_ = time;
}

bool KLib::DeltaTimer::Update()
{
	// 終了している場合
	if (isFinish_) {
		// このタイマーを非有効に
		isActive_ = false;
		// 実行中ではない
		return false;
	}

	// フレームを加算
	nowFlame_ += GetDeltaTime();

	// 経過秒数が終了秒数を超えていたら
	if (nowFlame_ >= finishFlame_) {
		// タイマー終了
		isFinish_ = true;
	}
}

void KLib::DeltaTimer::Finish()
{
	// 全メンバ変数をリセット
	nowFlame_ = 0.0f;
	isFinish_ = true;
	isActive_ = false;
	finishFlame_ = 0.0f;
}

float KLib::DeltaTimer::GetProgress() const
{
	// 現在のタイマーの進行度を返す
	return std::clamp(nowFlame_ / finishFlame_, 0.f, 1.f);
}
