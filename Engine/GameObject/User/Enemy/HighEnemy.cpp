#include "HighEnemy.h"
#include "../../../GlobalVariables/GlobalVariables.h"

void HighEnemy::Init()
{
	audio_ = Audio::GetInstance();
	AddMesh(&transform_, color_, "./Engine/Resource/Samples/Box", "Box.obj");
	AddColliderSphere("Enemy", &worldPos_, &transform_.scale_.x);
	transform_.scale_ = { 2.0f,2.0f,2.0f };
	gameObjectManager_ = GameObjectManager::GetInstance();

	isParent_ = false;
	//soundHandleStick_ = audio_->LoadWave("stick.wav");
	HP_ = 4;
	// 調整項目クラスに値を追加
	GlobalVariables::GetInstance()->AddItem(name_, "Translate", worldPos_);
	// 調整項目クラスから値読み込み
	transform_.translate_ = GlobalVariables::GetInstance()->GetVector3Value(name_, "Translate");
	worldPos_ = transform_.GetWorldPos();
	isActive_ = true;
}

void HighEnemy::Update()
{
	//preIsCollision_ = isCollision_;
	//if (isActive_) {
	worldPos_ = transform_.GetWorldPos();
	//鎖とぶつかった場合
	if (isParent_) {
		Vector3 world = transform_.GetWorldPos();
		Vector3 weponWorld = wepon_->transform_.GetWorldPos();
		//当たった場所から武器の方向に飛ばす
		velocity_ = Math::Normalize(weponWorld - world);
		velocity_ = velocity_ * 0.8f;
		transform_.translate_ += velocity_;
	}
	collisionCount_++;
	if (collisionCount_ >= 15) {
		isCollision_ = false;
	}
	//}
}

void HighEnemy::DisplayImGui()
{
	transform_.DisplayImGui();
	ImGui::DragFloat3("worldPos", &worldPos_.x);
	ImGui::InputInt("HP", &HP_);
	ImGui::Checkbox("isActive", &isActive_);
	if (ImGui::Button("Save")) {
		GlobalVariables::GetInstance()->SetValue(name_, "Translate", worldPos_);
		GlobalVariables::GetInstance()->SaveFile(name_);
	}
}

void HighEnemy::Reset()
{
	transform_.scale_ = { 2.0f,2.0f,2.0f };
	worldPos_ = transform_.GetWorldPos();

	isParent_ = false;
	SetObjectTag(BaseObject::TagEnemy);
	transform_.SetParent(nullptr);
	// 調整項目クラスに値を追加
	GlobalVariables::GetInstance()->AddItem(name_, "Translate", worldPos_);
	// 調整項目クラスから値読み込み
	transform_.translate_ = GlobalVariables::GetInstance()->GetVector3Value(name_, "Translate");
	//transform_.translate_ = startTranslate_;
	HP_ = 4;
	if (midEnemy_[0]) {
		midEnemy_[0]->Reset();
	}if (midEnemy_[1]) {
		midEnemy_[1]->Reset();
	}
	isActive_ = true;
}

void HighEnemy::OnCollisionEnter(Collider* collider)
{
	collider;
	//武器とぶつかった
	if (isActive_) {
		if (collider->GetGameObject()->GetObjectTag() == BaseObject::TagWeapon || collider->GetGameObject()->GetObjectTag() == BaseObject::TagChain) {
			if (isCollision_ == false) {
				if (wepon_->GetSize() == low) {
					HP_ -= 1;
				}
				else if (wepon_->GetSize() == mid) {
					HP_ -= 3;
				}
					isCollision_ = true;

					collisionCount_ = 0;
					if (HP_ <= 0) {

						isActive_ = false;
					
						for (int i = 0; i < 2; i++) {
							midEnemy_[i] = gameObjectManager_->CreateInstance<MidEnemy>("Enemy", BaseObject::TagEnemy);
							if (i == 0) {
								midEnemy_[i]->transform_.translate_ = { transform_.GetWorldPos().x + 5.0f, transform_.GetWorldPos().y + 5.0f,0.0f };
							}
							else {
								midEnemy_[i]->transform_.translate_ = { transform_.GetWorldPos().x - 5.0f, transform_.GetWorldPos().y - 5.0f,0.0f };
							}
							midEnemy_[i]->Setveclocity(Math::Normalize(midEnemy_[i]->transform_.translate_ - transform_.translate_) * 0.3f);
							midEnemy_[i]->SetWepon(wepon_);
							midEnemy_[i]->SetStartTransform();
							transform_.translate_ = { 1000.0f,0.0f,0.0f };
						}
					}
				}
			}
		
		if (collider->GetGameObject()->GetObjectTag() == BaseObject::TagWeapon) {
			if (GetObjectTag() == BaseObject::TagEnemy) {
				if (wepon_->GetSize() == high ) {
					color_ = { 1.0f,0.0f,0.0f,1.0f };

					//ワールドポジションを計算
					Vector3 world = transform_.GetWorldPos();
					Vector3 weponWorld = wepon_->transform_.GetWorldPos();

					Vector3 pos = world - weponWorld;
					Matrix4x4 rotateMat = Math::MakeRotateXYZMatrix(wepon_->transform_.rotate_ * -1.0f);
					transform_.translate_ = Math::TransformNormal(pos, rotateMat);
					isParent_ = false;
					transform_.SetParent(&wepon_->transform_);
					//ペアレントしてる数を加算
					wepon_->AddParentCount();
					wepon_->AddParentCount();
					wepon_->OnCollisionEnter(colliders_.front().get());

					SetObjectTag(BaseObject::TagWeapon);
					return;
				}
			}
		}
		//鎖とぶつかった
		else if (collider->GetGameObject()->GetObjectTag() == BaseObject::TagChain) {
			if (GetObjectTag() == BaseObject::TagEnemy )  {
				if (wepon_->GetSize() == high ) {
					color_ = { 1.0f,0.0f,0.0f,1.0f };

					isParent_ = true;
				}
			}
		}
	}


}
