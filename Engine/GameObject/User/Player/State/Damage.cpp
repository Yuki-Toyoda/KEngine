#include "Damage.h"
#include "Root.h"
#include "../Player.h"
#include "../../../GameObjectManager.h"
#include "../../../Core/Camera.h"

void Damage::Init()
{
	stateName_ = "Damage";

	// アニメーションのループを切る
	player_->playerAnim_->isLoop_ = false;

	// プレイヤーアニメーションを変更
	player_->playerAnim_->ChangeParameter("Player_Damage", true);
	
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

		if (!player_->transform_.animations_[6].isPlay) {
			player_->transform_.animations_[6].isPlay = true;
			player_->transform_.animations_[6].animationTime = 0.0f;
			player_->transform_.animations_[0].isPlay = false;
			player_->transform_.animations_[1].isPlay = false;
			player_->transform_.animations_[2].isPlay = false;
			player_->transform_.animations_[3].isPlay = false;
			player_->transform_.animations_[4].isPlay = false;
		}
	}
	else {
		if (!player_->transform_.animations_[5].isPlay) {
			player_->transform_.animations_[5].isPlay = true;
			player_->transform_.animations_[5].animationTime = 0.0f;
			player_->transform_.animations_[0].isPlay = false;
			player_->transform_.animations_[1].isPlay = false;
			player_->transform_.animations_[2].isPlay = false;
			player_->transform_.animations_[3].isPlay = false;
			player_->transform_.animations_[4].isPlay = false;
		}
	}
}

void Damage::Update()
{
	// ダメージアニメーション中なら
	if (player_->playerAnim_->GetReadingParameterName() == "Player_Damage") {
		// アニメーションの5割が終了するまで、後ろに吹っ飛ばす
		if (player_->playerAnim_->GetAnimationProgress() <= 0.25f) {
			player_->transform_.translate_ += velocity_;
		}
		
		// プレイヤーのHPが0以下の時ゲームオーバー
		if (player_->hp_ <= 0) {

			// カメラのポストプロセスの強さをだんだん上げてく
			//c->postProcessIntensity_ = KLib::Lerp<float>(0.0f, 9.0f, KLib::EaseInQuad(player_->playerAnim_->GetAnimationProgress()));
			
			// アニメーションの9割りが終了していた場合、スティックの入力があった場合待機状態へ移行
			if (player_->playerAnim_->GetAnimationProgress() >= 0.5f && !isDead_) {
				player_->playerAnim_->Stop();
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
				//player_->sprites_[8]->color_.w = KLib::Lerp<float>(0.0f, 1.0f, KLib::EaseOutQuad(timer_.GetProgress()));

				timer_.Update();
			}
		}

		// アニメーションが終了していれば
		if (player_->playerAnim_->isEnd_) {
			// 待機状態に移行
			player_->ChangeState(std::make_unique<Root>());
		}
	}
}

void Damage::DisplayImGui()
{
}
