#include "CounterAttack.h"
#include "App/GameObject/User/Player/Player.h"
#include "App/GameObject/User/Enemy/Enemy.h"

void CounterAttack::Init()
{
	// 行動名設定
	stateName_ = "Counter";

	// プレイヤーの攻撃開始アニメーション再生
	player_->skiningModels_["Player"]->animationManager_.PlayAnimation("CounterStart");

	// 敵の取得
	enemy_ = player_->GetEnemy();
	// 開始座標はプレイヤー座標
	startPos_ = player_->transform_.translate_;
	// 補間座標は敵の少し上
	targetPos_ = enemy_->GetBodyTransform()->GetWorldPos();
	targetPos_.y += 1.25f;

	// ロックオン対象の座標
	Vector3 targetPos = enemy_->transform_.translate_;
	// 追従対象からロックオン対象への差分ベクトル
	Vector3 sub = targetPos - player_->transform_.translate_;
	// 方向ベクトルを元にプレイヤーがいる角度を求める
	player_->transform_.rotate_.y = std::atan2(sub.x, sub.z);

	// 角度補正を無効に
	player_->SetIsCorrectDirection(false);

	// 攻撃対象のロックオンを不可能な状態に
	player_->GetEnemy()->SetCanLockOn(false);

	// プレイヤーが向いてる方向のベクトルを取得
	Matrix4x4 rotateMat = Matrix4x4::MakeRotateY(player_->transform_.rotate_.y);
	endPos_ =  enemy_->transform_.translate_ + (Vector3(0.0f, 0.0f, 3.0f) * rotateMat);
	endPos_.y = 0.0f;

	// 補間点を求める
	startControlPos_ = startPos_ - (targetPos_ - startPos_);
	endControlPos_ = endPos_ + (endPos_ - targetPos_);

	// 演出用タイマー開始
	stagingTimer_.Start(moveTime);
}

void CounterAttack::Update()
{
	
	// 進捗によって管理
	switch (progress_)
	{
	case CounterAttack::MOVE:
		// 移動更新
		MoveUpdate();
		break;
	case CounterAttack::ATTACK:
		// 攻撃更新
		AttackUpdate();
		break;
	case CounterAttack::END:
		// 終了更新
		EndUpdate();
		break;
	}
}

void CounterAttack::DisplayImGui()
{

}

void CounterAttack::MoveUpdate()
{
	// プレイヤー座標を移動させる
	player_->transform_.translate_ = CatmullRomSpline(startControlPos_, startPos_, targetPos_, endPos_, stagingTimer_.GetProgress());

	// 演出用タイマー終了時
	if (stagingTimer_.GetIsFinish()) {
		// プレイヤーの攻撃アニメーション再生
		player_->skiningModels_["Player"]->animationManager_.PlayAnimation("CounterAttack", 0.1f);

		// 次の段階へ
		progress_++;
	}
	// 演出用タイマー更新
	stagingTimer_.Update();
}

void CounterAttack::AttackUpdate()
{
	if (player_->skiningModels_["Player"]->animationManager_.GetPlayingAnimationProgress() >= 0.25f) {
		// 攻撃中状態に
		player_->GetAttackManager()->SetIsAttacking(true);
	}

	// カウンター攻撃アニメーションが終了している場合
	if (player_->skiningModels_["Player"]->animationManager_.GetPlayingAnimationProgress() >= 0.8f) {
		// 演出用タイマー更新
		stagingTimer_.Start(endTime);
		// 攻撃していない状態に
		player_->GetAttackManager()->SetIsAttacking(false);
		// 次の段階へ
		progress_++;
	}
}

void CounterAttack::EndUpdate()
{
	// プレイヤー座標を移動させる
	player_->transform_.translate_ = CatmullRomSpline(startPos_, targetPos_, endPos_, endControlPos_, stagingTimer_.GetProgress());

	// 演出用タイマー終了時
	if (stagingTimer_.GetIsFinish()) {
		// 角度補正を有効に
		player_->SetIsCorrectDirection(true);

		// 攻撃対象のロックオンを可能な状態に
		player_->GetEnemy()->SetCanLockOn(true);

		// 待機状態へ移行
		player_->ChangeState(std::make_unique<Root>());
	}
	// 演出用タイマー更新
	stagingTimer_.Update();
}

Vector3 CounterAttack::CatmullRomSpline(const Vector3& p0, const Vector3& p1, const Vector3& p2, const Vector3& p3, const float t)
{
	float t2 = t * t;
	float t3 = t2 * t;

	return	p1 * (2.0f * t3 - 3.0f * t2 + 1.0f) +
			p2 * (-2.0f * t3 + 3.0f * t2) +
			(p2 - p0) * (t3 - 2.0f * t2 + t) * 0.5f +
			(p3 - p1) * (t3 - t2) * 0.5f;
}
