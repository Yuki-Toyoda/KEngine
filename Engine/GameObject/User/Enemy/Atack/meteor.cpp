#include "meteor.h"
#include "../../Editor/GameDataManager.h"
#include "../../../GameObjectManager.h"
#include "../../../../Resource/Texture/TextureManager.h"
#include "../../Random/RandomEngine.h"
void Meteor::Init()
{
	// 攻撃範囲用トランスフォームの初期化
	attackAreaTransform_.Init();
	// 親座標とペアレント
	attackAreaTransform_.SetParent(&transform_);
	// 回転を合わせる
	attackAreaTransform_.rotate_.y = -(float)std::numbers::pi / 2.0f;

	// 速度ベクトル設定
	velocity_.y = -1.0f;
	//速度
	moveSpeed_ = 0.2f;

	/// メッシュの追加
	// 攻撃範囲表示
	AddMesh(&attackAreaTransform_, areaColor_, "./Engine/Resource/Samples/Box", "Box.obj");
	//meshes_[0]->texture_ = TextureManager::Load("./Resources", "denger.png");
	// 色の設定
	areaColor_ = { 1.f, 0.f, 0.f, 0.8f };

	// 箱表示
	AddMesh(&transform_, color_, "./Engine/Resource/Samples/Box", "Box.obj");
	// 色設定
	color_ = { 0.0f,0.0f,0.0f,1.0f };

	// コライダーを追加
	AddColliderSphere("meteor", &transform_.translate_, &transform_.scale_.x);
	
}

void Meteor::Update()
{
	//速度
	moveSpeed_ = gameManager_->meteorSpeed_;

	int isGrav = GameDataManager::GetInstance()->GetValue<int>({ "AttackParam","Info" }, "IsGravity");
	if (isGrav) {
		transform_.translate_ += velocity_ * moveSpeed_;
		attackAreaTransform_.translate_.y = -transform_.translate_.y + 0.25f;
	}
}

void Meteor::DisplayImGui()
{
}

void Meteor::OnCollisionEnter(Collider* collider)
{
	//if()

	//床と当たったらオブジェクトを破壊してがれきを生成
	
	if (collider->GetGameObject()->GetObjectTag() == BaseObject::TagFloor) {
		float angle= RandomEngine::GetRandom(0.0f, 10.0f);
		Rubble* rubble[4];
		for (int i = 0; i< 4; i++) {
			float distance = RandomEngine::GetRandom(0.0f, gameManager_->rubbleRange);
			float theta=0.0f;
			//上下左右にそれぞれ
			if (i == 0) {
				theta = 0.0f;
			}
			else if (i == 1) {
				theta = 3.141592f;
			}
			else if (i == 2) {
				theta = 3.141592f / 2.0f;
			}
			else if (i == 3) {
				theta = 3.141592f*1.5f;
			}
			//上下左右から一定角度ずらす
			theta *= angle;
			rubble[i] = GameObjectManager::GetInstance()->CreateInstance<Rubble>("rubble", BaseObject::TagRubble);
			Vector3 pos;
			//Lerpのゴールとスタートを設定
			pos.x = transform_.translate_.x + std::cosf(theta) * (distance + transform_.scale_.x);
			pos.z = transform_.translate_.z + std::sinf(theta) * (distance + transform_.scale_.z);
			pos.y = gameManager_->rubbleSize_ + 1.0f;
			rubble[i]->SetStart(transform_.translate_);
			rubble[i]->SetGoal(pos);
			rubble[i]->lerpTime_ = gameManager_->rubbleAliveCount_;
			rubble[i]->moveTime_ = gameManager_->rubblMoveTime_;
			rubble[i]->SetgameManager(gameManager_);
			//もしゴール位置が枠の外ならDestroy
			if (std::abs(pos.x + gameManager_->rubbleSize_) >= 55.0f || std::abs(pos.z + gameManager_->rubbleSize_) >= 55.0f) {
				rubble[i]->Destroy();
			}
		}
		Destroy();
		return;
	}
}
