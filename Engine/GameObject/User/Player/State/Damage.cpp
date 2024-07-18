#include "Damage.h"
#include "Root.h"
#include "../Player.h"
#include "../../../GameObjectManager.h"
#include "../../../Core/Camera.h"

void Damage::Init()
{
	stateName_ = "Damage";
	
	// 攻撃中でない
	player_->isAttacking_ = false;

	// 速度を求める
	velocity_ = { 0.0f, 0.0f, -0.1f };
	Matrix4x4 rotateMat = Matrix4x4::MakeRotateY(player_->transform_.rotate_.y);
	velocity_ = (velocity_ * rotateMat);

	// プレイヤーのHPが0以下の時ゲームオーバー
	if (player_->hp_ <= 0) {
		// カメラ移動
		c = GameObjectManager::GetInstance()->CreateInstance<Camera>("StagingCamera", IObject::TagCamera);
		c->transform_.SetParent(&player_->transform_);
		c->transform_.translate_ = { 0.0f, 5.0f, -1.15f };
		c->transform_.rotate_ = { (float)std::numbers::pi / 2.0f, 0.0f, 0.0f };
		c->fov_ = 0.6f;
		c->UseThisCamera();
	}

	player_->skiningModels_[0]->animationManager_.PlayAnimation("06_Damage");
}

void Damage::Update()
{
	// ダメージアニメーション中なら
	if (player_->skiningModels_[0]->animationManager_.GetIsPlayingAnimation("06_Damage") && !recovering_ || player_->hp_ <= 0) {
		// アニメーションが一定に達するまで、後ろに吹っ飛ばす
		if (player_->skiningModels_[0]->animationManager_.GetPlayingAnimationProgress() <= 0.25f) {
			player_->transform_.translate_ += velocity_;
		}
		
		// プレイヤーのHPが0以下の時ゲームオーバー
		if (player_->hp_ <= 0) {
			
			// カメラのポストプロセスの強さをだんだん上げてく
			c->ppProcessor_.grayScale_.intensity_ = KLib::Lerp<float>(0.0f, 1.0f, KLib::EaseInQuad(player_->skiningModels_[0]->animationManager_.GetPlayingAnimationProgress()));
			//c->ppProcessor_.gaussian_.intensity_ = KLib::Lerp<float>(0.0f, 3.0f, KLib::EaseInQuad(player_->skiningModels_[0]->animationManager_.GetPlayingAnimationProgress()));

			// アニメーションの9割りが終了していた場合、スティックの入力があった場合待機状態へ移行
			if (!player_->skiningModels_[0]->animationManager_.GetIsPlayingAnimation() && !isDead_) {
				isDead_ = true;
				timer_.Start(1.5f);
			}

			if (isDead_) {
				if (timer_.GetIsFinish()) {
					player_->isDead_ = true;
				}
				else {
					c->transform_.translate_.z -= 0.0025f;
				}
				
				// フェード演出スプライトを暗転させる
				player_->sprites_[8]->color_.w = KLib::Lerp<float>(0.0f, 1.0f, KLib::EaseOutQuad(timer_.GetProgress()));

				timer_.Update();
			}
		}
	}
	else { // アニメーションが終了したら
		// リカバリーを行う
		if (!player_->skiningModels_[0]->animationManager_.GetIsPlayingAnimation("07_Recovery") && !player_->isDead_) {
			if (recovering_) {
				// 待機状態に移行
				player_->ChangeState(std::make_unique<Root>());

				return;
			}
			else {
				player_->skiningModels_[0]->animationManager_.PlayAnimation("07_Recovery");

				recovering_ = true;
			}

		}
	}
}

void Damage::DisplayImGui()
{
}
