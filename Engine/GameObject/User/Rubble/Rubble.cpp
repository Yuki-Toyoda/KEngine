#include "Rubble.h"
#include "../GameManager/GameManager.h"

void Rubble::Init()
{
	// 0 ~ 2 までのランダムな値を生成
	int generateValue = Math::Random(0, 2);

	switch (generateValue)
	{
	case 0:
		AddMesh(&transform_, color_, "./Resources/FallingCarrot", "fallingCarrot.obj");
		break;
	case 1:
		AddMesh(&transform_, color_, "./Resources/Tomato", "tomato.obj");
		break;
	case 2:
		AddMesh(&transform_, color_, "./Resources/GreenPepper", "greenPepper.obj");
		break;

	}

	color_ = { 1.0f,0.25f,0.0f,1.0f };
	AddColliderAABB("Rubble", &transform_.translate_, &CollisionScale_);
	
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
	color_.w = KLib::Lerp<float>(1.0f, 0.0f, aliveTimer_.GetProgress());
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
