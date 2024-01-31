#include "IParticleEmitter.h"

void IParticleEmitter::PreInit(const std::string& name, int32_t maxCount, int32_t maxGenerateCount, const Vector3& translate, float aliveTime, float frequency, Texture* texture)
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

	// テクスチャを取得
	texture_ = texture;
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
		// 粒子配列のサイズが0以下なら
		if (particles_.size() <= 0) {
			// エミッタは終了している
			isEnd_ = true;
		}
	}

	// 生成間隔タイマーが終了していたら
	if (frequencyTimer_.GetIsFinish() && !aliveTimer_.GetIsFinish()) {
		// 新しい粒子を生成
		GenerateParticle();

		// 生成間隔リセット
		frequencyTimer_.Start(frequency_);
	}
}

void IParticleEmitter::GenerateParticle()
{
	// 一度に生成する数分のパーティクルを生成する
	for (int i = 0; i < generateParticleCount_; i++) {
		// パーティクル最大数を超過していた場合生成を行わない
		if (maxCount_ <= particles_.size()) {
			break;
		}

		// 生成粒子の大きさ設定
		float size = Math::RandomF(1.0f, 4.5f, 1);
		Vector2 generateScale = { size, size };
		// 生成粒子の速度ベクトル設定
		Vector3 generateVelocity 
			= { Math::RandomF(-0.25f, 0.25f, 3), Math::RandomF(-0.25f, 0.25f, 3), Math::RandomF(-0.25f, 0.25f, 3) };
		// 生成粒子の色
		Vector4 generateColor = { Math::RandomF(0.35f, 1.0f, 2), Math::RandomF(0.35f, 1.0f, 2), Math::RandomF(0.35f, 1.0f, 2), 1.0f };

		// 新しい粒子を生成
		std::unique_ptr<IParticle>newParticle = type_();
		newParticle->PreInit(1.0f, transform_.translate_, generateScale, generateVelocity, texture_, generateColor);

		// 生成した粒子をリストに追加
		particles_.push_back(std::move(newParticle));
	}
}

void IParticleEmitter::DisplayImGui()
{
	if (ImGui::TreeNode(name_.c_str())) {
		// 放出座標の調整
		transform_.DisplayImGui("EmitTransform");

		// 再生中かどうか表示
		ImGui::Text("PlayState : ");
		ImGui::SameLine(); // 非改行
		if (!aliveTimer_.GetIsFinish()) {
			ImGui::Text("Playing");
		}
		else {
			ImGui::Text("Ending");
		}

		// 表示されている粒子の数の表示
		ImGui::Text("AliveParticleCount : %d", (int)particles_.size());
		ImGui::SameLine();
		// と、粒子最大数の表示
		ImGui::Text("  Max : %d", (int)particles_.size());

		// 各種タイマーの情報表示
		aliveTimer_.DisplayImGui("AliveTime");		// エミッタ自体の生存時間
		frequencyTimer_.DisplayImGui("FrequencyTime");	// 生成間隔の表示

		ImGui::TreePop();
	}
	
}
