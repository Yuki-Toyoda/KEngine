#include "IEnemyState.h"

void IEnemyState::PreInit(IEnemy* enemy)
{
	// 敵の実体を取得
	enemy_ = enemy;
}

void IEnemyState::DisplayImGui()
{
	// 基底クラスでは記述なし
}

void IEnemyState::SaveParameter(const std::string& levelName, const std::string& enemyName)
{
	// 基底クラスでは記述なし、以下はエラー回避用記述
	levelName;
	enemyName;
}

void IEnemyState::LoadParameter(const std::string& levelName, const std::string& enemyName)
{
	// 基底クラスでは記述なし、以下はエラー回避用記述
	levelName;
	enemyName;
}
