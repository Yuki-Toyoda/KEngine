#include "Floor.h"

void StageFloor::Initialize(std::string name, Tag tag)
{
	// 基底クラス初期化
	BaseObject::Initialize(name, tag);
	isActive_ = true;

	// モデル読み込み
	objects_.push_back(OBJ::Create(&transform_, { 1.0f, 1.0f, 1.0f, 1.0f }, "./Resources", "Stage.obj"));

	// 
	transform_.scale_ = { 25.0f, 0.5f, 25.0f };

	// 衝突属性を設定
	collider_->SetCollisionAttribute(0xfffffffd);
	// 衝突対象を自分の属性以外に設定
	collider_->SetCollisionMask(0x00000002);

	// 当たり判定用aabb生成
	AABB* aabb = new AABB();
	aabb->Initialize(transform_.GetWorldPos(), transform_.scale_);
	// 当たり判定設定
	collider_->SetColliderShape(aabb);
	collider_->SetGameObject(this);

	// グローバル変数に調整したい値を追加
	AddGlobalVariables();
}

void StageFloor::Update()
{
	// 基底クラス更新
	BaseObject::Update();

	if (!isDestroy_) {
		// 当たり判定更新
		collider_->Update(transform_.GetWorldPos(), transform_.scale_);
		// リストに自身を登録
		collisionManager_->RegisterCollider(collider_);
	}
}

void StageFloor::Draw()
{
	// カメラオブジェクトの描画
	for (OBJ* obj : objects_) {
		if (isActive_) {
			obj->Draw();
		}
	}
}

void StageFloor::AddGlobalVariables()
{
	// 調整したい項目をグローバル変数に追加
	//globalVariables_->AddItem(objectName_.c_str(), "FloorTranslate", transform_.translate_); // 床の座標
	//globalVariables_->AddItem(objectName_.c_str(), "FloorScale", transform_.scale_); // 床の大きさ
}

void StageFloor::ApplyGlobalVariables()
{
	// 調整した値を適用
	//transform_.translate_ = globalVariables_->GetVector3Value(objectName_.c_str(), "FloorTranslate"); // 床の座標
	//transform_.scale_ = globalVariables_->GetVector3Value(objectName_.c_str(), "FloorScale"); // 床の大きさ
}

void StageFloor::OnCollisionEnter(BaseObject* object)
{

}
