#include "Player.h"
#include "PlayerAnimManager.h"
#include "../../../GlobalVariables/GlobalVariables.h"
#include "../../Core/Camera.h"
#include "../../GameObjectManager.h"
#include "../FeedVegetable/FeedVegetable.h"
#include "../Enemy/Boss.h"
void Player::Init()
{
	// 音再生クラスのインスタンス取得
	audio_ = Audio::GetInstance();

	/// 効果音のロード
	// 攻撃
	attackSE_ = audio_->LoadWave("./Resources/Audio/SE/attack.wav");
	// 食べる
	eatSE_ = audio_->LoadWave("./Resources/Audio/SE/eat.wav");
	// 餌を与える
	feedSE_ = audio_->LoadWave("./Resources/Audio/SE/feed.wav");
	callapseSE= audio_->LoadWave("./Resources/Audio/SE/callapse.wav");
	// 移動ベクトル初期化
	velocity_ = { 0.0f,0.0f,0.0f };
	// 球のコライダーを追加
	AddColliderSphere("Enemy", &worldPos_, &transform_.scale_.x);

	// 行動状態を待機状態に変更
	ChangeState(std::make_unique<RootState>());

	// ゲームオーバーフラグ
	isGameOver_ = false;
	atackPushCount_ = 0;
	// 調整項目クラスのインスタンス取得
	GlobalVariables* variables = GlobalVariables::GetInstance();
	// 調整項目クラスのグループ生成
	variables->CreateGroup(name_);

	/// 調整項目クラスに値を追加する
	variables->AddItem(name_, "MoveAcceleration", moveAcceleration_);
	variables->AddItem(name_, "MaxMoveAcceleration", kMaxMoveAcceleration_);
	variables->AddItem(name_, "DecayAcceleration", decayAcceleration_);
	variables->AddItem(name_, "Velocity", velocity_.x);
	variables->AddItem(name_, "scale rate", scaleForce_);
	variables->AddItem(name_, "atack rate", atackForce_);
	variables->AddItem(name_, "hitCoolTime", hitCoolTime_);
	variables->AddItem(name_, "correctFrame", correctEndFrame_);
	variables->AddItem(name_, "correctValue", correctOffsetValue_);

	variables->AddItem(name_, "axelTime", dashAxelTime_);
	variables->AddItem(name_, "brakeTime", dashBrakeTime_);
	variables->AddItem(name_, "dashPower", dashPower_);

	variables->AddItem(name_, "MoveAcceleration", moveAcceleration_);
	variables->AddItem(name_, "StanTime", damageStanTime_);
	variables->AddItem(name_, "healpower", healPower_);
	variables->AddItem(name_, "MaxSize", maxSize);
	variables->AddItem(name_, "PushUpHitForce", pushUpHitForce);
	variables->AddItem(name_, "DecelerationRate", decelerationRate);
	variables->AddItem(name_, "subtractionAbsorptionCount_", subtractionAbsorptionCount_);
	variables->AddItem(name_, "AbsorptionAcceleration", absorptionAccelerationForce_);
	SetGlobalVariables();
	isTutrial_ = false;
	isTutrialDash_ = true;
	canDash_ = true;
	// パーティクルエミッタマネージャのインスタンス取得
	pem_ = ParticleEmitterManager::GetInstance();

	// チャージパーティクル生成
	chargeParticleEmitter_ = pem_->CreateEmitter<ChargeParticleEmitter, ChargeParticle>("Charge", 25, 1, { 0.0f, 0.0f, 0.0f }, 10.0f, 0.1f, TextureManager::Load("chargeParticle.png"));
	chargeParticleEmitter_->transform_.SetParent(&transform_);
	chargeParticleEmitter_->SetIsLoop(true);
	chargeParticleEmitter_->SetIsPlay(false);
	chargeParticleEmitter_->SetPlayerWorldTransform(&transform_);

	// チャージ時の円のパーティクル
	chargeCircleEmitter_ = pem_->CreateEmitter<ChargeCircleParticleEmitter, ChargeCircleParticle>("Circle", 30, 1, {}, 10.0f, 0.5f, TextureManager::Load("chargeParticle.png"));
	chargeCircleEmitter_->transform_.SetParent(&transform_);
	chargeCircleEmitter_->SetIsLoop(true);
	chargeCircleEmitter_->SetIsPlay(false);
	chargeCircleEmitter_->SetPlayerWorldTransform(&transform_);
	
	// チャージ終了時パーティクル生成
	chargeFinishParticleEmitter_ = pem_->CreateEmitter<ChargeFinishParticleEmitter, ChargeFinishParticle>("ChargeFinish", 8, 8, { 0.0f, 0.0f, 0.0f }, 10.0f, 0.25f, TextureManager::Load("chargeMaxParticle.png"));
	chargeFinishParticleEmitter_->transform_.SetParent(&transform_);
	chargeFinishParticleEmitter_->SetIsLoop(true);
	chargeFinishParticleEmitter_->SetIsPlay(false);

	// 移動時のパーティクルエフェクト
	moveParticleEmitter_ = pem_->CreateEmitter<MoveParticleEmitter, MoveParticle>("Move", 80, 1, { 0.0f, 0.0f, 0.0f }, 10.0f, 0.04f, TextureManager::Load("moveParticle.png"));
	moveParticleEmitter_->transform_.SetParent(&transform_);
	moveParticleEmitter_->SetIsLoop(true);
	moveParticleEmitter_->SetIsPlay(true);
	moveParticleEmitter_->SetPlayerWorldTransform(&transform_);
}

void Player::Update()
{
	// 現在のスケールが最大スケールを超えている場合は
	if (transform_.scale_.x >= maxSize) {
		// スケールを最大スケールで固定する
		transform_.scale_ = { maxSize,maxSize,maxSize };
	}

	// 行動状態クラスがあれば
	if (state_.get()) {
		// ダメージを喰らっている状態でなければ かつ うりぼが死んでいない時
		if (!isDamaged_ && uribo_->GetHP() > 0) {
			// 行動状態の更新を行う
			state_->Update();
		}
		else {
			// チャージパーティクル停止
			chargeParticleEmitter_->SetIsPlay(false);
			// チャージ終了時のパーティクル停止
			chargeFinishParticleEmitter_->SetIsPlay(false);

			chargeCircleEmitter_->SetIsPlay(false);
		}
	}



	// 攻撃命中クールタイマーの更新処理
	hitCollTimer_.Update();

	//回復用タイマーの更新
	healTimer.Update();

	// 前フレーム座標
	prevPos_ = worldPos_;
	// ワールド座標の更新
	worldPos_ = transform_.translate_;

	//BlowAwayStateじゃない場合
	if (!(state_->name_ == "BlowAway")) {
		// 入力ベクトルが0以外の場合
		if ((velocity_.x != 0.0f || velocity_.z != 0.0f) && !isDamaged_) {
			// 向いている方向に角度を合わせる
			transform_.rotate_.y = -std::atan2(velocity_.x, -velocity_.z);
			// 方向ベクトル保存
			moveVector_ = velocity_;
		}
	}

	// ダメージを喰らっていない状態の場合
	if (!isDamaged_ && uribo_->GetHP() > 0) {
		// 座標に速度ベクトルを加算する
		transform_.translate_ = transform_.translate_ + velocity_;

		// 回転速度を速度ベクトルの長さによって変化させる
		float rotateSpeed = KLib::Lerp<float>(0.0f, 0.25f, Math::Length(velocity_));
		// 回転させる
		transform_.rotate_.x -= rotateSpeed;
	}
	else {
		// 速度ベクトルを0に
		velocity_ = { 0.0f, 0.0f, 0.0f };
		// 回転角のリセット
		transform_.rotate_.x = 0.0f;
	}

	//地面より外に出たら中に戻して状態をRootに変更
	// 右壁
	if (transform_.translate_.x + transform_.scale_.x >= ground_->transform_.scale_.x) {
		// 補間座標
		correctionPos_ = {
			ground_->transform_.scale_.x - transform_.scale_.x - correctOffsetValue_,
			transform_.translate_.y,
			transform_.translate_.z
		};

		// 強制的に待機状態ステートに変更
		ChangeState(std::make_unique<WallCollisionState>());
	}
	// 左壁
	else if (transform_.translate_.x - transform_.scale_.x <= -ground_->transform_.scale_.x) {
		// 補間座標
		correctionPos_ = {
			-ground_->transform_.scale_.x + transform_.scale_.x + correctOffsetValue_,
			transform_.translate_.y,
			transform_.translate_.z
		};
		// 強制的に待機状態ステートに変更
		ChangeState(std::make_unique<WallCollisionState>());
	}

	// 上壁
	if (transform_.translate_.z + transform_.scale_.z >= ground_->transform_.scale_.z) {
		// 補間座標
		correctionPos_ = {
			transform_.translate_.x,
			transform_.translate_.y,
			ground_->transform_.scale_.z - transform_.scale_.z - correctOffsetValue_
		};
		// 強制的に待機状態ステートに変更
		ChangeState(std::make_unique<WallCollisionState>());
	}
	// 下壁
	else if (transform_.translate_.z - transform_.scale_.z <= -ground_->transform_.scale_.z) {
		// 補間座標
		correctionPos_ = {
			transform_.translate_.x,
			transform_.translate_.y,
			-ground_->transform_.scale_.z + transform_.scale_.z + correctOffsetValue_
		};
		// 強制的に待機状態ステートに変更
		ChangeState(std::make_unique<WallCollisionState>());
	}

	// 現在HPの取得
	if (uribo_->GetHP() <= 0 && !isTutrial_) {
		if (uribo_->GetIsDead()) {
			// ゲームオーバーフラグtrue
			isGameOver_ = true;
		}
	}
}

void Player::DisplayImGui()
{
	// トランスフォームの情報表示
	transform_.DisplayImGui();
	//移動加速度
	ImGui::DragFloat("MoveAcceleration", &moveAcceleration_, 0.01f);
	// 最大移動加速度
	ImGui::DragFloat("MaxMoveAcceleration", &kMaxMoveAcceleration_, 0.01f);
	// 減衰速度
	ImGui::DragFloat("DecayAcceleration", &decayAcceleration_, 0.01f);

	ImGui::DragInt("AbsorptionCount", &absorptionCount_, 1.0f);

	// 速度ベクトル
	ImGui::DragFloat3("Velocity", &velocity_.x, 0.01f);
	// 拡大率
	ImGui::DragFloat("scale rate", &scaleForce_, 0.01f);
	// 攻撃倍率
	ImGui::DragFloat("atack rate", &atackForce_, 0.01f);
	// 無敵時間
	ImGui::DragFloat("hitCoolTime", &hitCoolTime_, 0.01f);
	// 修正フレーム
	ImGui::DragFloat("correctFrame", &correctEndFrame_, 0.01f);
	// 修正量
	ImGui::DragFloat("correctValue", &correctOffsetValue_, 0.01f);
	// 無敵時間
	ImGui::DragFloat("dashAxelTime", &dashAxelTime_, 0.01f);
	// 無敵時間
	ImGui::DragFloat("dashBrakeTime", &dashBrakeTime_, 0.01f);
	// 無敵時間
	ImGui::DragFloat("dashPower", &dashPower_, 0.01f);
	// 吸収数
	ImGui::DragInt("Absorption Count", &absorptionCount_);
    // 回復力
	ImGui::DragInt("heal Power", &healPower_);
	// 最大値
	ImGui::DragFloat("Max Size", &maxSize);
	// 突き上げの力（？）
	ImGui::DragFloat("PushUpHitForce", &pushUpHitForce);
	// 減速率
	ImGui::DragFloat("DecelerationRate", &decelerationRate);
	ImGui::DragInt("maxAtackCount", &kMaxAtackPushCount_);

	ImGui::DragInt( "subtractionAbsorptionCount_", &subtractionAbsorptionCount_);
	ImGui::DragInt("AtackCount", &atackPushCount_);

	ImGui::DragFloat3("MoveVect", &moveVector_.x);
	ImGui::DragFloat("absorptionAccelerationForce", &absorptionAccelerationForce_, 0.001f);
	// 改行する
	ImGui::NewLine();

	// ダメージのスタン秒数
	ImGui::DragFloat("StanTime", &damageStanTime_, 0.1f, 0.01f, 5.0f);
	if(ImGui::Button("Save")) {
		GlobalVariables* variables = GlobalVariables::GetInstance();

		variables->SetValue(name_, "MoveAcceleration", moveAcceleration_);
		variables->SetValue(name_, "MaxMoveAcceleration", kMaxMoveAcceleration_);
		variables->SetValue(name_, "DecayAcceleration", decayAcceleration_);
		variables->SetValue(name_, "Velocity", velocity_.x);
		variables->SetValue(name_, "scale rate", scaleForce_);
		variables->SetValue(name_, "atack rate", atackForce_);
		variables->SetValue(name_, "hitCoolTime", hitCoolTime_);
		variables->SetValue(name_, "correctFrame", correctEndFrame_);
		variables->SetValue(name_, "correctValue", correctOffsetValue_);

		variables->SetValue(name_, "axelTime", dashAxelTime_);
		variables->SetValue(name_, "brakeTime", dashBrakeTime_);
		variables->SetValue(name_, "dashPower", dashPower_);

		variables->SetValue(name_, "MoveAcceleration", moveAcceleration_);
		variables->SetValue(name_, "StanTime", damageStanTime_);
		variables->SetValue(name_, "healpower", healPower_);
		variables->SetValue(name_, "MaxSize", maxSize);
		variables->SetValue(name_, "PushUpHitForce", pushUpHitForce);
		variables->SetValue(name_, "DecelerationRate", decelerationRate);
		variables->SetValue(name_, "maxAtackCount", kMaxAtackPushCount_);
		variables->SetValue(name_, "subtractionAbsorptionCount_", subtractionAbsorptionCount_);
		variables->SetValue(name_, "AbsorptionAcceleration", absorptionAccelerationForce_);
		 variables->SaveFile(name_);
		/*kMaxMoveAcceleration_ = variables->GetFloatValue(name_, "MaxMoveAcceleration");
		decayAcceleration_ = variables->GetFloatValue(name_, "DecayAcceleration");
		scaleForce_ = variables->GetFloatValue(name_, "scale rate");
		atackForce_ = variables->GetFloatValue(name_, "atack rate");
		moveAcceleration_ = variables->GetFloatValue(name_, "MoveAcceleration");
		damageStanTime_ = variables->GetFloatValue(name_, "StanTime");*/
	}
}

void Player::OnCollisionEnter(Collider* collider) {
	if (state_->name_ == "Root") {
		
		// 降ってくる野菜に衝突した場合
		if (collider->GetGameObject()->GetObjectTag() == BaseObject::TagMeteor &&
			state_->name_ != "BlowAway" && !isAtack_) {
			// ダメージ処理を行う
			Damage();
			state_->Update();
		}
	}
	if (state_->name_ == "Root"||state_->name_=="Dash") {
		//	// 破片に衝突した場合
		if (collider->GetGameObject()->GetObjectTag() == BaseObject::TagRubble) {
			//がれきにぶつかったらしてサイズを大きくする
			absorptionCount_++;
			if (transform_.scale_.x < maxSize) {
				// 加算するサイズを計算
				float addSize = absorptionCount_ * scaleForce_;
				// 加算サイズ分y座標を加算
				transform_.translate_.y += addSize;
				// 大きさにサイズを加算
				transform_.scale_ = {
					transform_.scale_.x + addSize,
					transform_.scale_.y + addSize,
					transform_.scale_.z + addSize };

			}

			// 食べたSEを再生する
			audio_->PlayWave(eatSE_);
		}
		if (collider->GetGameObject()->GetObjectTag() == BaseObject::TagPushUp) {
			state_->Update();
			transform_.translate_ = prevPos_;
			pushUpPos_ = collider->GetGameObject()->transform_.translate_;
			
			ChangeState(std::make_unique<PushUpHitState>());
			
		}
	}
}

void Player::OnCollision(Collider* collider)
{
	if (collider->GetGameObject()->GetObjectTag() == BaseObject::TagUribo) {
		Heal();
	}
	if (collider->GetGameObject()->GetObjectTag() == BaseObject::TagEnemy &&state_->name_=="Atack") {
		// 前フレーム座標に固定する
		transform_.translate_ = prevPos_;
		// y座標を補正
		transform_.translate_.y = 3.0f;
		// 大きさリセット
		transform_.scale_ = { 2.0f , 2.0f , 2.0f };
		// 攻撃状態でない
		//isAtack_ = false;
		
		
		// 与えたSEを再生する
		audio_->PlayWave(attackSE_);
		ChangeState(std::make_unique<BlowAwayState>());
		// 速度をリセット
		velocity_ = { 0.0f,0.0f,0.0f };
		return;
	}
}

Vector3 Player::GetBossDirection()
{
	// ボスへの方向ベクトルを返す
	return  transform_.translate_ - boss_->transform_.translate_;
}

void Player::Damage()
{
	//クールタイムを過ぎていたら攻撃をくらう
	if (hitCollTimer_.GetIsFinish()) {

		// 速度を0に
		velocity_ = { 0.0f, 0.0f, 0.0f };
		// 回転角をリセット
		transform_.rotate_.x = 0.0f;

		// ダメージアニメーション再生
		pam_->Damage(damageStanTime_);
		audio_->PlayWave(callapseSE);
		// HPを減らす
		//hitPoint_--;
		//食べた野菜の数を減らす
		absorptionCount_ -= absorptionCount_/ subtractionAbsorptionCount_;
		if (absorptionCount_ < 0) {
			absorptionCount_ = 0;
		}
		// 大きさリセット
		transform_.scale_ = { 2.0f , 2.0f , 2.0f };
		transform_.translate_.y = 3.0f;
		for (int i = 0; i < absorptionCount_; i++) {
			if (transform_.scale_.x > maxSize) {
				break;
			}
			// 加算するサイズを計算
			float addSize = i * scaleForce_;
			// 加算サイズ分y座標を加算
			transform_.translate_.y += addSize;
			// 大きさにサイズを加算
			transform_.scale_ = {
				transform_.scale_.x + addSize,
				transform_.scale_.y + addSize,
				transform_.scale_.z + addSize };
		}

		moveParticleEmitter_->SetIsPlay(false);
		// ヒットクールタイマーリセット
		hitCollTimer_.Start(hitCoolTime_);
		ChangeState(std::make_unique<RootState>());
	}

}
void Player::SubtractVelocity()
{
	velocity_ = velocity_ * decelerationRate;
	if (std::abs(velocity_.x) <= 0.01f) {
		velocity_.x = 0.0f;
	}
	if (std::abs(velocity_.z) <= 0.01f) {
		velocity_.z = 0.0f;
	}
}

void Player::Heal()
{
	//カウントが０じゃなくかつ、HPが最大値以下の時、ヒールボタンを押したとき
	if (InputManager::Heal() && absorptionCount_ > 0 && uribo_->GetHP() < uribo_->GetDefaultHP()&&hitCollTimer_.GetIsFinish()&&!isCharge_) {
		if (healTimer.GetIsFinish()) {
			//うりぼーの回復
			uribo_->Heal(healPower_);
			//食べたカウントを減らす
			if (uribo_->GetIsCanHeal()) {
				absorptionCount_--;

				// 減算するサイズを計算
				float decSize = absorptionCount_ * scaleForce_;
				if (transform_.scale_.y - decSize > 2.0f) {

					// 減算サイズ分y座標を減算
					transform_.translate_.y -= decSize;
					// 大きさにサイズを減算
					transform_.scale_ = {
						transform_.scale_.x - decSize,
						transform_.scale_.y - decSize,
						transform_.scale_.z - decSize };


				}
				else {
					transform_.translate_.y = 3.0f;
					transform_.scale_ = { 2.0f,2.0f,2.0f };
				}

				healTimer.Start(healCoolTime_);

				// 野菜を投げる演出用
				FeedVegetable* fvg = GameObjectManager::GetInstance()->CreateInstance<FeedVegetable>("FeedVege", BaseObject::TagNone);
				// 初期化後処理を呼び出す
				fvg->PostInit(pam_->bodyTransform_.GetWorldPos(), uribo_->transform_.GetWorldPos());

				// 与えたSEを再生する
				audio_->PlayWave(feedSE_);
			}
		}
	}
}

void Player::SetGlobalVariables()
{
	GlobalVariables* variables = GlobalVariables::GetInstance();
	variables->CreateGroup(name_);

	// 移動時の加速度
	moveAcceleration_= variables->GetFloatValue(name_, "MoveAcceleration");
	// 
	kMaxMoveAcceleration_ = variables->GetFloatValue(name_, "MaxMoveAcceleration" );
	// 
	decayAcceleration_ = variables->GetFloatValue(name_, "DecayAcceleration" );
	// スケールの値
	scaleForce_ = variables->GetFloatValue(name_, "scale rate");
	// 攻撃の力
	atackForce_ = variables->GetFloatValue(name_, "atack rate");
	// 無敵時間
	hitCoolTime_ = variables->GetFloatValue(name_, "hitCoolTime");
	// 修正補間フレーム数
	correctEndFrame_ = variables->GetFloatValue(name_, "correctFrame");
	// 修正量
	correctOffsetValue_ = variables->GetFloatValue(name_, "correctValue");

	// 加速の時間
	dashAxelTime_ = variables->GetFloatValue(name_, "axelTime");
	// 減速の時間
	dashBrakeTime_ = variables->GetFloatValue(name_, "brakeTime");
	// ダッシュの量
	dashPower_ = variables->GetFloatValue(name_, "dashPower");

	// ダメージ受けた際の硬直時間
	damageStanTime_ = variables->GetFloatValue(name_, "StanTime");
	// 回復力
	healPower_ = variables->GetIntValue(name_, "healpower");
	// 最大サイズ
    maxSize = variables->GetFloatValue(name_, "MaxSize");
	// 突き上げ衝突時のパワー
    pushUpHitForce = variables->GetFloatValue(name_, "PushUpHitForce" );
	// 減速率
	decelerationRate = variables->GetFloatValue(name_, "DecelerationRate");
	//アタックカウントの最大値
	kMaxAtackPushCount_= variables->GetIntValue(name_, "maxAtackCount");
	//減らす野菜の数
	subtractionAbsorptionCount_= variables->GetIntValue(name_, "subtractionAbsorptionCount_" );

	absorptionAccelerationForce_ = variables->GetFloatValue(name_, "AbsorptionAcceleration");
}

void Player::Atacking()
{
	if (boss_) {
		//攻撃力を吸収した数*攻撃倍率に
		atackPower_ = static_cast<float>(absorptionCount_) * atackForce_;
		velocity_ = Math::Normalize(Vector3(boss_->transform_.translate_.x, transform_.translate_.y, boss_->transform_.translate_.z) - transform_.translate_) * 2.0f;

		isAtack_ = true;
	}
}

void Player::AddAtackCount()
{
	atackPushCount_++;
}

void Player::ResetAtackCount()
{
	atackPushCount_ = 0;
}

void Player::ChangeState(std::unique_ptr<IPlayerState> newState)
{
	// 共通初期化関数を実行
	newState->PreInit(this);
	// 初期化関数を実行
	newState->Init();

	// 新しい行動状態を受け渡す
	state_ = std::move(newState);
}
