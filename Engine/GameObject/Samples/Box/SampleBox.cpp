#include "SampleBox.h"

void SampleBox::Initialize(std::string name, Tag tag)
{
	// 基底クラス初期化
	BaseObject::Initialize(name, tag);
	isActive_ = true;

	// モデル読み込み
	AddOBJ(&transform_, color_, "./Engine/Resource/Samples/Box", "Box.obj");

	// 音声ファイル読み込み
	testSound_ = Audio::GetInstance()->LoadWave("./Engine/Resource/Samples/Audio/Alarm01.wav");

	// 入力状態取得
	input_ = Input::GetInstance();

	// 色初期設定
	color_ = { 1.0f, 1.0f, 1.0f, 1.0f };

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

void SampleBox::Update()
{

	// 基底クラス更新
	BaseObject::Update();

	if (!isDestroy_) {
		// 当たり判定更新
		collider_->Update(transform_.GetWorldPos(), colliderRadius_);
		// リストに自身を登録
		collisionManager_->RegisterCollider(collider_.get());
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

void SampleBox::Draw()
{
	DrawAllOBJ();
}

void SampleBox::AddGlobalVariables()
{
	// 調整したい項目をグローバル変数に追加
	globalVariables_->AddItem(objectName_.c_str(), "AABBRadius", colliderRadius_);
}

void SampleBox::ApplyGlobalVariables()
{
	// 調整した値を適用
	colliderRadius_ = globalVariables_->GetVector3Value(objectName_.c_str(), "AABBRadius");
}

void SampleBox::OnCollision(BaseObject* object)
{
	switch (object->GetObjectTag())
	{
	case tagCamera:
		break;
	case tagPlayer:
		break;
	case tagEnemy:
		break;
	case tagOther:
		break;
	default:
		break;
	}

	isCollision_ = true;
}

void SampleBox::OnCollisionExit(BaseObject* object)
{
	object;

}
