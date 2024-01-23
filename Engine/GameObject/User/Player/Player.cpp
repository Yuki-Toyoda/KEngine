#include "Player.h"
#include "PlayerAnimManager.h"

void Player::Init()
{
	// 移動ベクトル初期化
	velocity_ = { 0.0f,0.0f,0.0f };
	// 球のコライダーを追加
	AddColliderSphere("Enemy", &worldPos_, &transform_.scale_.x);
	ChangeState(std::make_unique<RootState>());
	isGameOver_ = false;
}

void Player::Update()
{
	
	if (transform_.scale_.x >= maxSize) {
		transform_.scale_ = { 5.0f,5.0f,5.0f };
	}
	// 行動状態クラスがあれば
	if (state_.get()) {
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
		velocity_.x =0.0f;
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
		velocity_.z =0.0f;
		// 前フレーム座標に固定する
		transform_.translate_.z = ground_->transform_.scale_.z - transform_.scale_.z;
		// 強制的に待機状態ステートに変更
		ChangeState(std::make_unique<RootState>());
	}else if (transform_.translate_.z - transform_.scale_.z <= -ground_->transform_.scale_.z) {
		// x軸方向の速度ベクトルを0に
		velocity_.z = 0.0f;
		// 前フレーム座標に固定する
		transform_.translate_.z = -ground_->transform_.scale_.z + transform_.scale_.z;
		// 強制的に待機状態ステートに変更
		ChangeState(std::make_unique<RootState>());
	}
	//SubtractVelocity();
	if (uribo_->GetHP() <= 0) {
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

	//拡大率
	ImGui::DragFloat("scale rate", &scaleForce_, 0.01f);
	//攻撃倍率
	ImGui::DragFloat("atack rate", &atackForce_, 0.01f);

	ImGui::DragInt("Absorption Count", &absorptionCount_);
	// 改行する
	ImGui::NewLine();

	// ダメージのスタン秒数
	ImGui::DragFloat("StanTime", &damageStanTime_, 0.1f, 0.01f, 5.0f);
}

void Player::OnCollisionEnter(Collider* collider){
	//if (state_->name_ == "Root") {
	//	// 破片に衝突した場合
		if (collider->GetGameObject()->GetObjectTag() == BaseObject::TagRubble) {
			//がれきにぶつかったらしてサイズを大きくする
			absorptionCount_++;
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
	//	// 降ってくる野菜に衝突した場合
	//	if (collider->GetGameObject()->GetObjectTag() == BaseObject::TagMeteor) {
	//		// ダメージ処理を行う
	//		Damage();
	//	}
	//}
	if (collider->GetGameObject()->GetObjectTag() == BaseObject::TagEnemy && isAtack_) {
		//吸収した数をリセットして座標とスケール調整
		ResetAbsorptionCount();
		// 前フレーム座標に固定する
		transform_.translate_ = prevPos_;
		// y座標を補正
		transform_.translate_.y = 3.0f;
		// 大きさリセット
		transform_.scale_ = { 2.0f , 2.0f , 2.0f };
		// 攻撃状態でない
		isAtack_ = false;
		// 速度をリセット
		velocity_ = { 0.0f,0.0f,0.0f };

		// 吹っ飛び状態に変更
		ChangeState(std::make_unique<BlowAwayState>());
	}
	if (collider->GetGameObject()->GetObjectTag() == BaseObject::TagPushUp) {
		/*transform_.translate_ = prevPos_;
		pushUpPos_ = collider->GetGameObject()->transform_.translate_;

		ChangeState(std::make_unique<PushUpHitState>());*/

	}
	
}

void Player::OnCollision(Collider* collider)
{
	if (collider->GetGameObject()->GetObjectTag() == BaseObject::TagUribo) {
		Heal();

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
	if (InputManager::Heal()&&absorptionCount_>0) {
		if (healTimer.GetIsFinish()) {
			//うりぼーの回復
			uribo_->Heal(healPower_);
			// 減算するサイズを計算
			float decSize = absorptionCount_ * scaleForce_;
			// 減算サイズ分y座標を減算
			transform_.translate_.y -= decSize;
			// 大きさにサイズを減算
			transform_.scale_ = {
				transform_.scale_.x - decSize,
				transform_.scale_.y - decSize,
				transform_.scale_.z - decSize };
			//食べたカウントを減らす
			absorptionCount_--;
			healTimer.Start(healCoolTime_);
				
		}
	}
}

void Player::Atack()
{
	
	//攻撃力を吸収した数*攻撃倍率に
	atackPower_ =static_cast<float>( absorptionCount_) * atackForce_;
	velocity_ = Math::Normalize(  Vector3(0.0f, transform_.translate_.y, 0.0f)- transform_.translate_)*2.0f;


	//吸収した数をリセットして座標とスケール調整
	ResetAbsorptionCount();
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
