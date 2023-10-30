#include "Weapon.h"

void Weapon::Initialize(std::string name, Tag tag)
{
	// 基底クラス初期化
	BaseObject::Initialize(name, tag);
	isActive_ = true;

	// obj読み込み
	AddOBJ(&transform_, color_, "./Engine/Resource/Samples/Player/Weapon", "Weapon.obj");

	// 攻撃状態false
	isAttack_ = false;

	// 当たり判定用aabb生成
	AABB* aabb = new AABB();
	aabb->Initialize(transform_.GetWorldPos(), { 1.0f, 1.0f, 1.0f });
	// 当たり判定設定
	collider_->SetColliderShape(aabb);
	collider_->SetGameObject(this);

	// グローバル変数に調整したい値を追加
	AddGlobalVariables();
}

void Weapon::Update()
{
	if (isAttack_) {

	}
}

void Weapon::Draw()
{
}

void Weapon::AddGlobalVariables()
{
}

void Weapon::ApplyGlobalVariables()
{
}
