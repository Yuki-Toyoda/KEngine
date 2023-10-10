#include "Box.h"

void Box::Initialize(std::string name, Tag tag)
{
	// 基底クラス初期化
	BaseObject::Initialize(name, tag);
	isActive_ = true;

	// モデル読み込み
	objects_.push_back(OBJ::Create(&transform_, { 1.0f, 1.0f, 1.0f, 1.0f }, "./Resources", "Box.obj"));

	// 音声ファイル読み込み
	testSound_ = Audio::GetInstance()->LoadWave("Alarm01.wav");

	// 入力状態取得
	input_ = Input::GetInstance();

	// 色初期設定
	color_ = { 1.0f, 1.0f, 1.0f, 1.0f };

	// 衝突属性を設定
	collider_->SetCollisionAttribute(0xfffffffd);
	// 衝突対象を自分の属性以外に設定
	collider_->SetCollisionMask(0x00000002);

	// 当たり判定用aabb生成
	colliderRadius_ = { 1.0f, 1.0f, 1.0f };
	AABB* aabb = new AABB();
	aabb->Initialize(transform_.GetWorldPos(), colliderRadius_);
	// 当たり判定設定
	collider_->SetColliderShape(aabb);
	collider_->SetGameObject(this);

	// グローバル変数に調整したい値を追加
	AddGlobalVariables();
}

void Box::Update()
{

	// 基底クラス更新
	BaseObject::Update();

	if (!isDestroy_) {
		// 当たり判定更新
		collider_->Update(transform_.GetWorldPos(), colliderRadius_);
		// リストに自身を登録
		collisionManager_->RegisterCollider(collider_);
	}

	if (isCollision_) {
		// とりあえず色を変える
		color_ = { 1.0f, 0.0f, 0.0f, 1.0f };
		isCollision_ = false;
	}
	else {
		// とりあえず色を変える
		color_ = { 1.0f, 1.0f, 1.0f, 1.0f };
	}

	// 色設定
	objects_[0]->SetColor(color_);

#ifdef _DEBUG

	// Imgui
	ImGui::Begin(objectName_.c_str());
	if (!Audio::GetInstance()->IsPlaying(voiceHundle) || voiceHundle == -1) {
		ImGui::Checkbox("input?", &isInput_);
		if (ImGui::Button("PlayTestSound")) {
			voiceHundle = Audio::GetInstance()->PlayWave(testSound_);
		}
	}
	ImGui::ColorEdit4("color", &color_.x);
	ImGui::End();

#endif // _DEBUG
}

void Box::Draw()
{
	// オブジェクトの描画
	for (OBJ* obj : objects_) {
		if (isActive_) {
			obj->SetWorldTransform(&transform_);
			obj->Draw();
		}
	}
}

void Box::AddGlobalVariables()
{
	// 調整したい項目をグローバル変数に追加
	globalVariables_->AddItem(objectName_.c_str(), "AABBRadius", colliderRadius_);
}

void Box::ApplyGlobalVariables()
{
	// 調整した値を適用
	colliderRadius_ = globalVariables_->GetVector3Value(objectName_.c_str(), "AABBRadius");
}

void Box::OnCollision(BaseObject* object)
{
	switch (object->GetObjectTag())
	{
	case Camera:
		break;
	case Player:
		break;
	case Enemy:
		break;
	case Other:
		break;
	default:
		break;
	}

	isCollision_ = true;
}

void Box::OnCollisionExit(BaseObject* object)
{
	object;

}
