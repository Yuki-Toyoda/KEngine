#include "Catapult.h"
#include "../../Resource/Texture/TextureManager.h"

#include "../Player/Player.h"

#include <numbers>

Catapult::Catapult()
{

}
Catapult::~Catapult()
{

}

void Catapult::Initialize(std::string name, Tag tag)
{
	// 基底クラス初期化
	BaseObject::Initialize(name, tag);
	isActive_ = false;

	// 色初期設定
	color_ = { 1.0f, 1.0f, 1.0f, 1.0f };

	// 用意した変数の初期化
	InitializeVariables();

	AddOBJ(&transform_, color_, "./Resources/Catapult/base", "Catapult.obj");
	//AddOBJ(&transform_, color_, "./Resource/Catapult/UpAllow", "UpAllow.obj");

	// コライダーを設定する
	Sphere* sphere = new Sphere;
	sphere->Initialize(transform_.GetWorldPos(), radius_);
	collider_->SetColliderShape(sphere);
	collider_->SetGameObject(this);

	// グローバル変数に調整したい値を追加
	AddGlobalVariables();
	ApplyGlobalVariables();
}

void Catapult::Update()
{
	// 基底クラス更新
	BaseObject::Update();

	// 飛んだあと
	if (isActive_ && !isJumpEnable_) {
		rePopTime_--;
		if (rePopTime_ <= 0) {
			isJumpEnable_ = true;
			rePopTime_ = 0;
			//color_.w = 1.0f;
			objects_[0]->SetColor(color_);
			//objects_[1]->SetColor(color_);
		}
	}

	UpdateCatapult();

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

void Catapult::Draw()
{
	DrawAllOBJ();
}

void Catapult::AddGlobalVariables()
{
	globalVariables_->CreateGroup("Catapult");
	globalVariables_->AddItem("Catapult", "CollisionRadius", radius_);
	globalVariables_->AddItem("Catapult", "UsedRGB", Vector3(usedColor_.x, usedColor_.y, usedColor_.z));
	globalVariables_->AddItem("Catapult", "UsedAlpha", usedColor_.w);
	globalVariables_->SetValue("Catapult", "UsedRGB", Vector3(usedColor_.x, usedColor_.y, usedColor_.z));
	globalVariables_->SetValue("Catapult", "UsedAlpha", usedColor_.w);

	globalVariables_->AddItem("Catapult", "allowScale", allowTransform_.scale_);
	globalVariables_->AddItem("Catapult", "allowRotate", allowTransform_.rotate_);
	globalVariables_->AddItem("Catapult", "allowTranslate", allowTransform_.translate_);
}

void Catapult::ApplyGlobalVariables()
{
	radius_ = globalVariables_->GetFloatValue("Catapult", "CollisionRadius");
	Vector3 RGB = globalVariables_->GetVector3Value("Catapult", "UsedRGB");
	float A = globalVariables_->GetFloatValue("Catapult", "UsedAlpha");
	usedColor_ = { RGB.x,RGB.y,RGB.z,A };
}

void Catapult::OnCollisionEnter(BaseObject* object)
{
	object;
}

void Catapult::OnCollision(BaseObject* object)
{
	object;
}

void Catapult::OnCollisionExit(BaseObject* object)
{
	object;
}


void Catapult::SetCatapultInfo(const BaseStage::CatapultInfo& info)
{
	//transform_.translate_ = info.position_;
	kTheta_ = info.theta_;
	kLength_ = info.length_;
	isRePop_ = info.isRePop_;
	kPopTime_ = info.popTime_;
	isActive_ = true;
	if (!isRePop_) {
		//objects_[0]->SetTextureHandle(TextureManager::Load("./Item/InstanceItemTex.png"));;
	}
}

float Catapult::GetTheta() const
{
	return player_->GetGearTheta() + kTheta_;
}

void Catapult::AirJump()
{
	// アイテムがもう一度踏めないタイプなら
	if (!isRePop_) {
		isActive_ = false;
		isJumpEnable_ = false;
	}
	else {
		rePopTime_ = kPopTime_;
		isJumpEnable_ = false;
		objects_[0]->SetColor(usedColor_);
		//objects_[1]->SetColor(usedColor_);
	}
}

/// プライべート関数

void Catapult::InitializeVariables()
{

	// 色初期設定
	color_ = { 1.0f, 1.0f, 1.0f, 1.0f };
	usedColor_ = { 0.5f,0.5f,0.5f,0.5f };

	kTheta_ = 0.0f;
	kLength_ = 1.0f;

	// 半径
	radius_ = 2.0f;

	rePopTime_ = 0;
	isJumpEnable_ = true;

	allowTransform_.Initialize();

	// プレイヤーの情報を取る
	BaseObject* buff = GameObjectManager::GetInstance()->GetGameObject(BaseObject::tagPlayer).front();
	player_ = dynamic_cast<Player*>(buff);
	assert(player_ != nullptr);
}

void Catapult::UpdateCatapult()
{
	// 歯車の回転によって回転の補正をする
	float rotateTheta_ = player_->GetGearTheta() + kTheta_;
	// float にキャストしたπを取得
	float pi = static_cast<float>(std::numbers::pi);
	// 角度を一回転の中に収める
	if (pi < kTheta_) {
		rotateTheta_ = -pi + (pi - rotateTheta_);
	}
	else if (rotateTheta_ < -pi) {
		rotateTheta_ = pi + (pi + rotateTheta_);
	}
	// 位置を変える
	Vector2 direct = { std::cosf(rotateTheta_),std::sinf(rotateTheta_) };
	direct.x = direct.x * (kLength_ - 0.5f);
	direct.y = direct.y * (kLength_ - 0.5f);
	transform_.translate_ = Vector3(direct.x, direct.y, transform_.translate_.z);
	// ついでに回転させる
	transform_.rotate_.z = rotateTheta_ + pi / 2.0f;

}


void Catapult::DebugGui() {
#ifdef _DEBUG

	ImGui::Begin(objectName_.c_str());

	ImGui::Separator();

	if (ImGui::ColorPicker4("UsedColor", &usedColor_.x)) {
		AddGlobalVariables();
	}

	if (isRePop_) {
		//objects_[0]->SetTextureHandle(TextureManager::Load("./Item/ItemTex.png"));;
	}
	else {
		//objects_[0]->SetTextureHandle(TextureManager::Load("./Item/InstanceItemTex.png"));;
	}
	ImGui::End();

#endif // _DEBUG
}
