#include "MultiAtackState.h"
#include "../IEnemy.h"
#include "../BossAnimManager.h"

void MultiAtackState::Init()
{
	// ステート名設定
	name_ = "MultiAtack";

	// データマネージャーの取得
	GameDataManager* dataManager = GameDataManager::GetInstance();
	// グループ名の取得
	std::string group = GameDataManager::GetInstance()->GetMultiAttack(0);
	// クールタイムの取得
	coolTime_ = dataManager->GetValue<float>({ group,"Parameter" }, "CoolTime");
	// 生成最大数の取得
	kMaxCount_ = dataManager->GetValue<int>({ group,"Parameter" }, "MaxCount");

	// 攻撃間隔タイマーの開始
	atackTimer_.Start(coolTime_);

	// 複数落下攻撃アニメーション開始
	enemy_->GetBossAnimManager()->PlayMultiFallAnim();

}

void MultiAtackState::Update()
{
	// 攻撃間隔タイマー終了時
	if (atackTimer_.GetIsFinish()) {
		//攻撃回数を増加
		atackCount_++;
		//タイマーをスタート
		atackTimer_.Start(coolTime_);
		//隕石を生成して
		Meteor* meteor = gameObjectmanager_->CreateInstance<Meteor>("meteor", BaseObject::TagMeteor);
		//隕石の座標
		meteor->transform_.translate_ = enemy_->GetPlayer()->transform_.translate_;
		GameDataManager* dataManager = GameDataManager::GetInstance();
		std::string group = dataManager->GetMultiAttack(0);
		std::string distance = dataManager->GetSingleAttack(0);
		meteor->transform_.translate_.y = dataManager->GetValue<float>({ distance,"Parameter" }, "Distance");
		meteor->transform_.scale_ = dataManager->GetValue<Vector3>({ group,"Parameter" }, "Scale");
		meteor->SetgameManager(enemy_->gameManager_);

		// 落下させる
		meteor->SetIsFall(true);
		// 攻撃アニメーション再生
		enemy_->GetBossAnimManager()->SetPlayMultiFallAttackAnim(true);
	}

	// 攻撃回数が最大回数を超過している場合
	if (atackCount_ >= kMaxCount_) {

		// 複数落下攻撃アニメーションの終了を伝える
		enemy_->GetBossAnimManager()->SetIsMultiFallEnd(true);
		// 攻撃アニメーション再生
		enemy_->GetBossAnimManager()->SetPlayMultiFallAttackAnim(false);

		// stateを変更
		enemy_->SetWaitTime(enemy_->GetWaitMulti());
		enemy_->ChangeState(std::make_unique<WaitTimeState>());

		// その後の処理を行わない
		return;
	}

	// ダメージアニメーション再生時は強制終了
	if (enemy_->GetBossAnimManager()->GetAnimation()->GetReadingParameterName() == "Boss_Damage") {

		// stateを変更
		enemy_->SetWaitTime(enemy_->GetWaitMulti());
		enemy_->ChangeState(std::make_unique<WaitTimeState>());

		// その後の処理を行わない
		return;
	}

	// タイマーの更新
	atackTimer_.Update();
}

void MultiAtackState::DisplayImGui()
{
}

