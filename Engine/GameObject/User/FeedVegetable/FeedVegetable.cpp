#include "FeedVegetable.h"

void FeedVegetable::Init()
{
	// 0 ~ 2 までのランダムな値を生成
	int generateValue = Math::Random(0, 2);

	// 生成した値を元にモデルを生成
	switch (generateValue)
	{
	case 0:
		// 人参のモデル生成
		AddMesh(&transform_, color_, "./Resources/FallingCarrot", "fallingCarrot.obj");
		break;
	case 1:
		// トマトのモデル生成
		AddMesh(&transform_, color_, "./Resources/Tomato", "tomato.obj");
		break;
	case 2:
		// ピーマンのモデル生成
		AddMesh(&transform_, color_, "./Resources/GreenPepper", "greenPepper.obj");
		break;
	}
}

void FeedVegetable::PostInit(const Vector3& start, const Vector3& end)
{
	// 開始地点の設定
	transform_.translate_ = start;
	// 開始地点の取得
	startPos_ = start;
	// 終端地点の取得
	endPos_ = end;
	endPos_.y = endPos_.y + 10.0f;

	// 初期角度をランダムに設定
	transform_.rotate_ = {
		Math::RandomF(-(float)std::numbers::pi * 2.0f, (float)std::numbers::pi * 2.0f, 1),
		Math::RandomF(-(float)std::numbers::pi * 2.0f, (float)std::numbers::pi * 2.0f, 1),
		Math::RandomF(-(float)std::numbers::pi * 2.0f, (float)std::numbers::pi * 2.0f, 1)
	};

	// 補間秒数をランダムに設定
	lerpTime_ = Math::RandomF(0.75f, 1.0f, 1);

	// 補間秒数タイマーを開始
	lerpTimer_.Start(lerpTime_);
	// Ｙ軸のみの補間タイマーを開始
	lerpTimerY_.Start(lerpTime_ / 2.0f);
}

void FeedVegetable::Update()
{
	// 補間タイマーが終了していたら破壊する
	if (lerpTimer_.GetIsFinish()) {
		Destroy();
	}
	else {
		// 座標を線形補間によって動作させる
		transform_.translate_.x = KLib::Lerp<float>(startPos_.x, endPos_.x, KLib::EaseInOutQuad(lerpTimer_.GetProgress()));
		transform_.translate_.z = KLib::Lerp<float>(startPos_.z, endPos_.z, KLib::EaseInOutQuad(lerpTimer_.GetProgress()));

		// 角度を加算
		transform_.rotate_.x += 0.1f;

		// y軸のみは別で動作させる
		if (!lerpTimerY_.GetIsFinish() && !isReturn_) {
			transform_.translate_.y = KLib::Lerp<float>(startPos_.y, endPos_.y, KLib::EaseInQuad(lerpTimer_.GetProgress()));
		}
		else if (!lerpTimerY_.GetIsFinish() && isReturn_) {
			transform_.translate_.y = KLib::Lerp<float>(startPos_.y, endPos_.y, KLib::EaseOutQuad(lerpTimer_.GetProgress()));
		}
		else {
			// リスタート
			lerpTimerY_.Start(lerpTime_ / 2.0f);
			isReturn_ = true;

			startPos_.y = endPos_.y;
			endPos_.y -= 10.0f;
		}
	}
	
	// 補間タイマー更新
	lerpTimer_.Update();
	// Y軸のみの補間タイマー更新
	lerpTimerY_.Update();
}

void FeedVegetable::DisplayImGui()
{
}
