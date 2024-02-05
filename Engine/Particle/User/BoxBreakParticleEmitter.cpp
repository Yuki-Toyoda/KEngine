#include "BoxBreakParticleEmitter.h"

void BoxBreakParticleEmitter::Init()
{

}

void BoxBreakParticleEmitter::GenerateParticle()
{
	// 一度に生成する数分のパーティクルを生成する
	for (int i = 0; i < generateParticleCount_; i++) {
		// パーティクル最大数を超過していた場合生成を行わない
		if (maxCount_ <= particles_.size()) {
			break;
		}

		// 生成粒子の大きさ設定
		float size = Math::RandomF(1.0f, 3.0f, 1);
		Vector2 generateScale = { size, size };
		// 生成粒子の速度ベクトル設定
		Vector3 generateVelocity
			= { Math::RandomF(-0.35f, 0.35f, 3), Math::RandomF(0.25f, 0.65f, 3), Math::RandomF(-0.35f, 0.35f, 3) };
		// 生成粒子の色
		Vector4 generateColor = { Math::RandomF(0.35f, 1.0f, 2), Math::RandomF(0.35f, 1.0f, 2), Math::RandomF(0.35f, 1.0f, 2), 1.0f };

		// 新しい粒子を生成
		std::unique_ptr<IParticle>newParticle = type_();
		newParticle->PreInit(1.0f, transform_.translate_, generateScale, generateVelocity, texture_, generateColor);

		// 生成した粒子をリストに追加
		particles_.push_back(std::move(newParticle));
	}
}
