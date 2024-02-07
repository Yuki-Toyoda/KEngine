#include "HealParticleEmitter.h"

void HealParticleEmitter::Init()
{
}

void HealParticleEmitter::GenerateParticle()
{

	// 一度に生成する数分のパーティクルを生成する
	for (int i = 0; i < generateParticleCount_; i++) {
		// パーティクル最大数を超過していた場合生成を行わない
		if (maxCount_ <= particles_.size()) {
			break;
		}

		// 生成粒子の大きさ設定
		//float size = Math::RandomF(2.5f, 5.5f, 1);
		float size = 5.0f;
		Vector2 generateScale = { size, size };
		// 生成粒子の速度ベクトル設定
		Vector3 generateVelocity
			= { 0, Math::RandomF(0.5f, 0.85f, 3), 0 };

		float power = 5.0f;
		generateVelocity.y = generateVelocity.y * power;

		// 生成粒子の色
		Vector4 generateColor = { Math::RandomF(0.8f, 1.0f, 2), Math::RandomF(0.8f, 1.0f, 2), Math::RandomF(0.8f, 1.0f, 2), 1.0f };

		// 新しい粒子を生成
		std::unique_ptr<IParticle>newParticle = type_();
		newParticle->PreInit(1.0f, transform_.translate_, generateScale, generateVelocity, texture_, generateColor);

		// 生成した粒子をリストに追加
		particles_.push_back(std::move(newParticle));
	}


}
