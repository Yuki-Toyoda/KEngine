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
			= { 0, Math::RandomF(0.5f, 0.85f, 3), Math::RandomF(-0.85f,-0.5f,3)};

		float power = 5.0f;
		generateVelocity.y = generateVelocity.y * power;

		Vector3 generatePosition = { Math::RandomF(-4.0f,4.0f,2),0,Math::RandomF(-5.0f,1.5f,2) };
		float offset = 2.0f;
		generatePosition = transform_.translate_ + generatePosition;

		generatePosition.z += 5.0f;
		generatePosition.y -= 1.5f;

		if (transform_.translate_.x < 2.0f && transform_.translate_.x > 0) {
			generatePosition.x += offset;
		}
		else if (transform_.translate_.x > -2.0f && transform_.translate_.x < 0) {
			generatePosition.x -= offset;
		}

		// 生成粒子の色
		//Vector4 generateColor = { 0, Math::RandomF(0.9f, 1.0f, 2), 0, 1.0f };
		Vector4 generateColor = { 0,1.0f, 0, 1.0f };

		// 新しい粒子を生成
		std::unique_ptr<IParticle>newParticle = type_();
		newParticle->PreInit(1.0f, generatePosition, generateScale, generateVelocity, texture_, generateColor);

		// 生成した粒子をリストに追加
		particles_.push_back(std::move(newParticle));
	}


}
