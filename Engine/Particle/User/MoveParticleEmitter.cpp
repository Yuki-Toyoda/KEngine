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

		
		float scale = 0;
		// プレイヤーの最小最大
		float minPlScale = 2.0f;
		float maxPlScale = 7.0f;
		// 円の最小最大
		float minTargetScale = 0.75f;
		float maxTargetScale = 2.35f;
		float nowPlScale = playerWorldTransform_->scale_.x;

		// スケールの計算
		// 割合
		float ratio = (nowPlScale - minPlScale) / (maxPlScale - minPlScale);
		// スケール
		scale = KLib::Lerp<float>(minTargetScale, maxTargetScale, ratio);

		float offset = 0.5f;

		// 生成粒子の大きさ設定
		float size = Math::RandomF(scale, scale + offset, 1);
		Vector2 generateScale = { size, size };

		// 生成座標のY軸を制御
		minTargetScale = 1.45f;
		maxTargetScale = 2.65f;
		float offsetPosition = KLib::Lerp<float>(minTargetScale, maxTargetScale, ratio);
		offset = 0.25f;

		// 生成場所をランダム設定
		Vector3 generatePosition
			= { Math::RandomF(-0.5f,0.5f,3),Math::RandomF(-offsetPosition - offset,-offsetPosition,3) ,0 };
		generatePosition += transform_.GetWorldPos();

		// 生成粒子の色
		//Vector4 generateColor = { Math::RandomF(0.5f, 1.0f, 2), Math::RandomF(0.5f, 1.0f, 2), Math::RandomF(0.0f, 0.5f, 2), 1.0f };
		Vector4 generateColor = { 1.0f,1.0f,1.0f,0.3f };
		// 新しい粒子を生成
		std::unique_ptr<IParticle>newParticle = type_();
		newParticle->PreInit(0.75f, generatePosition, generateScale, {}, texture_, generateColor);

		// 生成した粒子をリストに追加
		particles_.push_back(std::move(newParticle));
	}
}
