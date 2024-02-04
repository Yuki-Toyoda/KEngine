#include "RubbleFadeParticle.h"

void RubbleFadeParticle::Init()
{
	// 開始時のサイズ
	startScale_ = transform_.scale_;

}

void RubbleFadeParticle::Update()
{

	// 波の動き用の計算
	velocity_.x = amplitude_ * std::cosf(2.0f * float(std::numbers::pi) * frequency_ * aliveTimer_.GetProgress());
	// どっち向きかを判断
	if (moveSide_ == 0) {
		velocity_.x *= (-1);
	}
	// 座標移動
	transform_.translate_.x += velocity_.x;
	transform_.translate_.y += 0.1f;
	// サイズの補間
	transform_.scale_ = KLib::Lerp<Vector3>(startScale_, { 0.0f, 0.0f, 0.0f }, KLib::EaseOutQuad(aliveTimer_.GetProgress()));

	// 透明度を補間で下げる
	color_.w = KLib::Lerp(1.0f, 0.0f, KLib::EaseOutQuad(aliveTimer_.GetProgress()));

}
