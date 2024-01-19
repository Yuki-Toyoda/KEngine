#include "Rubble.h"

void Rubble::Init()
{

	lerpTimer_.Start(lerpTime_);
	AddMesh(&transform_, color_, "./Engine/Resource/Samples/Box", "Box.obj");
	color_ = { 0.0f,0.0f,0.0f,1.0f };
	AddColliderSphere("Rubble", &transform_.translate_, &transform_.scale_.x);
	
}

void Rubble::Update()
{
	lerpTimer_.Update();
	//時間が切れるか表示フラグがない場合オブジェクトを破壊
	if (/*lerpTimer_.GetIsFinish()||*/!isActive_) {
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
