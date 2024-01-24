#include "Rubble.h"
#include "../GameManager/GameManager.h"

void Rubble::Init()
{

	
	
	AddMesh(&transform_, color_, "./Engine/Resource/Samples/Box", "Box.obj");
	color_ = { 0.0f,0.0f,0.0f,1.0f };
	AddColliderSphere("Rubble", &transform_.translate_, &transform_.scale_.x);
	
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
	transform_.translate_ = KLib::Lerp<Vector3>(startPos_, goalPos_, KLib::EaseOutQuad(lerpTimer_.GetProgress()));
	//時間が切れるか表示フラグがない場合オブジェクトを破壊
	if (aliveTimer_.GetIsFinish()||!isActive_) {
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

void Rubble::OnCollisionEnter(Collider* collider)
{
	//プレイヤーと当たったら表示をなくす
	if (collider->GetGameObject()->GetObjectTag() == BaseObject::TagPlayer) {
		isActive_ = false;
	}
}
