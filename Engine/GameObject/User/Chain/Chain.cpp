#include "Chain.h"
#include "../../../Resource/Texture/TextureManager.h"
void Chain::Init()
{
	line_ = std::make_unique<Line>();
	line_->Init("ChainLine",  line_->position_,{ 0.35f,0.35f }, line_->length_, TextureManager::Load("./Resources", "ChainTex.png"));
	line_->AddCollider("Chain", this);

	line_->rotate_.y = (float)std::numbers::pi / 2.0f;
}

void Chain::Update()
{
	//鎖をプレイヤーから武器まで伸ばす
	line_->position_= player_->transform_.GetWorldPos();
	
	// メッシュのuvTransformのスケールを長さによって調整
	line_->mesh_->material_.uvTransform_.scale_.y = line_->length_ * 2.0f;

	line_->length_ = wepon_->GetDistance()/2.0f;
	Vector3 diff =  wepon_->transform_.GetWorldPos()- player_->transform_.GetWorldPos();
	line_->rotate_.z = -std::atan2(Math::Normalize(diff).x,Math::Normalize(diff).y);
	transform_.translate_ = line_->position_;
	transform_.rotate_ = line_->rotate_;
	transform_.scale_ = { line_->length_,0.35f,0.35f };
	line_->Update();
	if (wepon_->GetBehavior() == Behavior::kAtack) {
		colliders_.front()->SetIsActive(false);
	}
	else {
		colliders_.front()->SetIsActive(true);
	}

}

void Chain::DisplayImGui()
{
	line_->DisplayImGui();
}

void Chain::OnCollisionEnter(Collider* collider)
{
	//障害物と当たったら
	if (collider->GetGameObject()->GetObjectTag() == BaseObject::TagObstacle) {
		wepon_->DeleteChain();
	}
}
