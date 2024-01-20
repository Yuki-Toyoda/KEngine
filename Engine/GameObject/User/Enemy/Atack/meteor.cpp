#include "meteor.h"
#include "../../Editor/GameDataManager.h"

void Meteor::Init()
{

	velocity_.y = -1.0f;
	gameObjectmanager_ = GameObjectManager::GetInstance();
	AddMesh(&transform_, color_, "./Engine/Resource/Samples/Box", "Box.obj");
	color_ = { 0.0f,0.0f,0.0f,1.0f };
	AddColliderSphere("meteor", &transform_.translate_, &transform_.scale_.x);
	//速度
	moveSpeed_ = 0.2f;
	
}

void Meteor::Update()
{
	int isGrav = GameDataManager::GetInstance()->GetIntValue({ "MeteorParam","A" }, "IsGravity");
	if (isGrav) {
		transform_.translate_ += velocity_ * moveSpeed_;
	}
}

void Meteor::DisplayImGui()
{
}

void Meteor::OnCollisionEnter(Collider* collider)
{
	//床と当たったらオブジェクトを破壊してがれきを生成
	if (collider->GetGameObject()->GetObjectTag() == BaseObject::TagFloor) {
		Rubble* rubble;
		rubble = gameObjectmanager_->CreateInstance<Rubble>("rubble",BaseObject::TagRubble );
		rubble->transform_ = transform_;
		rubble->transform_.scale_ = { 0.5f,0.5f,0.5f };
		rubble->transform_.translate_.y = 2.0f;
		Destroy();
		return;
	}
}
