#include "SampleBox.h"
#include "../../../Resource/Texture/TextureManager.h"

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

	// 番号
	intNum_ = 0.0f;
	testNum_ = 0;

	min_ = 0.0f;
	max_ = 100.0f;

	playX_ = 1;
	playY_ = 1;

	backGroundColor_ = { 0.25f, 0.25f, 0.25f, 1.0f };
	gageColor_ = { 1.0f, 1.0f, 1.0f, 1.0f };

	gage_ = std::make_unique<Gage>();
	gage_->Initialize(&intNum_, 0.0f, 100.0f, backGroundColor_, gageColor_, { 100.0f, 100.0f }, { 100.0f, 16.0f }, {0.0f, 0.0f});

	counter_ = std::make_unique<Counter>();
	counter_->Initialize(&testNum_, { 300.0f, 300.0f }, { 96.0f, 96.0f }, -32.0f);

	animSpriteTexture1_ = TextureManager::Load("./Engine/Resource/Samples/Texture", "NumberSheets.png");
	animSpriteTexture2_ = TextureManager::Load("./Engine/Resource/Samples/Texture", "AlphabetSheet.png");
	animationSprite_ = std::make_unique<AnimationSprite>();
	animationSprite_->Initialize(animSpriteTexture1_, { 100.0f, 100.0f }, { 64.0f, 64.0f }, {0.5f, 0.5f}, {1.0f, 1.0f, 1.0f, 1.0f}, 13, {512.0f, 512.0f}, true, 0.5f);

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
		if (ImGui::Button("PlayTestSound")) {
			voiceHundle = Audio::GetInstance()->PlayWave(testSound_);
		}
	}

	ImGui::DragFloat3("uvScale", &objects_[0]->uvTransform_.scale_.x, 0.01f);
	ImGui::DragFloat3("uvRotate", &objects_[0]->uvTransform_.rotate_.x, 0.01f);
	ImGui::DragFloat3("uvTranslate", &objects_[0]->uvTransform_.translate_.x, 0.01f);

	// ゲージ更新
	ImGui::DragFloat("num", &intNum_, 0.5f);
	ImGui::Checkbox("gage - isActive", &gage_->isActive_);
	ImGui::DragFloat2("gagePosition", &gage_->position_.x, 0.5f);
	ImGui::DragFloat2("gageSize", &gage_->size_.x, 0.5f);
	ImGui::DragFloat("gageMin_", &min_, 0.5f);
	ImGui::DragFloat("gageMax_", &max_, 0.5f);
	ImGui::ColorEdit4("gageBackGroundColor", &backGroundColor_.x);
	ImGui::ColorEdit4("gageColor", &gageColor_.x);
	gage_->Update();
	gage_->backGroundColor_ = backGroundColor_;
	gage_->gageColor_ = gageColor_;
	gage_->SetMin(min_);
	gage_->SetMax(max_);

	// カウンター更新
	ImGui::Checkbox("counter - isActive", &counter_->isActive_);
	ImGui::DragInt("Intnum", &testNum_, 0.5f);
	ImGui::DragFloat2("counterPosition", &counter_->position_.x, 0.5f);
	ImGui::DragFloat2("counterSize", &counter_->size_.x, 0.5f);
	ImGui::DragFloat("counterLineSpace", &counter_->lineSpace_, 0.5f);
	counter_->Update();

	// アニメーションスプライト更新
	ImGui::Checkbox("animationSprite - isActive", &animationSprite_->isActive_);
	ImGui::DragFloat2("animPosition", &animationSprite_->position_.x, 0.5f);
	ImGui::DragFloat2("animSize", &animationSprite_->size_.x, 0.5f);
	ImGui::SliderInt("playX", &playX_, 1 , 10);
	ImGui::SliderInt("playY", &playY_, 1 , 10);
	if (ImGui::Button("ChangeTex1"))
		animationSprite_->ChangeAnimationSheets(animSpriteTexture1_, {512.0f, 512.0f}, 13, 0, 0);

	if (ImGui::Button("ChangeTex2"))
		animationSprite_->ChangeAnimationSheets(animSpriteTexture2_, { 512.0f, 512.0f }, 29 ,0 , 0);
	if (ImGui::Button("Replay"))
		animationSprite_->Replay();
	if (ImGui::Button("moveSelectFrame"))
		animationSprite_->ChangeSelectedFrame(playX_, playY_);
	if (ImGui::Button("setBeginFrame"))
		animationSprite_->SetBeginFrame(playX_, playY_);
	animationSprite_->Update();

	ImGui::End();

#endif // _DEBUG
}

void SampleBox::Draw()
{
	DrawAllOBJ();
}

void SampleBox::SpriteDraw()
{
	gage_->Draw();
	counter_->Draw();
	animationSprite_->Draw();
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
