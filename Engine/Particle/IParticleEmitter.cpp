#include "IParticleEmitter.h"

void IParticleEmitter::PreInit(const std::string& name, int32_t maxCount, int32_t maxGenerateCount, const Vector3& translate, float aliveTime, float frequency)
{
	// 名称設定
	name_ = name;
	// 最大数取得
	maxCount_ = maxCount;

	// 一度に生成する粒子数取得
	generateParticleCount_ = maxGenerateCount;
	// 発生座標設定
	transform_.Init();
	transform_.translate_ = translate;
	// 生存時間タイマースタート
	aliveTimer_.Start(aliveTime);
	// 粒子生成間隔タイマースタート
	frequency_ = frequency;
	frequencyTimer_.Start(0.0f);
}

void IParticleEmitter::Init()
{
	// 基底クラスでは記述なし
}

void IParticleEmitter::PreUpdate()
{
	// 粒子が終了状態の時リストから除外
	particles_.remove_if([](std::unique_ptr<IParticle>& particle) {
		if (particle->GetIsEnd())
			return true;

		return false;
	});

	// 生成されている粒子の更新
	for (std::unique_ptr<IParticle>& particle : particles_) {
		particle->Update();	    // 更新
		particle->PostUpdate(); // 更新後処理
	}
}

void IParticleEmitter::Update()
{
	// 基底クラスでは記述なし
}

void IParticleEmitter::PostUpdate()
{
	// 各種タイマーの更新
	aliveTimer_.Update();
	// パーティクルの生成個数が最大数を上回ってなければタイマーを更新する
	if (maxCount_ > particles_.size()) {
		frequencyTimer_.Update();
	}

	// エミッタの生存時間タイマーが終了していたら
	if (aliveTimer_.GetIsFinish()) {
		isEnd_ = true;
	}

	// 生成間隔タイマーが終了していたら
	if (frequencyTimer_.GetIsFinish() && !isEnd_) {

	}
}

void IParticleEmitter::GenerateParticle()
{
	// 一度に生成する数分のパーティクルを生成する
	for (int i = 0; i < generateParticleCount_; i++) {
		// パーティクル最大数を超過していた場合生成を行わない
		if (maxCount_ > particles_.size()) {
			break;
		}

		// 新しい粒子を生成
		std::unique_ptr<IParticle>newParticle = std::make_unique<IParticle>();
		Vector2 generateScale = { Math::RandomF(3.0f, 5.0f, 1), generateScale.x };
		newParticle->PreInit(1.0f, transform_.translate_, generateScale, )
	}
}
