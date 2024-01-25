#include "Boss.h"
#include "../../../GlobalVariables/GlobalVariables.h"
void Boss::SuccessorInit()
{
	// 大きさ設定
	transform_.scale_.y = 14.0f;

	// メッシュ追加
	AddMesh(&transform_, color_, "./Engine/Resource/Samples/Box", "Box.obj");
	// 色の設定
	color_ = { 0.6f,0.6f,0.0f,1.0f };

	// 行動状態変更
	ChangeState(std::make_unique<WaitTimeState>());

	// OBBのコライダーを追加
	AddColliderOBB("Boss", &transform_.scale_, &transform_.scale_, &transform_.rotate_);
	
	// 最大体力設定
	kMaxHitPoint_ = 25.0f;
	// 体力を最大体力をもとに設定
	hitPoint_ = kMaxHitPoint_;

	// ゲームオブジェクトマネージャのインスタンスを取得
	gameObjectManager_ = GameObjectManager::GetInstance();
	// データマネージャーのインスタンス取得
	GameDataManager* dataManager = GameDataManager::GetInstance();
	// 行動状態リストの生成
	MakeStateList();
	
	//下からの攻撃を生成
	int pushUpMax = dataManager->GetValue<int>({ "AttackParam","PushUp" }, "MaxCount");
	pushUpMax;
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
	ApplyGlobalVariables();
}

void Boss::SuccessorUpdate()
{

	// HPが0以下になったら
	if (hitPoint_ <= 0.0f) {
		// 非表示
		isActive_ = false;
		// 行動状態削除
		//state_ = nullptr;
	}

}

void Boss::DisplayImGui()
{
	IEnemy::DisplayImGui();
	ImGui::DragFloat("HitPoint", &hitPoint_);
	ImGui::InputInt("PatternNumber", &patternNumber_);
	ImGui::DragFloat("waitSingle", &waitForSingle_,0.1f);
	ImGui::DragFloat("waitmulti", &waitForMulti_, 0.1f);
	ImGui::DragFloat("waitRoller", &waitForRoller_, 0.1f);
	ImGui::DragFloat("waitPushUp", &waitForPushUp_, 0.1f);
	if (ImGui::Button("save")) {
		GlobalVariables* variables = GlobalVariables::GetInstance();
		variables->SetValue(name_, "HitPoint", hitPoint_);
		variables->SetValue(name_, "waitSingle", waitForSingle_);
		variables->SetValue(name_, "waitmulti", waitForMulti_);
		variables->SetValue(name_, "waitRoller", waitForRoller_);
		variables->SetValue(name_, "waitPushUp", waitForPushUp_);
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
		hitPoint_ -= player_->GetAtackPower();
		player_->SetIsAtack(false);
	}
}

void Boss::MakeStateList()
{
	//行動状態のリストを作成
	stateList_.resize(1);
	stateList_.at(0).push_back(std::make_unique<SingleAtackState>());
	stateList_.at(0).push_back(std::make_unique<MultiAtackState>());
	stateList_.at(0).push_back(std::make_unique<RollerAtackState>());
	stateList_.at(0).push_back(std::make_unique<SingleAtackState>());
	stateList_.at(0).push_back(std::make_unique<MultiAtackState>());
	stateList_.resize(2);
	stateList_.at(1).push_back(std::make_unique<SingleAtackState>());
	stateList_.at(1).push_back(std::make_unique<SingleAtackState>());
	stateList_.at(1).push_back(std::make_unique<RollerAtackState>());
	stateList_.at(1).push_back(std::make_unique<SingleAtackState>());
	stateList_.at(1).push_back(std::make_unique<PushUpAtackState>());
}

void Boss::ApplyGlobalVariables()
{
	GlobalVariables* variables = GlobalVariables::GetInstance();

	hitPoint_=variables->GetFloatValue(name_, "HitPoint");
	waitForSingle_=variables->GetFloatValue(name_, "waitSingle");
	waitForMulti_=variables->GetFloatValue(name_, "waitmulti");
	waitForRoller_=variables->GetFloatValue(name_, "waitRoller");
	waitForPushUp_=variables->GetFloatValue(name_, "waitPushUp");
}			   
