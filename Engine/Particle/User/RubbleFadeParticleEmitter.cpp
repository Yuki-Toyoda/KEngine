#include "RubbleFadeParticleEmitter.h"
#include "RubbleFadeParticle.h"

void RubbleFadeParticleEmitter::Init()
{
}

void RubbleFadeParticleEmitter::GenerateParticle()
{

	// 一度に生成する数分のパーティクルを生成する
	for (int i = 0; i < generateParticleCount_; i++) {
		// パーティクル最大数を超過していた場合生成を行わない
		if (maxCount_ <= particles_.size()) {
			break;
		}

		// 生成粒子の大きさ設定
		float scaleValue = Math::RandomF(1.75f, 2.5f, 2);
		Vector2 generateScale = { scaleValue, scaleValue };

		// 生成場所をランダム設定
		Vector3 generatePosition
			= { Math::RandomF(-0.5f,0.5f,3),Math::RandomF(-0.5f,0.5f,3) ,0 };
		generatePosition += transform_.GetWorldPos();

		// 生成粒子の色
		Vector4 generateColor = { Math::RandomF(0.35f, 1.0f, 2), Math::RandomF(0.35f, 1.0f, 2), Math::RandomF(0.35f, 1.0f, 2), 0.85f };

		// 新しい粒子を生成
		std::unique_ptr<RubbleFadeParticle>newParticle = std::make_unique<RubbleFadeParticle>();
		// 共通初期化
		newParticle->PreInit(2.5f, generatePosition, generateScale, {}, texture_, generateColor);
		// 固有初期化
		newParticle->Init();

		// 振幅
		float generateAmplitude = Math::RandomF(0.05f, 0.1f, 2);
		// 振動数
		float generateFrequency = Math::RandomF(0.75f, 1.25f, 2);
		// 初手の方向（左右
		int randomSide = Math::Random(0, 1);
		// 専用の初期化
		newParticle->SetUp(generateAmplitude, generateFrequency,randomSide);

		// 生成した粒子をリストに追加
		particles_.push_back(std::move(newParticle));
	}


}
