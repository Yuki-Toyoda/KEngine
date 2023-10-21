#include "Item.h"
#include "../../Resource/Texture/TextureManager.h"

Item::Item()
{

}
Item::~Item()
{

}

void Item::Initialize(std::string name, Tag tag)
{
	// 基底クラス初期化
	BaseObject::Initialize(name, tag);
	isActive_ = true;

	// 色初期設定
	color_ = { 1.0f, 1.0f, 1.0f, 1.0f };

	// 用意した変数の初期化
	InitializeVariables();

	AddOBJ(&transform_, color_, "./Resources/Item", "Item.obj");

	// コライダーを設定する
	Sphere* sphere = new Sphere;
	sphere->Initialize(transform_.GetWorldPos(), radius_);
	collider_->SetColliderShape(sphere);
	collider_->SetGameObject(this);

	// グローバル変数に調整したい値を追加
	AddGlobalVariables();
	ApplyGlobalVariables();
}

void Item::Update()
{
	// 基底クラス更新
	BaseObject::Update();

	if (isActive_ && !isJumpEnable_) {
		rePopTime_--;
		if (rePopTime_ <= 0) {
			isJumpEnable_ = true;
			rePopTime_ = 0;
			//color_.w = 1.0f;
			objects_[0]->SetColor(color_);
		}
	}

	objects_;

	// デバッグ表示
	DebugGui();

	// 破壊されていない時
	if (!isDestroy_) {
		// 当たり判定更新
		collider_->Update(transform_.GetWorldPos(), radius_);
		// リストに自身を登録
		collisionManager_->RegisterCollider(collider_.get());
	}
}

void Item::Draw()
{
	DrawAllOBJ();
}

void Item::AddGlobalVariables()
{
	globalVariables_->AddItem("Item", "CollisionRadius", radius_);
	globalVariables_->AddItem("Item", "UsedRGB", Vector3(usedColor_.x, usedColor_.y, usedColor_.z));
	globalVariables_->AddItem("Item", "UsedAlpha", usedColor_.w);
	globalVariables_->SetValue("Item", "UsedRGB", Vector3(usedColor_.x, usedColor_.y, usedColor_.z));
	globalVariables_->SetValue("Item", "UsedAlpha", usedColor_.w);
}

void Item::ApplyGlobalVariables()
{
	radius_ = globalVariables_->GetFloatValue("Item", "CollisionRadius");
	Vector3 RGB = globalVariables_->GetVector3Value("Item", "UsedRGB");
	float A = globalVariables_->GetFloatValue("Item", "UsedAlpha");
	usedColor_ = { RGB.x,RGB.y,RGB.z,A };
}

void Item::OnCollisionEnter(BaseObject* object)
{
	object;
}

void Item::OnCollision(BaseObject* object)
{
	object;
}

void Item::OnCollisionExit(BaseObject* object)
{
	object;
}

void Item::SetItemInfo(const BaseStage::ItemInfo& info)
{
	transform_.translate_ = info.position_;
	isRePop_ = info.isRePop_;
	kPopTime_ = info.popTime_;
	isActive_ = true;
	isUsed_ = false;
	if (!isRePop_) {
		objects_[0]->SetTextureHandle(TextureManager::Load("./Item/InstanceItemTex.png"));;
	}
}

void Item::AirJump()
{
	// アイテムがもう一度踏めないタイプなら
	if (!isRePop_) {
		isActive_ = false;
		isJumpEnable_ = false;
		isUsed_ = true;
	}
	else {
		rePopTime_ = kPopTime_;
		isJumpEnable_ = false;
		isUsed_ = true;
		objects_[0]->SetColor(usedColor_);
	}
}

/// プライべート関数

void Item::InitializeVariables()
{

	// 色初期設定
	color_ = { 1.0f, 1.0f, 1.0f, 1.0f };
	usedColor_ = { 0.5f,0.5f,0.5f,0.5f };

	// 半径
	radius_ = 2.0f;

	rePopTime_ = 0;
	isJumpEnable_ = true;
	isUsed_ = false;
}

void Item::DebugGui() {
#ifdef _DEBUG

	ImGui::Begin(objectName_.c_str());

	ImGui::Separator();

	if (ImGui::ColorPicker4("UsedColor", &usedColor_.x)) {
		AddGlobalVariables();
	}

	if (isRePop_) {
		objects_[0]->SetTextureHandle(TextureManager::Load("./Item/ItemTex.png"));;
	}
	else {
		objects_[0]->SetTextureHandle(TextureManager::Load("./Item/InstanceItemTex.png"));;
	}
	ImGui::End();

#endif // _DEBUG
}