#include "IEnemy.h"

void IEnemy::PreInit()
{
	// インスタンス取得
	audio_ = Audio::GetInstance();

	// 武器とのペアレントフラグfalse
	isParent_ = false;

	// ワールド座標を取得
	worldPos_ = transform_.GetWorldPos();
}
