#include "uribo.h"

void Uribo::Init()
{
	AddMesh(&transform_, color_, "./Engine/Resource/Samples/Box", "Box.obj");
	AddColliderOBB("Roller", &collisionScaele_, &transform_.rotate_, &transform_.translate_);
	collisionScaele_ = { 10.0f,10.0f,10.0f };
	transform_.translate_.y = 2.0f;
	transform_.translate_.x = 30.0f;
	GlobalVariables* variables = GlobalVariables::GetInstance();
	variables->CreateGroup(name_);
	variables->AddItem(name_, "DefaultHp", defaultHP_);
	variables->AddItem(name_, "collisionScale", collisionScaele_);
	variables->AddItem(name_, "scale", transform_.scale_);
	variables->AddItem(name_, "translate", transform_.translate_);
	variables->AddItem(name_, "decrementHP", decrementHP);
	SetGlobalVariables();
	hitPoint_ = defaultHP_;
}

void Uribo::Update()
{
	// ボスが死亡していない場合
	if (!isBossDead_) {
		hitPoint_ -= decrementHP;
		color_ = { 0.0f,0.0f,1.0f,1.0f };
	}
}

void Uribo::DisplayImGui()
{
	transform_.DisplayImGui();
	ImGui::DragFloat3("collisionScale", &collisionScaele_.x);
	ImGui::DragInt("defaultHP", &defaultHP_);
	ImGui::DragInt("HP", &hitPoint_);
	if (ImGui::Button("Save")) {
		GlobalVariables* variables = GlobalVariables::GetInstance();
		variables->SetValue(name_, "DefaultHp", defaultHP_);
		variables->SetValue(name_, "collisionScale", collisionScaele_);
		variables->SetValue(name_, "scale", transform_.scale_);
		variables->SetValue(name_, "translate", transform_.translate_);
		variables->SetValue(name_, "decrementHP", decrementHP);
		variables->SaveFile(name_);
	}
}

void Uribo::OnCollision(Collider* collider)
{
	if (collider->GetGameObject()->GetObjectTag() == BaseObject::TagPlayer) {
		color_ = { 1.0f,1.0f,0.0f,1.0f };
	}
	
}

void Uribo::Heal(int healPower)
{
	hitPoint_ += healPower;
}

void Uribo::SetGlobalVariables()
{
	GlobalVariables* variables = GlobalVariables::GetInstance();
	defaultHP_=variables->GetIntValue(name_, "DefaultHp");
	collisionScaele_=variables->GetVector3Value(name_, "collisionScale");
	transform_.scale_=variables->GetVector3Value(name_, "scale");
	transform_.translate_=variables->GetVector3Value(name_, "translate");
	decrementHP=variables->GetIntValue(name_, "decrementHP");
}
