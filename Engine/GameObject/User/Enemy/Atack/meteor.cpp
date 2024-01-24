#include "meteor.h"
#include "../../Editor/GameDataManager.h"
#include "../../../GameObjectManager.h"
#include "../../../../Resource/Texture/TextureManager.h"

void Meteor::Init()
{
	// 攻撃範囲用トランスフォームの初期化
	attackAreaTransform_.Init();
	// 親座標とペアレント
	attackAreaTransform_.SetParent(&transform_);
	// 回転を合わせる
	attackAreaTransform_.rotate_.y = -(float)std::numbers::pi / 2.0f;

	// 速度ベクトル設定
	velocity_.y = -1.0f;
	//速度
	moveSpeed_ = 0.2f;

	/// メッシュの追加
	// 攻撃範囲表示
	AddMesh(&attackAreaTransform_, areaColor_, "./Engine/Resource/Samples/Box", "Box.obj");
	//meshes_[0]->texture_ = TextureManager::Load("./Resources", "denger.png");
	// 色の設定
	areaColor_ = { 1.f, 0.f, 0.f, 0.9f };

	// 箱表示
	AddMesh(&transform_, color_, "./Engine/Resource/Samples/Box", "Box.obj");
	// 色設定
	color_ = { 0.0f,0.0f,0.0f,1.0f };

	// コライダーを追加
	AddColliderSphere("meteor", &transform_.translate_, &transform_.scale_.x);
	
}

void Meteor::Update()
{
	int isGrav = GameDataManager::GetInstance()->GetValue<int>({ "AttackParam","Info" }, "IsGravity");
	if (isGrav) {
		transform_.translate_ += velocity_ * moveSpeed_;
		attackAreaTransform_.translate_.y = -transform_.translate_.y + 0.25f;
	}
}

void Meteor::DisplayImGui()
{
}

void Meteor::OnCollisionEnter(Collider* collider)
{
	//if()

	//床と当たったらオブジェクトを破壊してがれきを生成
	
	if (collider->GetGameObject()->GetObjectTag() == BaseObject::TagFloor) {
		
		Rubble* rubble;
		rubble = GameObjectManager::GetInstance()->CreateInstance<Rubble>("rubble",BaseObject::TagRubble );
		rubble->transform_ = transform_;
		rubble->transform_.scale_ = { 1.0f,1.0f,1.0f };
		rubble->transform_.translate_.y = 2.0f;
		Destroy();
		return;
	}
}
