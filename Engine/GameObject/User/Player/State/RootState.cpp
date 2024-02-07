#include "RootState.h"
#include "../Player.h"
#include "../../../Core/Camera.h"

void RootState::Init()
{
	// 名称設定
	name_ = "Root";
	audio_= Audio::GetInstance();
	//攻撃ボタンを押しているカウントを0に
	player_->ResetAtackCount();
}

void RootState::Update()
{
	//プレイヤーがダッシュできるとき
	if (player_->GetCanDash()) {
		//チャージ完了時
		if (player_->GetAtackCount() == player_->GetmaxAtackCount() && player_->GetISTutrialDash()) {
			player_->chargeEndhandle = audio_->PlayWave(player_->chargeEnd_);
		}
		if (InputManager::AtackCharge() && player_->GetAbsorptionCount() >= kMinCount && !player_->GetIsDamaged()) {
			if (!audio_->IsPlaying(player_->chargeHandle) || player_->chargeHandle == -1) {
				player_->chargeHandle = audio_->PlayWave(player_->charge_, false);
			}
		}
		else {
			if (audio_->IsPlaying(player_->chargeHandle)) {
				audio_->StopWave(player_->chargeHandle);
			}
		}
		if (player_->GetIsDamaged()) {
			if (audio_->IsPlaying(player_->chargeHandle)) {
				audio_->StopWave(player_->chargeHandle);
			}
			// ステートをダメージ状態に変更
			player_->ChangeState(std::make_unique<DamageState>());
			// 早期リターン
			return;
		}
		// チャージ秒数が既定秒数以上のとき、かつボタンを話したとき
		if (player_->GetAtackCount() > player_->GetmaxAtackCount() && InputManager::Atacking() && player_->GetISTutrialDash()) {
			// チャージパーティクルをとめる
			player_->chargeParticleEmitter_->Reset();
			player_->chargeParticleEmitter_->SetIsPlay(false);
			// チャージ終了パーティクルをとめる
			player_->chargeFinishParticleEmitter_->Reset();
			player_->chargeFinishParticleEmitter_->SetIsPlay(false);
			// チャージの円
			player_->chargeCircleEmitter_->Reset();
			player_->chargeCircleEmitter_->SetIsPlay(false);

			//攻撃ボタンを押しているカウントが一定以上でボタンを離したときに攻撃開始
			player_->ChangeState(std::make_unique<AtackState>());
			return;
		}
		// 攻撃不可能な場合（ダッシュステートに移動
		else if (!player_->GetISTutrialDash() && InputManager::Atacking()
			&& player_->GetAbsorptionCount() >= kMinCount)
		{
			// チャージパーティクルをとめる
			player_->chargeParticleEmitter_->Reset();
			player_->chargeParticleEmitter_->SetIsPlay(false);
			// チャージ終了パーティクルをとめる
			player_->chargeFinishParticleEmitter_->Reset();
			player_->chargeFinishParticleEmitter_->SetIsPlay(false);
			// チャージの円
			player_->chargeCircleEmitter_->Reset();
			player_->chargeCircleEmitter_->SetIsPlay(false);

			// ダッシュステートへ変更
			player_->ChangeState(std::make_unique<DashState>());
			return;

		}
		// チャージ秒数が足りない場合（ダッシュステートに移動
		else if (player_->GetAtackCount() <= player_->GetmaxAtackCount() && InputManager::Atacking()
			&& player_->GetAbsorptionCount() >= kMinCount)
		{
			// チャージパーティクルをとめる
			player_->chargeParticleEmitter_->Reset();
			player_->chargeParticleEmitter_->SetIsPlay(false);
			// チャージ終了パーティクルをとめる
			player_->chargeFinishParticleEmitter_->Reset();
			player_->chargeFinishParticleEmitter_->SetIsPlay(false);
			// チャージの円
			player_->chargeCircleEmitter_->Reset();
			player_->chargeCircleEmitter_->SetIsPlay(false);

			// ダッシュステートへ変更
			player_->ChangeState(std::make_unique<DashState>());
			return;

		}
	
	

	//攻撃ボタンを押して且つ攻撃できるときにStateをAtackに
	if (InputManager::AtackCharge()&&player_->GetAbsorptionCount()>=kMinCount) {
		// 最大チャージされている場合
		if (player_->GetAtackCount() > player_->GetmaxAtackCount() && player_->GetISTutrialDash()) {
			// チャージ終了パーティクルを再生
			player_->chargeParticleEmitter_->Reset();
			player_->chargeFinishParticleEmitter_->SetIsPlay(true);
			// チャージパーティクルをとめる
			player_->chargeParticleEmitter_->SetIsPlay(false);
			// チャージの円
			player_->chargeCircleEmitter_->Reset();
			player_->chargeCircleEmitter_->SetIsPlay(false);
		}
		else {
			// チャージパーティクルを再生
			player_->chargeParticleEmitter_->SetIsPlay(true);
			// チャージ終了パーティクルをとめる
			player_->chargeFinishParticleEmitter_->Reset();
			player_->chargeFinishParticleEmitter_->SetIsPlay(false);
			// チャージの円
			player_->chargeCircleEmitter_->SetIsPlay(true);
		}

		player_->SetIsCharge(true);

		//攻撃ボタンを押いるカウントを加算
		player_->AddAtackCount();
		velocity_ = { 0.0f,0.0f,0.0f };
		//移動ベクトルをプレイヤーに渡す
		player_->SetVelocity(velocity_);
	}
	else {
		// チャージパーティクルをとめる
		player_->chargeParticleEmitter_->Reset();
		player_->chargeParticleEmitter_->SetIsPlay(false);
		// チャージの円
		player_->chargeCircleEmitter_->Reset();
		player_->chargeCircleEmitter_->SetIsPlay(false);
		player_->SetIsCharge(false);
		//攻撃ボタンを押しているカウントを0に
		player_->ResetAtackCount();

		// 移動処理
		Move();
	}
	}
else {
	// 移動処理
	Move();
	}
}

void RootState::DisplayImGui()
{
}



void RootState::Move()
{
	//// 移動ベクトル初期化
	//velocity_ = { 0.0f,0.0f,0.0f };
	
	// 入力マネージャから移動方向ベクトルを取得
	Vector3 move = Vector3(0.0f, 0.0f, 0.0f);
	move = InputManager::Move(move);

	// カメラの角度から回転行列を生成
	Matrix4x4 rotateMat = Math::MakeRotateYMatrix(player_->GetCamera()->transform_.rotate_.y);
	
	// 求めた回転行列を元に移動ベクトルを回転させる
	move = Math::Transform(move, rotateMat);
	if (move.x != 0.0f || move.y != 0.0f || move.z != 0.0f) {
		// プレイヤーの移動ベクトルと加速度を乗算
		velocity_ += move * (player_->GetMoveAcceleration()/*-((player_->GetAbsorptionAcceleration()/100.0f)*player_->GetAbsorptionCount())*/);
	}
	else {
		// 無操作状態の場合は現在速度を0に近づけていく
		velocity_ = KLib::Lerp<Vector3>(velocity_, Vector3(0.0f, 0.0f, 0.0f), player_->GetDecayAcceleration());
	}

	if (player_->GetIsDamaged()) {
		player_->moveParticleEmitter_->SetIsPlay(false);
	}
	else {
		player_->moveParticleEmitter_->SetIsPlay(true);
	}

	// 各軸の速度ベクトルが最大加速度を超過していた場合
	if (velocity_.x > player_->GetMaxMoveAcceleration()) {
		velocity_.x = player_->GetMaxMoveAcceleration();
	}else if (velocity_.x < -player_->GetMaxMoveAcceleration()) {
		velocity_.x = -player_->GetMaxMoveAcceleration();
	}// x軸
	if (velocity_.z > player_->GetMaxMoveAcceleration()) {
		velocity_.z = player_->GetMaxMoveAcceleration();
	}
	else if (velocity_.z < -player_->GetMaxMoveAcceleration()) {
		velocity_.z = -player_->GetMaxMoveAcceleration();
	}// z軸
	
	//移動ベクトルをプレイヤーに渡す
	player_->SetVelocity(velocity_);
}

void RootState::Attack()
{
	player_->SetIsAtack(true);
	player_->ChangeState(std::make_unique<AtackState>());
	return;
}
