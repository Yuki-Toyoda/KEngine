#include "IEnemy.h"
#include "State/EnemyStateList.h"

void IEnemy::PreInit()
{
	// インスタンス取得
	audio_ = Audio::GetInstance();

	// 武器とのペアレントフラグfalse
	isParent_ = false;

	// ワールド座標を取得
	worldPos_ = transform_.GetWorldPos();
}

void IEnemy::PreUpdate()
{
	// 行動状態クラスがあれば
	if (state_.get()) {
		// 行動状態の更新を行う
		state_->Update();
	}
}

void IEnemy::ChangeState(std::unique_ptr<IEnemyState> newState)
{
	// 共通初期化関数を実行
	newState->PreInit(this);
	// 初期化関数を実行
	newState->Init();

	// 新しい行動状態を受け渡す
	state_ = std::move(newState);
}
