#include "IEnemyState.h"
#include "../IEnemy.h"

void IEnemyState::PreInit(IEnemy* enemy)
{
	// 敵の実体を取得
	enemy_ = enemy;
	gameObjectmanager_ = GameObjectManager::GetInstance();
}

void IEnemyState::DisplayImGui(){
	// 基底クラスでは記述なし
}

void IEnemyState::AddParameter(const std::string& levelName, const std::string& enemyName)
{
	// 基底クラスでは記述なし、以下はエラー回避用記述
	levelName;
	enemyName;
}

void IEnemyState::SetParameter(const std::string& levelName, const std::string& enemyName)
{
	// 基底クラスでは記述なし、以下はエラー回避用記述
	levelName;
	enemyName;
}

void IEnemyState::ApplyParameter(const std::string& levelName, const std::string& enemyName)
{
	// 基底クラスでは記述なし、以下はエラー回避用記述
	levelName;
	enemyName;
}
