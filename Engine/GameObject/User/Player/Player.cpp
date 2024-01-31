#include "Player.h"
#include "PlayerAnimManager.h"
#include"../../../GlobalVariables/GlobalVariables.h"
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
	variables->AddItem(name_, "MoveAcceleration", moveAcceleration_);
	variables->AddItem(name_, "StanTime", damageStanTime_);
	variables->AddItem(name_, "healpower", healPower_);
	variables->AddItem(name_, "MaxSize", maxSize);
	variables->AddItem(name_, "PushUpHitForce", pushUpHitForce);
	variables->AddItem(name_, "DecelerationRate", decelerationRate);
	variables->AddItem(name_, "subtractionAbsorptionCount_", subtractionAbsorptionCount_);
	SetGlobalVariables();
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
		// ダメージを喰らっている状態でなければ
		if (!isDamaged_) {
			// 行動状態の更新を行う
			state_->Update();
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
		}
	}

	// ダメージを喰らっていない状態の場合
	if (!isDamaged_) {
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
	if (transform_.translate_.x + transform_.scale_.x >= ground_->transform_.scale_.x) {
		// x軸方向の速度ベクトルを0に
		velocity_.x = 0.0f;
		// 前フレーム座標に固定する
		transform_.translate_.x = ground_->transform_.scale_.x - transform_.scale_.x;
		// 強制的に待機状態ステートに変更
		ChangeState(std::make_unique<RootState>());
	}
	else if (transform_.translate_.x - transform_.scale_.x <= -ground_->transform_.scale_.x) {
		// x軸方向の速度ベクトルを0に
		velocity_.x = 0.0f;
		// 前フレーム座標に固定する
		transform_.translate_.x = -ground_->transform_.scale_.x + transform_.scale_.x;
		// 強制的に待機状態ステートに変更
		ChangeState(std::make_unique<RootState>());
	}

	if (transform_.translate_.z + transform_.scale_.z >= ground_->transform_.scale_.z) {
		// z軸方向の速度ベクトルを0に
		velocity_.z = 0.0f;
		// 前フレーム座標に固定する
		transform_.translate_.z = ground_->transform_.scale_.z - transform_.scale_.z;
		// 強制的に待機状態ステートに変更
		ChangeState(std::make_unique<RootState>());
	}
	else if (transform_.translate_.z - transform_.scale_.z <= -ground_->transform_.scale_.z) {
		// x軸方向の速度ベクトルを0に
		velocity_.z = 0.0f;
		// 前フレーム座標に固定する
		transform_.translate_.z = -ground_->transform_.scale_.z + transform_.scale_.z;
		// 強制的に待機状態ステートに変更
		ChangeState(std::make_unique<RootState>());
	}
	//SubtractVelocity();

	// 現在HPの取得
	if (uribo_->GetHP() <= 0) {
		// ゲームオーバーフラグtrue
		isGameOver_ = true;
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
	// 速度ベクトル
	ImGui::DragFloat3("Velocity", &velocity_.x, 0.01f);
	// 拡大率
	ImGui::DragFloat("scale rate", &scaleForce_, 0.01f);
	// 攻撃倍率
	ImGui::DragFloat("atack rate", &atackForce_, 0.01f);
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
		variables->SetValue(name_, "MoveAcceleration", moveAcceleration_);
		variables->SetValue(name_, "StanTime", damageStanTime_);
		variables->SetValue(name_, "healpower", healPower_);
		variables->SetValue(name_, "MaxSize", maxSize);
		variables->SetValue(name_, "PushUpHitForce", pushUpHitForce);
		variables->SetValue(name_, "DecelerationRate", decelerationRate);
		variables->SetValue(name_, "maxAtackCount", kMaxAtackPushCount_);
		variables->SetValue(name_, "subtractionAbsorptionCount_", subtractionAbsorptionCount_);
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
	//if (state_->name_ == "Root") {
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
		// 降ってくる野菜に衝突した場合
		if (collider->GetGameObject()->GetObjectTag() == BaseObject::TagMeteor && 
			state_->name_ != "BlowAway" && !isAtack_) {
			//食べた野菜の数を減らす
			absorptionCount_ -= subtractionAbsorptionCount_;
			if (absorptionCount_ < 0) {
				absorptionCount_ = 0;
			}
			// ダメージ処理を行う
			Damage();
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
		}
	

	if (collider->GetGameObject()->GetObjectTag() == BaseObject::TagPushUp) {
		transform_.translate_ = prevPos_;
		pushUpPos_ = collider->GetGameObject()->transform_.translate_;
		//食べた野菜の数を減らす
		absorptionCount_ -= subtractionAbsorptionCount_;
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
		ChangeState(std::make_unique<PushUpHitState>());

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
		// 速度をリセット
		velocity_ = { 0.0f,0.0f,0.0f };
		
		// 与えたSEを再生する
		audio_->PlayWave(attackSE_);
		ChangeState(std::make_unique<BlowAwayState>());
		return;
	}
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

		// HPを減らす
		hitPoint_--;

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
	//カウントが０じゃなくヒールボタンを押したとき
	if (InputManager::Heal() && absorptionCount_ > 0) {
		if (healTimer.GetIsFinish()) {
			//うりぼーの回復
			uribo_->Heal(healPower_);
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
			//食べたカウントを減らす
			absorptionCount_--;
			healTimer.Start(healCoolTime_);

			// 与えたSEを再生する
			audio_->PlayWave(feedSE_);
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
}

void Player::Atacking()
{

	//攻撃力を吸収した数*攻撃倍率に
	atackPower_ = static_cast<float>(absorptionCount_) * atackForce_;
	velocity_ = Math::Normalize(Vector3(0.0f, transform_.translate_.y, 0.0f) - transform_.translate_) * 2.0f;

	isAtack_ = true;
	
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
