#include "EnemyManager.h"

void EnemyManager::Init()
{
	// 全ての敵のリセットを行う
	for (IEnemy* e : enemies_) {
		e->Reset();
	}
}

void EnemyManager::Update()
{
	// 全ての敵の共通更新を呼び出す
	for (IEnemy* e : enemies_) {
		e->PreUpdate();
	}
}
