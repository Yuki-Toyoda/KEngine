#include "Boss.h"
#include "../../../GlobalVariables/GlobalVariables.h"
#include "BossAnimManager.h"
#include "../../../Scene/FadeManager.h"
#include "../../../Particle/ParticleEmitterManager.h"
#include "../../../Particle/ParticleList.h"
#include "../camera/InGameCamera.h"

void Boss::SuccessorInit()
{
	// パーティクルエミッタマネージャのインスタンス取得
	pem_ = ParticleEmitterManager::GetInstance();

	// 大きさ設定
	transform_.scale_ = {10.0f, 10.0f, 10.0f };

	// メッシュ追加
	//AddMesh(&transform_, color_, "./Engine/Resource/Samples/Box", "Box.obj");
	// 色の設定
	color_ = { 0.6f,0.6f,0.0f,1.0f };

	// 行動状態変更
	ChangeState(std::make_unique<WaitTimeState>());

	// Sphereのコライダーを追加
	AddColliderSphere("Boss", &transform_.translate_, &transform_.scale_.x);
	
	// 最大体力設定
	kMaxHitPoint_ = 25.0f;
	// 体力を最大体力をもとに設定
	hitPoint_ = kMaxHitPoint_;

	// ゲームオブジェクトマネージャのインスタンスを取得
	gameObjectManager_ = GameObjectManager::GetInstance();
	// 行動状態リストの生成
	MakeStateList();
	tutrialAtackEnd_ = false;
	GlobalVariables* variables = GlobalVariables::GetInstance();
	variables->CreateGroup(name_);
	variables->AddItem(name_, "HitPoint", hitPoint_);
	variables->AddItem(name_,"waitSingle", waitForSingle_);
	variables->AddItem(name_,"waitmulti", waitForMulti_);
	variables->AddItem(name_,"waitRoller", waitForRoller_);
	variables->AddItem(name_,"waitPushUp", waitForPushUp_);
	variables->AddItem(name_, "FallAttackReadyTime", fallAttackReadyTime_);
	variables->AddItem(name_, "PushUpReadyTime", pushUpReadyTime_);
	variables->AddItem(name_, "RollerAttackReadyTime", rollerAttackReadyTime_);
	variables->AddItem(name_, "ShakeTime", shakeTime_);
	variables->AddItem(name_, "ShakeStrength", shakeStrength_);
	ApplyGlobalVariables();
	isTutrial_ = false;
}

void Boss::SuccessorUpdate()
{
	if (isTutrial_) {
		// 行動状態クラスがあれば
		if (state_.get()) {
			// 現在ステートを更新
			state_->Update();
		}
		// HPが0以下でない場合
		if (hitPoint_ < 0.0f) {
			GlobalVariables* variables = GlobalVariables::GetInstance();
			hitPoint_ = variables->GetFloatValue(name_, "HitPoint");
		}
	}
	else {
		// HPが0以下でない場合
		if (hitPoint_ > 0.0f) {
			// 行動状態クラスがあれば
			if (state_.get()) {
				// 現在ステートを更新
				state_->Update();
			}
		}
		else {
			if (bam_->GetAnimation()->GetReadingParameterName() == "Boss_Dead" && bam_->GetAnimation()->isEnd_) {
				// フェード演出を一度も行っていない場合
				if (!isFade_) {
					// フェードアウト
					FadeManager::GetInstance()->ChangeParameter("WhiteOut", true);
					FadeManager::GetInstance()->Play();
					// フェード演出を行ったらトリガー
					isFade_ = true;
				}

				// フェード演出中でなく、かつフェード演出トリガーを行っていた場合
				if (!FadeManager::GetInstance()->GetIsStaging() && isFade_) {
					// シーン遷移トリガーtrue
					isSceneChange_ = true;
				}
			}
		}
	}
}

void Boss::DisplayImGui()
{
	float time = GetBossAnimManager()->GetAnimation()->GetAnimationProgress();
	// 基底クラスのImGuiを表示
	IEnemy::DisplayImGui();
	// 各種パラメータのImGuiを表示
	ImGui::DragFloat("HitPoint", &hitPoint_); // HP
	ImGui::InputInt("PatternNumber", &patternNumber_); // パターン番号
	
	// 攻撃後待機時間の設定
	ImGui::DragFloat("waitSingle", &waitForSingle_,0.1f);
	ImGui::DragFloat("waitmulti", &waitForMulti_, 0.1f); 
	ImGui::DragFloat("waitRoller", &waitForRoller_, 0.1f);
	ImGui::DragFloat("waitPushUp", &waitForPushUp_, 0.1f);
	ImGui::DragFloat("Time", &time);

	// カメラシェイクに関するパラメータの設定
	if (ImGui::TreeNode("ShakeParameters")) {
		ImGui::DragFloat("ShakeTime", &shakeTime_, 0.01f, 0.01f); // 振動秒数
		ImGui::DragFloat2("ShakeStrength", &shakeStrength_.x, 0.1f); // カメラ振動強さ

		ImGui::TreePop();
	}

	// アニメーション時の待機時間
	if (ImGui::TreeNode("ReadyTime")) {
		// 落下攻撃待機時間
		ImGui::DragFloat("FallAttackReadyTime", &fallAttackReadyTime_, 0.01f, 0.01f, 2.0f);
		ImGui::DragFloat("PushUpReadyTime", &pushUpReadyTime_, 0.01f, 0.01f, 2.0f);
		ImGui::DragFloat("RollerAttackReadyTime", &rollerAttackReadyTime_, 0.01f, 0.01f, 2.0f);

		ImGui::TreePop();
	}

	if (ImGui::Button("save")) {
		GlobalVariables* variables = GlobalVariables::GetInstance();
		variables->SetValue(name_, "HitPoint", hitPoint_);
		variables->SetValue(name_, "waitSingle", waitForSingle_);
		variables->SetValue(name_, "waitmulti", waitForMulti_);
		variables->SetValue(name_, "waitRoller", waitForRoller_);
		variables->SetValue(name_, "waitPushUp", waitForPushUp_);
		variables->SetValue(name_, "FallAttackReadyTime", fallAttackReadyTime_);
		variables->SetValue(name_, "PushUpReadyTime", pushUpReadyTime_);
		variables->SetValue(name_, "RollerAttackReadyTime", rollerAttackReadyTime_);
		variables->SetValue(name_, "ShakeTime", shakeTime_);
		variables->SetValue(name_, "ShakeStrength", shakeStrength_);
		variables->SaveFile(name_);
	}if (ImGui::Button("changeStateAtack")) {
		ChangeState(std::make_unique<SingleAtackState>());
		
		return;
	}
	if (ImGui::Button("changeStateMultiAtack")) {
		ChangeState(std::make_unique<MultiAtackState>());
		return;
	}
	if (ImGui::Button("changeStatepushUp")) {
		ChangeState(std::make_unique<PushUpAtackState>());
		return;
	}

	if (ImGui::Button("changeStateRoller")) {
		ChangeState(std::make_unique<RollerAtackState>());
		
		return;
	}
	ImGui::InputInt("Pattern", &patternNumber_);
	ImGui::InputInt("State", &StateNumber_);
}

void Boss::Reset()
{
}

void Boss::SetParameter(const std::string& levelName, const std::string& enemyName)
{
	levelName, enemyName;
}

void Boss::ApplyParameter(const std::string& levelName, const std::string& enemyName)
{
	levelName, enemyName;
}

void Boss::OnCollisionEnter(Collider* collider)
{
	
	//プレイヤーが攻撃していたらダメージをくらう
	if (collider->GetGameObject()->GetObjectTag() == BaseObject::TagPlayer && player_->GetIsAtack()) {
		// ボスのHPをプレイヤーの攻撃力に基づいて減少させる
		hitPoint_ -= player_->GetAtackPower();
		// プレイヤーを攻撃していない状態に
		player_->SetIsAtack(false);
		
		//吸収した数をリセットして座標とスケール調整
		player_->ResetAbsorptionCount();

		//チュートリアルの攻撃を終了
		tutrialAtackEnd_ = true;

		// パーティクル再生
		Vector3 generatePos = transform_.translate_;
		generatePos.y = 7.0f;
		pem_->CreateEmitter<HitParticleEmiiter, HitParticle>("Boss_Hit", 25, 25, generatePos, 1.0f, 10.0f, TextureManager::Load("bossHitParticle.png"));

		// カメラのシェイク演出をする
		inGameCamera_->Shake(shakeTime_, shakeStrength_);

		// HPが0以下になっていたら
		if (hitPoint_ <= 0) {
			if (!isTutrial_) {
				// 行動状態を強制的に変更
				ChangeState(std::make_unique<WaitTimeState>());

				// アニメーションマネージャーセットされている場合
				if (bam_ != nullptr) {
					// ダメージアニメーション再生
					bam_->PlayDeadAnim();
				}
			}
		}
		else {
			// アニメーションマネージャーセットされている場合
			if (bam_ != nullptr) {
				// ダメージアニメーション再生
				bam_->PlayDamageAnim();
			}
		}
	}
}

void Boss::OnCollision(Collider* collider)
{
		//プレイヤーが攻撃していたらダメージをくらう
		if (collider->GetGameObject()->GetObjectTag() == BaseObject::TagPlayer && player_->GetIsAtack()) {
			// ボスのHPをプレイヤーの攻撃力に基づいて減少させる
			hitPoint_ -= player_->GetAtackPower();
			// プレイヤーを攻撃していない状態に
			player_->SetIsAtack(false);

			//吸収した数をリセットして座標とスケール調整
			player_->ResetAbsorptionCount();

			// パーティクル再生
			Vector3 generatePos = transform_.translate_;
			generatePos.y = 7.0f;
			pem_->CreateEmitter<HitParticleEmiiter, HitParticle>("Boss_Hit", 25, 25, generatePos, 1.0f, 10.0f, TextureManager::Load("bossHitParticle.png"));

			// カメラのシェイク演出をする
			inGameCamera_->Shake(shakeTime_, shakeStrength_);

			//チュートリアルの攻撃を終了
			tutrialAtackEnd_ = true;
			// HPが0以下になっていたら
			if (hitPoint_ <= 0) {
				if (!isTutrial_) {
					// 行動状態を強制的に変更
					ChangeState(std::make_unique<WaitTimeState>());


					// アニメーションマネージャーセットされている場合
					if (bam_ != nullptr) {
						// ダメージアニメーション再生
						bam_->PlayDeadAnim();
					}
				}
			}
			else {
				// アニメーションマネージャーセットされている場合
				if (bam_ != nullptr) {
					// ダメージアニメーション再生
					bam_->PlayDamageAnim();
				}
			}

		}
	
}

void Boss::MakeStateList()
{
	for (int i = 0; i < 5; i++) {
		// 番号
		std::string section = "TableList" + std::to_string(i);
		// アクション内容
		std::list<std::string> actList = gameDataManager_->GetValue<std::list<std::string>>({ "TableData",section }, "ActionList");
		stateList_.state_.resize(i + 1);
		for (std::string nameValue : actList) {
			stateList_.state_.at(i).push_back(MakeState(nameValue));
		}
		// パターン番号
		std::list<int> numList = gameDataManager_->GetValue<std::list<int>>({ "TableData",section }, "NumberList");
		stateList_.stateNumber_.resize(i + 1);
		
		for (int numValue : numList) {
			stateList_.stateNumber_.at(i).push_back(numValue);
		}
	}
}

void Boss::ApplyGlobalVariables()
{
	GlobalVariables* variables = GlobalVariables::GetInstance();

	hitPoint_=variables->GetFloatValue(name_, "HitPoint");
	kMaxHitPoint_ = hitPoint_;
	waitForSingle_=variables->GetFloatValue(name_, "waitSingle");
	waitForMulti_=variables->GetFloatValue(name_, "waitmulti");
	waitForRoller_=variables->GetFloatValue(name_, "waitRoller");
	waitForPushUp_=variables->GetFloatValue(name_, "waitPushUp");
	fallAttackReadyTime_ = variables->GetFloatValue(name_, "FallAttackReadyTime");
	pushUpReadyTime_ = variables->GetFloatValue(name_, "PushUpReadyTime");
	rollerAttackReadyTime_ = variables->GetFloatValue(name_, "RollerAttackReadyTime");
	shakeTime_ = variables->GetFloatValue(name_, "ShakeTime");
	shakeStrength_ = variables->GetVector2Value(name_, "ShakeStrength");
}

std::unique_ptr<IEnemyState> Boss::MakeState(std::string name)
{
	//文字列にあったStateを生成　文字列が一致しなければ強制的にMultiAtackに
	if (name == "MultiAttack") {
		return std::make_unique<MultiAtackState>();
	}
	else if (name == "SingleAttack") {
		return std::make_unique<SingleAtackState>();
	}
	else if (name == "Roller") {
		return std::make_unique<RollerAtackState>();
	}
	else if (name == "PushUp") {
	 	return std::make_unique<PushUpAtackState>();
	}
	else {
		return std::make_unique<MultiAtackState>();
	}
}
