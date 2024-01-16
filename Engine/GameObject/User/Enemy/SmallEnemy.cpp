#include "SmallEnemy.h"
#include "State/EnemyStateList.h"

void SmallEnemy::Init()
{
	/// 各トランスフォームの初期化
	// 身体
	bodyTransform_.Init();
	bodyTransform_.SetParent(&transform_);
	// 左羽
	wingTransform_L_.Init();
	wingTransform_L_.SetParent(&transform_);
	wingTransform_L_.translate_ = { 0.35f, 0.0f, 0.75f };
	// 右羽
	wingTransform_R_.Init();
	wingTransform_R_.SetParent(&transform_);
	wingTransform_R_.translate_ = { -0.35f, 0.0f, 0.75f };

	// メッシュの追加
	AddMesh(&bodyTransform_, color_, "./Resources/Enemy", "Body.obj");
	AddMesh(&wingTransform_L_, color_, "./Resources/Enemy", "Wing_L.obj");
	AddMesh(&wingTransform_R_, color_, "./Resources/Enemy", "Wing_R.obj");

	// 行動状態を変更
	ChangeState(std::make_unique<EnemySampleState>());
}

void SmallEnemy::Update()
{

}

void SmallEnemy::DisplayImGui()
{
}

void SmallEnemy::Reset()
{
}

void SmallEnemy::SaveParameter(const std::string& levelName, const std::string& enemyName)
{
	levelName;
	enemyName;
}

void SmallEnemy::LoadParameter(const std::string& levelName, const std::string& enemyName)
{
	levelName;
	enemyName;
}

void SmallEnemy::OnCollisionEnter(Collider* collider)
{
	collider;
}
