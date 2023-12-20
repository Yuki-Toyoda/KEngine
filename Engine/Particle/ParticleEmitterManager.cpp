#include "ParticleEmitterManager.h"

void ParticleEmitterManager::Init()
{
	// エミッタ配列をクリア
	emitters_.clear();

	CreateEmitter<IParticleEmitter, IParticle>("test", 50, 3, Vector3(0.0f, 0.0f, 0.0f), 30.0f, 0.1f, TextureManager::Load("./Engine/Resource/Samples/Texture", "circle.png"));
}

void ParticleEmitterManager::Update()
{
	// 粒子が終了状態の時リストから除外
	emitters_.remove_if([](std::unique_ptr<IParticleEmitter>& emitter) {
		if (emitter->GetIsEnd())
			return true;

		return false;
	});

	// 生成されている粒子の更新
	for (std::unique_ptr<IParticleEmitter>& emitter : emitters_) {
		emitter->PreUpdate();	// 共通更新
		emitter->Update();	    // 更新
		emitter->PostUpdate();  // 更新後処理
	}
}
