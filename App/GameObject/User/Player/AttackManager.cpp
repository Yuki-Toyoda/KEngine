#include "AttackManager.h"
#include "Player.h"
#include "App/GameObject/User/FollowCamera/FollowCamera.h"
#include "App/GameObject/User/Enemy/Enemy.h"

void AttackManager::Init()
{
	// 入力管理インスタンスの取得
	input_ = Input::GetInstance();

	// コンボマネージャーの初期化
	comboManager_.Init(player_);
	// コンボマネージャーに条件を追加する
	comboManager_.AddCondition("IsHit", &isHit_); // 攻撃の命中フラグ
}

void AttackManager::Update()
{
	// 軌跡更新
	TrailUpdate();

	// これ以降の処理は行動できる状態で無ければ早期リターン
	if (!player_->GetCanAction()) { return; }

	// 回転方向の補正処理
	RotateCorrectUpdate();

	// カウンター攻撃関連処理
	CounterUpdate();

	// 攻撃開始関連処理
	AttackStartUpdate();
}

void AttackManager::PostUpdate()
{
	// 命中フラグのリセット
	isHit_ = false;
	// カウンター可能フラグのリセット
	isCanCounter_ = false;
}

void AttackManager::DisplayImGui()
{
	// コンボマネージャーのImGuiの表示
	comboManager_.DisplayImGui();
}

void AttackManager::TrailUpdate()
{
	// 軌跡a値
	float& trailAlpha = player_->GetSwordLine()->trailMaterial_.color_.w;
	// 攻撃中は軌跡を表示させる
	if (isAttacking_) {
		trailAlpha = KLib::Lerp(trailAlpha, maxTrailAlpha, startAppearTrailCorrectSpeed);
	}
	else {
		// 攻撃中でない場合は軌跡を徐々に消す
		if (trailAlpha <= trailAlphaThresold) {
			trailAlpha = minTrailAlpha;
		}
		else {
			trailAlpha = KLib::Lerp(trailAlpha, minTrailAlpha, endAppearTrailCorrectSpeed);
		}
	}
}

void AttackManager::CounterUpdate()
{
	// カウンター不可能、カウンター攻撃状態の場合早期リターン
	std::string stateName = player_->GetStateName();
	if (!isCanCounter_ || stateName == "Counter") { return; }

	// Damage状態の場合早期リターン
	if (stateName == "Damage") { return; }

	// Bボタンを押したときにカウンター攻撃を発動
	if (input_->InspectButton(XINPUT_GAMEPAD_B, TRIGGER)) {
		// 行動を変更
		player_->ChangeState(std::make_unique<CounterAttack>());
	}
}

void AttackManager::AttackStartUpdate()
{
	// 下記条件の場合早期リターン
	// 1. プレイヤーがダメージを受けている状態
	// 2. 攻撃中状態
	// 3. 攻撃可能状態でない
	// 4. 攻撃中である
	std::string stateName = player_->GetStateName();
	if (stateName == "Damage" ||
		stateName == "Attack" ||
		stateName == "Counter" ||
		!canAttack_ ||
		isAttacking_) {
		return;
	}

	// Aボタンを押すと攻撃する
	if (input_->InspectButton(XINPUT_GAMEPAD_A, TRIGGER)) {
		// 敵の座標を取得
		Vector3 enemyPosition = player_->GetEnemy()->transform_.translate_;
		// プレイヤーの座標取得
		Vector3 playerPosition = player_->transform_.translate_;

		// 敵との距離を計測
		float toEnemyDistance = std::abs(Vector3::Length(enemyPosition - playerPosition));
		// 敵との距離が補正閾値内だった場合
		if (toEnemyDistance <= correctDirectionDistance) {
			// 回転補正を行う
			isCorrectingToEnemy_ = true;
		}

		// プレイヤーの行動を変更
		player_->ChangeState(std::make_unique<Attack>());

		// Z注目有効時
		if (player_->GetFollowCamera()->GetEnableZForcus()) {
			comboManager_.ChangeCombo("LockOn");
		}
		else {
			comboManager_.ChangeCombo("Normal");
		}

		// デバッグ有効時には表示中コンボへ切り替える
		if (comboManager_.GetEnableComboDebug()) {
			comboManager_.ChangeCombo(comboManager_.GetImGuiDisplayName());
		}
	}
}

void AttackManager::RotateCorrectUpdate()
{
	// 補正が有効でない場合早期リターン
	if (!isCorrectingToEnemy_) { return; }

	// 敵の座標を取得
	Vector3 enemyPosition = player_->GetEnemy()->transform_.translate_;
	// プレイヤーの座標取得
	Vector3 playerPosition = player_->transform_.translate_;
	// 敵がいる方向ベクトルを求める
	enemyDirection_ = enemyPosition - playerPosition;
	// 敵がいる方向を求める
	float targetAngleY = std::atan2(enemyDirection_.x, enemyDirection_.z);
	// 閾値を超えるまで補正を続ける
	if (std::abs(player_->transform_.rotate_.y - targetAngleY) > angleCorrectThreshold) {
		// 身体を回転させる
		player_->transform_.rotate_.y = KLib::LerpShortAngle(player_->transform_.rotate_.y, targetAngleY, toEnemyCorrectSpeed);
	}
	else {
		// 補正終了
		isCorrectingToEnemy_ = false;
	}
}
