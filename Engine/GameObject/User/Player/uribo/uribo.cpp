#include "uribo.h"
#include "../../../../Resource/Texture/TextureManager.h"

void Uribo::Init()
{
	// 各種トランスフォームの初期化
	bodyTransform_.Init();
	feedAreaTransform_.Init();
	feedAreaTransform_.rotate_.x = (float)std::numbers::pi / 2.0f;
	// 親子付け
	bodyTransform_.SetParent(&transform_);
	feedAreaTransform_.SetParent(&transform_);

	// メッシュ追加
	AddMesh(&bodyTransform_, color_, "./Resources/Uribo", "Uribo.obj");
	feedAreaMesh_ = AddMesh(&feedAreaTransform_, areaColor_, "./Engine/Resource/Samples/Plane", "Plane.obj");
	feedAreaMesh_->texture_ = TextureManager::Load("FeedAreaTex_Enable.png");
	feedAreaMesh_->texture_ = TextureManager::Load("FeedAreaTex_Disable.png");

	// スフィアコライダーを追加
	AddColliderSphere("Roller", &transform_.translate_, &collisionScale_.x);
	collisionScale_ = { 10.0f,10.0f,10.0f };
	transform_.translate_.y = 3.0f;
	transform_.translate_.x = 30.0f;
	transform_.scale_ = { 1.75f,1.75f, 1.75f };

	GlobalVariables* variables = GlobalVariables::GetInstance();
	variables->CreateGroup(name_);
	variables->AddItem(name_, "DefaultHp", defaultHP_);
	variables->AddItem(name_, "collisionScale", collisionScale_);
	variables->AddItem(name_, "scale", transform_.scale_);
	variables->AddItem(name_, "translate", transform_.translate_);
	variables->AddItem(name_, "decrementHP", decrementHP);
	SetGlobalVariables();
	hitPoint_ = defaultHP_;

	// エリアをコライダーの大きさによって広げる
	feedAreaTransform_.scale_ = {collisionScale_.x / 2.0f, collisionScale_.z / 2.0f, 1.0f };
	feedAreaTransform_.translate_.y = -(transform_.scale_.y / 2.0f) + 0.1f;

}

void Uribo::Update()
{
	// ボスが死亡していない場合
	if (!isBossDead_) {
		hitPoint_ -= decrementHP;
	}

	if (feedAreaMesh_->texture_ != TextureManager::Load("FeedAreaTex_Disable.png")) {
		feedAreaMesh_->texture_ = TextureManager::Load("FeedAreaTex_Disable.png");
	}
}

void Uribo::DisplayImGui()
{
	transform_.DisplayImGui();

	feedAreaTransform_.DisplayImGuiWithTreeNode("FeedArea");
	ImGui::ColorPicker4("areaColor", &areaColor_.x);

	ImGui::DragFloat3("collisionScale", &collisionScale_.x);
	ImGui::DragInt("defaultHP", &defaultHP_);
	ImGui::DragInt("HP", &hitPoint_);
	if (ImGui::Button("Save")) {
		GlobalVariables* variables = GlobalVariables::GetInstance();
		variables->SetValue(name_, "DefaultHp", defaultHP_);
		variables->SetValue(name_, "collisionScale", collisionScale_);
		variables->SetValue(name_, "scale", transform_.scale_);
		variables->SetValue(name_, "translate", transform_.translate_);
		variables->SetValue(name_, "decrementHP", decrementHP);
		variables->SaveFile(name_);
	}
}

void Uribo::OnCollision(Collider* collider)
{
	// エリア内にプレイヤーがいたらテクスチャを変更する
	if (collider->GetGameObject()->GetObjectTag() == BaseObject::TagPlayer) {
		feedAreaMesh_->texture_ = TextureManager::Load("FeedAreaTex_Enable.png");
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
	collisionScale_=variables->GetVector3Value(name_, "collisionScale");
	transform_.scale_=variables->GetVector3Value(name_, "scale");
	transform_.translate_=variables->GetVector3Value(name_, "translate");
	decrementHP=variables->GetIntValue(name_, "decrementHP");
}
