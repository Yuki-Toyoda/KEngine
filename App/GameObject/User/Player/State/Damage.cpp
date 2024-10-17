#include "Damage.h"
#include "Root.h"
#include "App/GameObject/User/Player/Player.h"
#include "App/GameObject/User/GameManger/GameManager.h"
#include "Engine/GameObject/GameObjectManager.h"
#include "Engine/GameObject/Core/Camera.h"

void Damage::Init()
{
	// ステート名の設定
	stateName_ = "Damage";
	
	// 攻撃中でない
	player_->isAttacking_ = false;

	// 速度を求める
	velocity_ = { 0.0f, 0.0f, -0.1f };
	Matrix4x4 rotateMat = Matrix4x4::MakeRotateY(player_->transform_.rotate_.y);
	velocity_ = (velocity_ * rotateMat);

	// プレイヤーのHPが0以下の時ゲームオーバー
	if (player_->hp_ <= 0) {
		// 死亡演出用のカメラを生成
		c = GameObjectManager::GetInstance()->CreateInstance<Camera>("StagingCamera", IObject::TagCamera);
		c->transform_.SetParent(&player_->transform_);
		c->transform_.translate_ = { 0.0f, 5.0f, -1.15f };
		c->transform_.rotate_ = { (float)std::numbers::pi / 2.0f, 0.0f, 0.0f };
		c->fov_ = 0.6f;
		c->UseThisCamera();
		c->ppProcessor_.bloom_.threshold_ = 0.25f;

		// 体力UI非表示
		for (int i = 0; i < 6; i++) {
			// ハートスプライト名の取得
			std::string hert = "Hert" + std::to_string(i);
			player_->sprites_[hert]->isActive_ = false;
		}
		// ボタンUI非表示
		player_->sprites_["AButton"]->isActive_		= false;
		player_->sprites_["SwordIcon"]->isActive_	= false;
		player_->sprites_["BButton"]->isActive_		= false;
	}

	// ダメージアニメーションを再生
	player_->skiningModels_["Player"]->animationManager_.PlayAnimation("06_Damage", 0.1f);
}

void Damage::Update()
{
	// ダメージアニメーション中なら
	if (player_->skiningModels_["Player"]->animationManager_.GetIsPlayingAnimation("06_Damage") && !recovering_ || player_->hp_ <= 0) {
		// アニメーションが一定に達するまで、後ろに吹っ飛ばす
		if (player_->skiningModels_["Player"]->animationManager_.GetPlayingAnimationProgress() <= 0.25f) {
			player_->transform_.translate_ += velocity_;
		}
		
		// プレイヤーのHPが0以下の時ゲームオーバー
		if (player_->hp_ <= 0) {
			// カメラのポストプロセスの強さをだんだん上げてく
			c->ppProcessor_.hsvFilter_.hsv_.saturation = KLib::Lerp<float>(0.0f, -1.0f, KLib::EaseInQuad(player_->skiningModels_["Player"]->animationManager_.GetPlayingAnimationProgress()));
			c->ppProcessor_.gaussian_.intensity_		= KLib::Lerp<float>(0.0f, 3.0f, KLib::EaseInQuad(player_->skiningModels_["Player"]->animationManager_.GetPlayingAnimationProgress()));

			// アニメーションが再生されていない状態かつ死亡状態でないとき
			if (!player_->skiningModels_["Player"]->animationManager_.GetIsPlayingAnimation() && !isDead_) {
				// 死亡状態に
				isDead_ = true;
				// 演出用にタイマーを開始
				timer_.Start(1.5f);
				// フェードアウト開始
				player_->gameManager_->StartFade(GameManager::FADEOUT, 1.5f);
			}

			if (isDead_) {
				if (!timer_.GetIsFinish()) {
					// カメラを徐々に後ろに
					c->transform_.translate_.z -= 0.0025f;
				}
				timer_.Update();
			}
		}
	}
	else { // アニメーションが終了したら
		// リカバリーを行う
		if (!player_->skiningModels_["Player"]->animationManager_.GetIsPlayingAnimation("07_Recovery") && !player_->isDead_) {
			if (recovering_) {
				// 待機状態に移行
				player_->ChangeState(std::make_unique<Root>());
				// 以降の処理を無視
				return;
			}
			else {
				// 復帰アニメーションを再生
				player_->skiningModels_["Player"]->animationManager_.PlayAnimation("07_Recovery");
				// 復帰状態に
				recovering_ = true;
			}

		}
	}
}

void Damage::DisplayImGui()
{
}
