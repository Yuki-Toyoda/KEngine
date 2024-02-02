#include "Rubble.h"
#include "../GameManager/GameManager.h"
#include "../../../Particle/ParticleList.h"

void Rubble::Init()
{

	AddMesh(&transform_, color_, "./Engine/Resource/Samples/Box", "Box.obj");
	color_ = { 1.0f,0.25f,0.0f,1.0f };
	AddColliderAABB("Rubble", &transform_.translate_, &CollisionScale_);

	isGenerateParticle_ = false;
	emitterManager_ = ParticleEmitterManager::GetInstance();

}

void Rubble::Update()
{
	if (aliveTimer_.GetIsFinish()) {
		lerpTimer_.Start(moveTime_);
		aliveTimer_.Start(lerpTime_);
	}
	// 移動後パーティクルのセットアップ
	if (lerpTimer_.GetIsFinish() && !isGenerateParticle_) {
		SetUpParticle();
	}
	transform_.scale_ = { gameManager_->rubbleSize_, gameManager_->rubbleSize_, gameManager_->rubbleSize_ };
	
	// パーティクルの更新
	if (isGenerateParticle_) {
		fadeParticleEmitter_->transform_.translate_ = transform_.translate_;
	}

	lerpTimer_.Update();
	aliveTimer_.Update();
	transform_.translate_ = KLib::Lerp<Vector3>(startPos_, goalPos_, KLib::EaseOutQuad(lerpTimer_.GetProgress()));
	color_.w = KLib::Lerp<float>(1.0f, 0.0f, aliveTimer_.GetProgress());
	//時間が切れるか表示フラグがない場合オブジェクトを破壊
	if (aliveTimer_.GetIsFinish()||!isActive_) {
		fadeParticleEmitter_->SetIsPlay(false);
		fadeParticleEmitter_->SetIsEnd(true);
		Destroy();
		return;
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
	transform_.translate_ = startPos_;
	isGenerateParticle_ = true;

	fadeParticleEmitter_ =
		emitterManager_->CreateEmitter<RubbleFadeParticleEmitter, RubbleFadeParticle>
		("Fade", 25, 1, transform_.translate_, lerpTime_ + moveTime_, 1.0f, TextureManager::Load("uvChecker.png"));
	
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
