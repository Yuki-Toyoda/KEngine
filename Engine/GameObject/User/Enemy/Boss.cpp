#include "Boss.h"

void Boss::SuccessorInit()
{
	transform_.scale_.y = 14.0f;
	AddMesh(&transform_, color_, "./Engine/Resource/Samples/Box", "Box.obj");
	color_ = { 0.6f,0.6f,0.0f,1.0f };
	ChangeState(std::make_unique<WaitTimeState>());
	AddColliderOBB("Boss", &transform_.scale_, &transform_.scale_, &transform_.rotate_);
	hitPoint_ = 25.0f;
	gameObjectManager_ = GameObjectManager::GetInstance();
	GameDataManager* dataManager = GameDataManager::GetInstance();
	MakeStateList();
	
	//下からの攻撃を生成
	std::string group = gameDataManager_->GetPushUpAttack(0);
	int pushUpMax = dataManager->GetValue<int>({ group,"Parameter"}, "MaxCount");

	for (int i = 0; i < pushUpMax; i++) {
		PushUp* pushUp;
		pushUp = gameObjectManager_->CreateInstance<PushUp>("PushUp", BaseObject::TagNone);
		// 名前
		std::string name = "PushUp" + std::to_string(i);
		// Y座標以外を設定
		Vector3 newPos = dataManager->GetValue<Vector3>({ group,name }, "Position");
		pushUp->transform_.translate_.x = newPos.x;
		pushUp->transform_.translate_.z = newPos.z;
		Vector3 scale = dataManager->GetValue<Vector3>({ group,"Parameter" }, "Scale");
		pushUp->transform_.scale_.x = scale.x;
		pushUp->transform_.scale_.z = scale.z;
		pushUp_.push_back(pushUp);
	}
}

void Boss::SuccessorUpdate()
{

	if (hitPoint_ <= 0.0f) {
		isActive_ = false;
		state_ = nullptr;
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
	if (ImGui::Button("changeStateAtack")) {
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
