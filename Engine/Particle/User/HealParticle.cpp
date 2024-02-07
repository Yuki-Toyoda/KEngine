#include "HealParticle.h"

void HealParticle::Init()
{
	// 終端座標は最初に与えられた座標
	endPos_ = (transform_.translate_ + velocity_);
	// 始端座標設定のために与えられた速度ベクトル分移動させる
	//transform_.translate_ = (transform_.translate_ + velocity_);
	// 始端座標は計算した後の座標
	startPos_ = transform_.translate_;
	// 始端サイズは最初に与えられたサイズ
	startScale_ = transform_.scale_;

}

void HealParticle::Update()
{
	// 線形補間によって座標移動
	transform_.translate_ = KLib::Lerp<Vector3>(startPos_, endPos_, KLib::EaseOutQuad(aliveTimer_.GetProgress()));
	transform_.scale_ = KLib::Lerp<Vector3>(startScale_, { 0.0f, 0.0f, 0.0f }, KLib::EaseOutQuad(aliveTimer_.GetProgress()));

	// 透明度を補間で下げる
	color_.w = KLib::Lerp(0.75f, 0.0f, KLib::EaseInOutQuad(aliveTimer_.GetProgress()));
}
