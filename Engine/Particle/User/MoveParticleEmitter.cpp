#include "MoveParticleEmitter.h"

void MoveParticleEmitter::Init()
{
}

void MoveParticleEmitter::GenerateParticle()
{
	// 一度に生成する数分のパーティクルを生成する
	for (int i = 0; i < generateParticleCount_; i++) {
		// パーティクル最大数を超過していた場合生成を行わない
		if (maxCount_ <= particles_.size()) {
			break;
		}

		// 生成粒子の大きさ設定
		float size = Math::RandomF(0.75f, 1.75f, 1);
		Vector2 generateScale = { size, size };
		// 生成場所をランダム設定
		Vector3 generatePosition
			= { Math::RandomF(-0.75f,0.75f,3),Math::RandomF(-0.25f,0.25f,3) ,0 };
		generatePosition += transform_.GetWorldPos();

		// 生成粒子の色
		Vector4 generateColor = { Math::RandomF(0.35f, 1.0f, 2), Math::RandomF(0.35f, 1.0f, 2), Math::RandomF(0.35f, 1.0f, 2), 1.0f };

		// 新しい粒子を生成
		std::unique_ptr<IParticle>newParticle = type_();
		newParticle->PreInit(2.0f, generatePosition, generateScale, {}, texture_, generateColor);

		// 生成した粒子をリストに追加
		particles_.push_back(std::move(newParticle));
	}
}
