#include "ChargeCircleParticleEmitter.h"

void ChargeCircleParticleEmitter::Init()
{
	//playerWorldTransform_->scale_
}

void ChargeCircleParticleEmitter::GenerateParticle()
{
	// 一度に生成する数分のパーティクルを生成する
	for (int i = 0; i < generateParticleCount_; i++) {
		// パーティクル最大数を超過していた場合生成を行わない
		if (maxCount_ <= particles_.size()) {
			break;
		}

		// 生成粒子の大きさ設定
		float scale = 15.0f;
		// プレイヤーの最小最大
		float minPlScale = 2.0f;
		float maxPlScale = 5.0f;
		// 円の最小最大
		float minCirScale = 12.5f;
		float maxCirScale = 27.5f;
		float nowPlScale = playerWorldTransform_->scale_.x;

		// スケールの計算
		// 割合
		float ratio = (nowPlScale - minPlScale) / (maxPlScale - minPlScale);
		// スケール
		scale = KLib::Lerp<float>(minCirScale, maxCirScale, ratio);

		Vector2 generateScale = { scale , scale };
		// 生成粒子の方向ベクトルをランダムに設定
		Vector3 generateVelocity
			= { Math::RandomF(-1.0f, 1.0f, 3), Math::RandomF(0.35f, 0.75f, 3), Math::RandomF(-1.0f, 1.0f, 3) };
		// 方向ベクトルにランダムな速度を掛ける
		generateVelocity = generateVelocity * Math::RandomF(3.0f, 5.0f, 1);

		// 生成粒子の色
		Vector4 generateColor = { Math::RandomF(0.1f, 0.3f, 2), Math::RandomF(0.1f, 0.3f, 2), Math::RandomF(0.1f, 0.3f, 2), 1.0f };
		//Vector4 generateColor = { Math::RandomF(0.35f, 0.5f, 2), Math::RandomF(0.35f, 0.5f, 2), Math::RandomF(0.0f, 0.2f, 2), 1.0f };

		// 新しい粒子を生成
		std::unique_ptr<IParticle>newParticle = type_();
		// 共通初期化
		newParticle->PreInit(1.f, transform_.GetWorldPos(), generateScale, {}, texture_, generateColor);
		// 固有初期化
		newParticle->Init();

		// 生成した粒子をリストに追加
		particles_.push_back(std::move(newParticle));
	}
}
