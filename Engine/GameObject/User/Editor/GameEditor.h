#pragma once
#include "GameDataManager.h"
#include "../../GameObjectManager.h"
#include "../../ObjectList.h"
#include "../../Core/Camera.h"
#include <vector>

#ifdef _DEBUG
#include "../../../../externals/imgui/imgui.h"
#endif // _DEBUG

class GameEditor
{
public:
	/// <summary>
	/// 初期化
	/// </summary>
	void Init();
	/// <summary>
	/// 更新
	/// </summary>
	void Update();

	/// <summary>
	/// Jsonから情報取得の関数
	/// </summary>
	void ParameterInitialize();

	/// <summary>
	/// エディターのImGui
	/// </summary>
	void ImGuiProgress();

	/// <summary>
	/// マウスの座標からワールド座標（使えない）
	/// </summary>
	void GetMouseCursor();

	/// <summary>
	/// データのリロード
	/// </summary>
	void DataReaload();

private:

	GameDataManager* dataManager_ = nullptr;
	GameObjectManager* gameObjectManager_ = nullptr;

	Vector3 mouseV3Position_ = {};
	Vector2 mouseV2Position_ = {};

	int kMaxEnemyCount_ = 0;

	int editPhaseNum_ = 0;
	int enemyCount_ = 0;

private:
	// 仮配置用のオブジェクト
	// 敵
	std::vector<Enemy*>enemies_;
	std::vector<MidEnemy*>midEnemies_;
	std::vector<HighEnemy*>highEnemies_;
	// ブロック
	std::vector<Obstacle*> obstacles_;

	// ゲームシーンで使用するカメラ
	InGameCamera* camera_;

};

