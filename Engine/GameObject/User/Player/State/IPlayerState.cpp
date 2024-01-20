#include "IPlayerState.h"

void IPlayerState::PreInit(Player* player)
{

	player_ = player;
}


void IPlayerState::DisplayImGui()
{
}

void IPlayerState::AddParameter(const std::string& levelName, const std::string& enemyName)
{
	// 基底クラスでは記述なし、以下はエラー回避用記述
	levelName;
	enemyName;
}

void IPlayerState::SetParameter(const std::string& levelName, const std::string& enemyName)
{
	// 基底クラスでは記述なし、以下はエラー回避用記述
	levelName;
	enemyName;
}

void IPlayerState::ApplyParameter(const std::string& levelName, const std::string& enemyName)
{
	// 基底クラスでは記述なし、以下はエラー回避用記述
	levelName;
	enemyName;
}
