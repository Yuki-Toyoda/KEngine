#include "Boss.h"
#include "../../../GlobalVariables/GlobalVariables.h"
#include "BossAnimManager.h"
#include "../../../Scene/FadeManager.h"

#include "../../../Particle/ParticleEmitterManager.h"

void Boss::SuccessorInit()
{
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
	// データマネージャーのインスタンス取得
	//GameDataManager* dataManager = GameDataManager::GetInstance();
	// 行動状態リストの生成
	MakeStateList();
	
	//下からの攻撃を生成
	// 全ての攻撃に対して
	//for (int i = 0; i < pushUpMax; i++) {
	//	// インスタンス生成
	//	PushUp* pushUp;
	//	pushUp = gameObjectManager_->CreateInstance<PushUp>("PushUp", BaseObject::TagNone);
	//	// 名前
	//	std::string name = "PushUp" + std::to_string(i);
	//	// Y座標以外を設定
	//	Vector3 newPos = dataManager->GetValue<Vector3>({ "PushUpAttack",name }, "Position");
	//	pushUp->transform_.translate_.x = newPos.x;
	//	pushUp->transform_.translate_.z = newPos.z;
	//	pushUp_.push_back(pushUp);
	//}
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
	ApplyGlobalVariables();
}

void Boss::SuccessorUpdate()
{
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

void Boss::DisplayImGui()
{
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
		
		// HPが0以下になっていたら
		if (hitPoint_ <= 0) {
			// 行動状態を強制的に変更
			ChangeState(std::make_unique<WaitTimeState>());

			// アニメーションマネージャーセットされている場合
			if (bam_ != nullptr) {
				// ダメージアニメーション再生
				bam_->PlayDeadAnim();
			}
		}
		else {
			// アニメーションマネージャーセットされている場合
			if (bam_ != nullptr) {
				// ダメージアニメーション再生
				bam_->PlayDamageAnim();
			}
		}
		ParticleEmitterManager::GetInstance()->CreateEmitter<IParticleEmitter, IParticle>("ParticleTest", 100, 25, { 0.0f,5.0f,0.0f }, 1, 2, TextureManager::Load("uvChecker.png"));

	}
}

void Boss::MakeStateList()
{
	/*stateList_.state_.resize(1);
	stateList_.state_.at(0).push_back(std::make_unique<WaitTimeState>());
	stateList_.stateNumber_.resize( 1);
	stateList_.stateNumber_.at(0).push_back(0);*/
	//for (int i = 0; i < 1; i++) {
	//	//行動状態のリストを作成
	//	stateList_.state_.resize(i+1);
	//	stateList_.state_.at(i).push_back(MakeState("SingleAtack"));
	//	stateList_.state_.at(i).push_back(MakeState("MultiAtack"));
	//	stateList_.state_.at(i).push_back(MakeState("Roller"));
	//	stateList_.state_.at(i).push_back(MakeState("PushUp"));
	//	stateList_.state_.at(i).push_back(MakeState("SingleAtack"));
	//	//行動状態のリストからどの種類を選ぶかの番号を設定
	//	stateList_.stateNumber_.resize(i+1);
	//	stateList_.stateNumber_.at(i).push_back(0);
	//	stateList_.stateNumber_.at(i).push_back(1);
	//	stateList_.stateNumber_.at(i).push_back(2);
	//	stateList_.stateNumber_.at(i).push_back(3);
	//	stateList_.stateNumber_.at(i).push_back(4);
	//}
	//std::list<int> testList = { 0,1,1,1,2 };
	//std::vector<int> tmpVector(testList.begin(), testList.end());
	//stateList_.stateNumber_.push_back(tmpVector);

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
		
		//std::vector<int> numVector(numList.begin(), numList.end());
		//stateList_.stateNumber_.push_back(numVector);

		//std::vector<std::string> actVector(actList.begin(), actList.end());
		//stateList_.state_.at(i).
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
