#include "Damage.h"
#include "Recovery.h"
#include "App/GameObject/User/Player/Player.h"
#include "Engine/GameObject/GameObjectManager.h"
#include "Engine/GameObject/Core/Camera.h"

void Damage::Init()
{
	// ステート名の設定
	stateName_ = "Damage";
	
	// 攻撃中でない
	player_->SetIsAttacking(false);
	// コライダー無効
	player_->GetSwordLine()->GetCollider()->SetIsActive(false);

	// 速度を求める
	Matrix4x4 rotateMat = Matrix4x4::MakeRotateY(player_->transform_.rotate_.y);
	velocity_ = (velocity_ * rotateMat);

	// プレイヤーのHPが0以下の時ゲームオーバー
	if (player_->GetHP() <= 0) {
		// 死亡演出用のカメラを生成
		c = GameObjectManager::GetInstance()->CreateInstance<Camera>("StagingCamera", IObject::TagCamera);
		c->transform_.SetParent(&player_->transform_);
		c->transform_.translate_ = { 0.0f, 5.0f, -1.15f };
		c->transform_.rotate_ = { (float)std::numbers::pi / 2.0f, 0.0f, 0.0f };
		c->fov_ = 0.6f;
		c->UseThisCamera();

		// 体力UI非表示
		for (int i = 0; i < player_->GetMaxHP(); i++) {
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
	player_->skiningModels_["Player"]->animationManager_.PlayAnimation("06_Damage");
}

void Damage::Update()
{
	// アニメーションが一定に達するまで、後ろに吹っ飛ばす
	if (player_->skiningModels_["Player"]->animationManager_.GetPlayingAnimationProgress() <= blowTimeThreshold_) {
		player_->transform_.translate_ += velocity_;
	}

	// プレイヤーのHPが0以下でないとき
	if (player_->GetHP() > 0) {
		// アニメーション終了時
		if (!player_->skiningModels_["Player"]->animationManager_.GetIsPlayingAnimation("06_Damage")) {
			// 待機状態に移行
			player_->ChangeState(std::make_unique<Recovery>());
			// 以降の処理を無視
			return;
		}

		// これ以降の処理は無視して早期リターン
		return; 
	}

	// カメラのポストプロセスの強さをだんだん上げてく
	c->ppProcessor_.hsvFilter_.hsv_.saturation = KLib::Lerp<float>(0.0f, targetSaturation_, KLib::EaseInQuad(player_->skiningModels_["Player"]->animationManager_.GetPlayingAnimationProgress()));
	c->ppProcessor_.gaussian_.intensity_ = KLib::Lerp<float>(0.0f, targetBlurStrength_, KLib::EaseInQuad(player_->skiningModels_["Player"]->animationManager_.GetPlayingAnimationProgress()));

	// アニメーションが再生されていない状態かつ死亡状態でないとき
	if (!player_->skiningModels_["Player"]->animationManager_.GetIsPlayingAnimation() && !player_->GetIsDead()) {
		// 死亡状態に
		player_->SetIsDead(true);
		// 演出用にタイマーを開始
		timer_.Start(deadStagingTime_);
	}

	// 死亡時
	if (player_->GetIsDead()) {
		if (!timer_.GetIsFinish()) {
			// カメラを徐々に後ろに
			c->transform_.translate_.z += cameraMoveSpeed_;
		}
		timer_.Update();
	}
}

void Damage::DisplayImGui()
{
}
