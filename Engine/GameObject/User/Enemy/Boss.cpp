#include "Boss.h"

void Boss::SuccessorInit()
{
	transform_.scale_.y = 14.0f;
	AddMesh(&transform_, color_, "./Engine/Resource/Samples/Box", "Box.obj");
	color_ = { 0.6f,0.6f,0.0f,1.0f };
	ChangeState(std::make_unique<SingleAtackState>());
	hitPoint_ = 100.0f;
	gameObjectManager_ = GameObjectManager::GetInstance();
	//下からの攻撃を生成
	PushUp* pushUp;
	pushUp = gameObjectManager_->CreateInstance<PushUp>("PushUp", BaseObject::TagNone);
	pushUp_.push_back(pushUp);
}

void Boss::SuccessorUpdate()
{
	//プレイヤーが攻撃していたらダメージをくらう
	if (player_->GetIsAtack()) {
		hitPoint_ -= player_->GetAtackPower();
	}
}

void Boss::DisplayImGui()
{
	IEnemy::DisplayImGui();
	ImGui::DragFloat("HitPoint", &hitPoint_);
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
	collider;
}
