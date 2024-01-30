#include "uribo.h"
#include "../../../../Resource/Texture/TextureManager.h"

void Uribo::Init()
{
	// 各種トランスフォームの初期化
	gageBGTransform_.Init();
	gageBGTransform_.translate_.y = 2.0f;

	// メッシュ追加
	AddMesh(&transform_, color_, "./Resources/Uribo", "Uribo.obj");
	gageBGPlane_ = AddBillboardPlane(&gageBGTransform_, bgColor_, TextureManager::Load("White2x2.png"));
	gagePlane_ = AddBillboardPlane(&gageTransform_, gageColor_, TextureManager::Load("White2x2.png"));

	// スフィアコライダーを追加
	AddColliderSphere("Roller", &transform_.translate_, &collisionScaele_.x);
	collisionScaele_ = { 10.0f,10.0f,10.0f };
	transform_.translate_.y = 3.0f;
	transform_.translate_.x = 30.0f;
	transform_.scale_ = { 1.75f,1.75f, 1.75f };

	GlobalVariables* variables = GlobalVariables::GetInstance();
	variables->CreateGroup(name_);
	variables->AddItem(name_, "DefaultHp", defaultHP_);
	variables->AddItem(name_, "collisionScale", collisionScaele_);
	variables->AddItem(name_, "scale", transform_.scale_);
	variables->AddItem(name_, "translate", transform_.translate_);
	variables->AddItem(name_, "decrementHP", decrementHP);
	SetGlobalVariables();
	hitPoint_ = defaultHP_;

	// 頂点座標を求める
	float left = (0.0f - anchorPoint_.x) * scale_.x;   // 左
	float right = (1.0f - anchorPoint_.x) * scale_.x;  // 右
	float top = (0.0f - anchorPoint_.y) * scale_.y;    // 上
	float bottom = (1.0f - anchorPoint_.y) * scale_.y; // 下

	// 頂点をずらす
	gageBGPlane_->vertices_[0].position = { right, top, 0.0f };  // 左下
	gageBGPlane_->vertices_[1].position = { left, top, 0.0f };     // 左上
	gageBGPlane_->vertices_[2].position = { left, bottom, 0.0f }; // 右下
	gageBGPlane_->vertices_[3].position = { right, bottom, 0.0f };    // 右上
}

void Uribo::Update()
{
	// ボスが死亡していない場合
	if (!isBossDead_) {
		hitPoint_ -= decrementHP;
	}

	scale_.x = Math::Linear((float)hitPoint_, 0.0f, 10.0f,(float)defaultHP_);

	// 頂点座標を求める
	float left = (0.0f - anchorPoint_.x) * scale_.x;   // 左
	float right = (1.0f - anchorPoint_.x) * scale_.x;  // 右
	float top = (0.0f - anchorPoint_.y) * scale_.y;    // 上
	float bottom = (1.0f - anchorPoint_.y) * scale_.y; // 下

	// 頂点をずらす
	gagePlane_->vertices_[0].position = { right, top, 0.0f };  // 左下
	gagePlane_->vertices_[1].position = { left, top, 0.0f };     // 左上
	gagePlane_->vertices_[2].position = { left, bottom, 0.0f }; // 右下
	gagePlane_->vertices_[3].position = { right, bottom, 0.0f };    // 右上

	gageBGTransform_.translate_ = transform_.translate_;
	gageBGTransform_.translate_.y = 6.5f;
	gageTransform_.translate_ = transform_.translate_;
	gageTransform_.translate_.y = 6.5f;

	// ビルボード平面の更新
	gageBGPlane_->UpdateBillboardMat();
	gagePlane_->UpdateBillboardMat();
}

void Uribo::DisplayImGui()
{
	transform_.DisplayImGui();
	
	ImGui::DragFloat2("gageBG_Scale", &scale_.x, 0.05f);
	ImGui::DragFloat2("gageBG_AnchorPoint", &anchorPoint_.x, 0.05f);

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
	collider;
	/*if (collider->GetGameObject()->GetObjectTag() == BaseObject::TagPlayer) {
		color_ = { 1.0f,1.0f,0.0f,1.0f };
	}*/
	
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
