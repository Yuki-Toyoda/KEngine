#include "EnemyMove.h"
#include "../Enemy.h"
#include "../../Player/Player.h"
#include "../../../../Utility/Random/Random.h"

void EnemyMove::Init()
{
	// 行動名称の設定
	stateName_ = "Move";

	// 移動元座標を取得
	prevPosition_ = enemy_->transform_.translate_;

	// 差分計算用
	Vector3 sub{};

	if (!enemy_->isFieldOut_) {
		// プレイヤーから敵へのベクトルを求める
		sub = Vector3::Normalize(enemy_->transform_.translate_ - enemy_->player_->transform_.translate_);

		// y軸方向のベクトルは0にする
		//sub.y = 0.0f;

		// 移動座標を求める
		movePosition_ = sub * KLib::RandomF(12.5f, 15.5f, 2);
	}
	else {
		// プレイヤーから敵へのベクトルを求める
		sub = Vector3::Normalize(enemy_->transform_.translate_ - Vector3(0.0f, 0.0f, 0.0f));

		// y軸方向のベクトルは0にする
		//sub.y = 0.0f;

		// 移動座標を求める
		movePosition_ = sub * KLib::RandomF(15.0f, 25.0f, 2);

		// フィールドアウトしていない状態に
		enemy_->isFieldOut_ = false;
	}

	// 移動座標の高さを統一
	movePosition_.y = KLib::RandomF(3.0f, 4.0f, 2);

	// タイマー起動
	moveTimer_.Start(2.5f);
}

void EnemyMove::Update()
{
	// 移動タイマーが終了していなければ
	if (!moveTimer_.GetIsFinish()) {
		// 移動を行う
		enemy_->transform_.translate_ = KLib::Lerp<Vector3>(prevPosition_, movePosition_, KLib::EaseInOutQuad(moveTimer_.GetProgress()));

		// 移動中にフィールド外にいった場合
		if (enemy_->isFieldOut_) {
			// 初期化
			Init();
		}
	}
	else { // 終了時
		// 待機状態へ
		enemy_->ChangeState(std::make_unique<EnemyRoot>());
		// それ以降の処理を無視
		return;
	}

	// タイマー更新
	moveTimer_.Update();
}

void EnemyMove::DisplayImGui()
{

}