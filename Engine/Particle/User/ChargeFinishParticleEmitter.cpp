#include "ChargeFinishParticleEmitter.h"

void ChargeFinishParticleEmitter::Init()
{

}

void ChargeFinishParticleEmitter::GenerateParticle()
{
	// 粒子生成時の回転角を求める
	float generateAngle = (float)(std::numbers::pi * 2.0f) / generateParticleCount_;

	// 一度に生成する数分のパーティクルを生成する
	for (int i = 0; i < generateParticleCount_; i++) {
		// パーティクル最大数を超過していた場合生成を行わない
		if (maxCount_ <= particles_.size()) {
			break;
		}

		// 生成粒子の大きさ設定
		Vector2 generateScale = { 1.5f, 1.5f };
		// 生成粒子の方向ベクトルを角度を元に設定する
		Vector3 generateVelocity
			= { std::cos(generateAngle * i), 0.0f, std::sin(generateAngle * i) };

		// プレイヤーの最小最大
		float minPlScale = 2.0f;
		float maxPlScale = 7.0f;
		// 円の最小最大
		float minCirScale = 5.0f;
		float maxCirScale = 8.5f;
		float nowPlScale = playerWorldTransform_->scale_.x;
		// スケールの計算
		// 割合
		float ratio = (nowPlScale - minPlScale) / (maxPlScale - minPlScale);
		float value = 0;
		// スケール
		value = KLib::Lerp<float>(minCirScale, maxCirScale, ratio);

		// 方向ベクトルに速度を掛ける
		generateVelocity = generateVelocity * value;

		// 生成粒子の色
		//Vector4 generateColor = { Math::RandomF(0.35f, 1.0f, 2), Math::RandomF(0.35f, 1.0f, 2), Math::RandomF(0.35f, 1.0f, 2), 1.0f };
		Vector4 generateColor = { Math::RandomF(0.35f, 1.0f, 2), Math::RandomF(0.6f, 1.0f, 2), Math::RandomF(0.35f, 1.0f, 2), 1.0f };

		// 新しい粒子を生成
		std::unique_ptr<IParticle>newParticle = type_();
		// 共通初期化
		newParticle->PreInit(0.5f, transform_.GetWorldPos(), generateScale, generateVelocity, texture_, generateColor);
		// 固有初期化
		newParticle->Init();

		// 生成した粒子をリストに追加
		particles_.push_back(std::move(newParticle));
	}
}
