#include "IEnemy.h"

void IEnemy::Init()
{
	// インスタンス取得
	audio_ = Audio::GetInstance();
	gameDataManager_ = GameDataManager::GetInstance();

	// 武器とのペアレントフラグfalse
	isParent_ = false;

	// ワールド座標を取得
	worldPos_ = transform_.GetWorldPos();

	// 固有初期化関数を呼び出し
	SuccessorInit();

	// ステートをセット
	//ChangeState(std::make_unique<EnemySampleState>());

}

void IEnemy::Update()
{
	// 行動状態クラスがあれば
	if (state_.get()) {
		// 行動状態の更新を行う
		state_->Update();
	}

	// 固有更新関数を呼び出す
	SuccessorUpdate();
}

void IEnemy::DisplayImGui()
{
	// 位置座標の表示
	transform_.DisplayImGui();

	// 行動状態のImGuiの表示
	ImGui::NewLine(); // 改行
	ImGui::Text("%s", state_->name_.c_str()); // 行動名表示
	state_->DisplayImGui(); // ImGui本体
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

void IEnemy::AddParameter(const std::string& levelName, const std::string& enemyName)
{
	// 基底クラスでは記述なし、以下はエラー回避用記述
	levelName;
	enemyName;
}

void IEnemy::SetParameter(const std::string& levelName, const std::string& enemyName)
{
	// 基底クラスでは記述なし、以下はエラー回避用記述
	levelName;
	enemyName;
}

void IEnemy::ApplyParameter(const std::string& levelName, const std::string& enemyName)
{
	// 基底クラスでは記述なし、以下はエラー回避用記述
	levelName;
	enemyName;
}
