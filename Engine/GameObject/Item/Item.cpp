#include "Item.h"
#include "../../Resource/Texture/TextureManager.h"
#include "../../Scene/SceneManager.h"

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
	isActive_ = false;

	// 音再生インスタンス取得
	audio_ = Audio::GetInstance();
	// 音量取得
	seVolume_ = &SceneManager::GetInstance()->seVolume_;

	// アイテム使用時の音
	soundHandleUseItem_ = audio_->LoadWave("/Audio/SE/ItemSound.wav");

	// 色初期設定
	color_ = { 1.0f, 1.0f, 1.0f, 1.0f };

	// 用意した変数の初期化
	InitializeVariables();

	AddOBJ(&bufferWorldTransform_, color_, "./Resources/Item", "Item.obj");

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

	bufferWorldTransform_.GetMatWorld();

	// 回転させる
	if (rotateT_ < 1.0f) {
		// 秒数でやる
		rotateT_ += 1.0f / 60.0f;
		float pi = static_cast<float>(std::numbers::pi);
		float theta = Math::EaseOut(rotateT_, -pi * 2.0f * 2.0f, 0.0f, 1.0f);
		if (isRePop_) {
			theta = Math::EaseOut(rotateT_, -pi * (kPopTime_ * 1.0f / 60.0f) * 2.0f, 0.0f, kPopTime_ * (1.0f / 60.0f));
		}
		bufferWorldTransform_.rotate_.y = theta;
	}
	// 使用可能回数が一回なら消す
	else if (isUsed_ && !isRePop_) {
		isActive_ = false;
	}

	if (isActive_ && isRePop_ && 0 < rePopTime_) {
		rePopTime_--;
		if (rePopTime_ <= 0) {
			isJumpEnable_ = true;
			rePopTime_ = 0;
			//color_.w = 1.0f;
			objects_[0]->SetColor(color_);
		}
	}

	//ImGui::Begin("checkItem");

	//ImGui::Text("rePopTime:%d", rePopTime_);
	//ImGui::Checkbox("isUsed", &isUsed_);
	//ImGui::Checkbox("isJump", &isJumpEnable_);
	//ImGui::Checkbox("isActive", &isActive_);

	//ImGui::End();

	floatingT_ += 0.01f;
	if (1.0f < floatingT_) {
		floatingT_ = 0.0f;
	}
	float y = 0.0f;
	if (floatingT_ < 0.5f) {
		y = Math::EaseInOut(floatingT_, -0.5f, 0.5f);
	}
	else {
		y = Math::EaseInOut(floatingT_, 0.5f, -0.5f);
	}
	// 浮遊ギミック
	bufferWorldTransform_.translate_.y = y;

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
	if (isRePop_) {
		objects_[0]->SetTextureHandle(TextureManager::Load("./Item/InstanceItemTex.png"));
	}
}

void Item::AirJump()
{
	// アイテムがもう一度踏めるタイプなら
	if (isRePop_) {
		rePopTime_ = kPopTime_;
	}
	objects_[0]->SetColor(usedColor_);
	isJumpEnable_ = false;

	if (!isUsed_ && !isRePop_)
		audio_->PlayWave(soundHandleUseItem_, false, *seVolume_ * 0.35f);

	isUsed_ = true;
	rotateT_ = 0;
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
	isRePop_ = false;

	rotateT_ = 1;
	floatingT_ = 0;

	bufferWorldTransform_.Initialize();
	bufferWorldTransform_.SetParent(&transform_, 0b111);
}

void Item::DebugGui() {

	if (isRePop_) {
		objects_[0]->SetTextureHandle(TextureManager::Load("./Item/InstanceItemTex.png"));;
	}
	else {
		objects_[0]->SetTextureHandle(TextureManager::Load("./Item/ItemTex.png"));;
	}

#ifdef _DEBUG

	ImGui::Begin(objectName_.c_str());

	ImGui::Separator();

	if (ImGui::ColorPicker4("UsedColor", &usedColor_.x)) {
		AddGlobalVariables();
	}
	ImGui::End();

#endif // _DEBUG
}