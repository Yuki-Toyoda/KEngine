#include "Boss.h"

void Boss::SuccessorInit()
{
	transform_.scale_.y = 14.0f;
	AddMesh(&transform_, color_, "./Engine/Resource/Samples/Box", "Box.obj");
	color_ = { 0.6f,0.6f,0.0f,1.0f };
	ChangeState(std::make_unique<SingleAtackState>());
}

void Boss::SuccessorUpdate()
{
}

void Boss::DisplayImGui()
{
	IEnemy::DisplayImGui();
	if (ImGui::Button("changeStateAtack")) {
		ChangeState(std::make_unique<SingleAtackState>());
		return;
	}
	if (ImGui::Button("changeStateMultiAtack")) {
		ChangeState(std::make_unique<MultiAtackState>());
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
