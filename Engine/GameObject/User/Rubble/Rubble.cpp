#include "Rubble.h"
#include "../GameManager/GameManager.h"
#include "../../../Particle/ParticleList.h"

void Rubble::Init()
{
	// 0 ~ 2 までのランダムな値を生成
	int generateValue = Math::Random(0, 2);

	// 生成した値を元にモデルを生成
	switch (generateValue)
	{
	case 0:
		// 人参のモデル生成
		AddMesh(&transform_, color_, "./Resources/FallingCarrot", "fallingCarrot.obj");
		break;
	case 1:
		// トマトのモデル生成
		AddMesh(&transform_, color_, "./Resources/Tomato", "tomato.obj");
		break;
	case 2:
		// ピーマンのモデル生成
		AddMesh(&transform_, color_, "./Resources/GreenPepper", "greenPepper.obj");
		break;
	}

	color_ = { 1.0f,0.25f,0.0f,1.0f };
	AddColliderAABB("Rubble", &transform_.translate_, &CollisionScale_);

	isGenerateParticle_ = false;
	emitterManager_ = ParticleEmitterManager::GetInstance();

	animOffset_ = 0.5f;
	isReturn_ = false;
}

void Rubble::Update()
{
	if (aliveTimer_.GetIsFinish()) {
		lerpTimer_.Start(moveTime_);
		aliveTimer_.Start(lerpTime_);
	}
	transform_.scale_ = { gameManager_->rubbleSize_, gameManager_->rubbleSize_, gameManager_->rubbleSize_ };

	lerpTimer_.Update();
	aliveTimer_.Update();
	animTimer_.Update();
	if (lerpTimer_.GetIsActive()) {
		transform_.translate_ = KLib::Lerp<Vector3>(startPos_, goalPos_, KLib::EaseOutQuad(lerpTimer_.GetProgress()));
	}
	color_.w = KLib::Lerp<float>(1.0f, 0.0f, aliveTimer_.GetProgress());
	//時間が切れるか表示フラグがない場合オブジェクトを破壊
	if (aliveTimer_.GetIsFinish()||!isActive_) {
		if (fadeParticleEmitter_ != nullptr) {
			fadeParticleEmitter_->SetIsPlay(false);
			fadeParticleEmitter_->SetIsEnd(true);
		}
		Destroy();
		return;
	}

	// 移動後パーティクルのセットアップ
	if (lerpTimer_.GetIsFinish() && !isGenerateParticle_) {
		SetUpParticle();
		default_Y_ = transform_.translate_.y;
		animTimer_.Start(1.f);
	}
	// パーティクルの更新
	if (isGenerateParticle_) {
		fadeParticleEmitter_->transform_.translate_ = transform_.translate_;
		fadeParticleEmitter_->transform_.translate_.y = fadeParticleEmitter_->transform_.translate_.y + 0.5f;
		if (isReturn_) {
			transform_.translate_.y = KLib::Lerp<float>(default_Y_, default_Y_ + animOffset_, KLib::EaseOutQuad(animTimer_.GetProgress()));
		}
		else {
			transform_.translate_.y = KLib::Lerp<float>(default_Y_ + animOffset_, default_Y_, KLib::EaseOutQuad(animTimer_.GetProgress()));
		}
	}

	if (animTimer_.GetIsFinish()) {
		if (isReturn_) {
			isReturn_ = false;
		}
		else {
			isReturn_ = true;
		}
		animTimer_.Start(1);
	}

}

void Rubble::DisplayImGui()
{
	float time = lerpTimer_.GetProgress();
	ImGui::DragFloat("Time", &time);
	transform_.DisplayImGui();
}

void Rubble::SetUpParticle()
{
	isGenerateParticle_ = true;

	fadeParticleEmitter_ =
		emitterManager_->CreateEmitter<RubbleFadeParticleEmitter, RubbleFadeParticle>
		("Fade", 25, 1, goalPos_, lerpTime_ + moveTime_, 1.0f, TextureManager::Load("vegetableParticle.png"));
	
	// 開始処理
	fadeParticleEmitter_->SetIsEnd(false);
	fadeParticleEmitter_->SetIsPlay(true);
	
}

void Rubble::OnCollisionEnter(Collider* collider)
{
	//プレイヤーと当たったら表示をなくす
	if (collider->GetGameObject()->GetObjectTag() == BaseObject::TagPlayer) {
		isActive_ = false;
	}
}
