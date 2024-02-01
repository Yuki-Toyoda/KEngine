//#include "TutrialBoss.h"
//#include "../BossAnimManager.h"
//void TutrialBoss::SuccessorInit()
//{
//	// 大きさ設定
//	transform_.scale_ = { 10.0f, 10.0f, 10.0f };
//
//	// メッシュ追加
//	//AddMesh(&transform_, color_, "./Engine/Resource/Samples/Box", "Box.obj");
//	// 色の設定
//	color_ = { 0.6f,0.6f,0.0f,1.0f };
//
//	// 行動状態変更
//	ChangeState(std::make_unique<TutrialState>());
//
//	// Sphereのコライダーを追加
//	AddColliderSphere("Boss", &transform_.translate_, &transform_.scale_.x);
//
//	// 最大体力設定
//	kMaxHitPoint_ = 25.0f;
//	// 体力を最大体力をもとに設定
//	hitPoint_ = kMaxHitPoint_;
//
//	// ゲームオブジェクトマネージャのインスタンスを取得
//	gameObjectManager_ = GameObjectManager::GetInstance();
//	// 行動状態リストの生成
//	MakeStateList();
//
//	GlobalVariables* variables = GlobalVariables::GetInstance();
//	variables->CreateGroup(name_);
//	variables->AddItem(name_, "HitPoint", hitPoint_);
//	variables->AddItem(name_, "waitSingle", waitForSingle_);
//	variables->AddItem(name_, "waitmulti", waitForMulti_);
//	variables->AddItem(name_, "waitRoller", waitForRoller_);
//	variables->AddItem(name_, "waitPushUp", waitForPushUp_);
//	variables->AddItem(name_, "FallAttackReadyTime", fallAttackReadyTime_);
//	variables->AddItem(name_, "PushUpReadyTime", pushUpReadyTime_);
//	variables->AddItem(name_, "RollerAttackReadyTime", rollerAttackReadyTime_);
//	ApplyGlobalVariables();
//}
//
//void TutrialBoss::SuccessorUpdate()
//{
//	// 行動状態クラスがあれば
//	if (state_.get()) {
//		// 現在ステートを更新
//		state_->Update();
//	}
//}
//
//void TutrialBoss::DisplayImGui()
//{
//}
//
//void TutrialBoss::Reset()
//{
//}
//
//void TutrialBoss::SetParameter(const std::string& levelName, const std::string& enemyName)
//{
//	levelName, enemyName;
//}
//
//void TutrialBoss::ApplyParameter(const std::string& levelName, const std::string& enemyName)
//{
//	levelName, enemyName;
//}
//
//void TutrialBoss::OnCollisionEnter(Collider* collider)
//{
//	//プレイヤーが攻撃していたらダメージをくらう
//	if (collider->GetGameObject()->GetObjectTag() == BaseObject::TagPlayer && player_->GetIsAtack()) {
//		// ボスのHPをプレイヤーの攻撃力に基づいて減少させる
//		hitPoint_ -= player_->GetAtackPower();
//		// プレイヤーを攻撃していない状態に
//		player_->SetIsAtack(false);
//
//		//吸収した数をリセットして座標とスケール調整
//		player_->ResetAbsorptionCount();
//
//
//
//			// アニメーションマネージャーセットされている場合
//			if (bam_ != nullptr) {
//				// ダメージアニメーション再生
//				bam_->PlayDamageAnim();
//			}
//		
//	}
//}
//
//void TutrialBoss::OnCollision(Collider* collider)
//{
//}
//
//void TutrialBoss::MakeStateList()
//{
//}
//
//void TutrialBoss::ApplyGlobalVariables()
//{
//}
