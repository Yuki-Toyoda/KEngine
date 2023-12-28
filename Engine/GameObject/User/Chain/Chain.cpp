#include "Chain.h"
#include "../../../Resource/Texture/TextureManager.h"
void Chain::Init()
{
	line_ = std::make_unique<Line>();
	line_->Init("ChainLine",  transform_.translate_,{ 0.35f,0.35f }, line_->length_, TextureManager::Load("./Engine/Resource/Samples/Box", "uvChecker.png"));
	line_->AddCollider("Chain", this);
}

void Chain::Update()
{
	
	line_->position_= player_->transform_.GetWorldPos();
	
	line_->length_ = wepon_->GetDistance()/2.0f;
	Vector3 diff =  wepon_->transform_.GetWorldPos()- player_->transform_.GetWorldPos();
	line_->rotate_.z = -std::atan2(Math::Normalize(diff).x,Math::Normalize(diff).y);
	transform_.translate_ = line_->position_;
	transform_.rotate_ = line_->rotate_;
	transform_.scale_ = { line_->length_,0.35f,0.35f };
	//line_->rotate_.y = std::atan2(player_->transform_.GetWorldPos().y, wepon_->transform_.GetWorldPos().y);
	line_->Update();
	//line_->rotate_.y = std::atan2(player_->transform_.GetWorldPos().y, wepon_->transform_.GetWorldPos().y);
}

void Chain::DisplayImGui()
{
	line_->DisplayImGui();
}

void Chain::OnCollisionEnter(Collider* collider)
{
	if (collider->GetGameObject()->GetObjectTag() == BaseObject::TagObstacle) {
		wepon_->DeleteChain();
	}
}
