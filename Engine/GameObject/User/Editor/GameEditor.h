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

private:
	// データ用マネージャ
	GameDataManager* dataManager_ = nullptr;
	// オブジェクト用マネージャ
	GameObjectManager* gameObjectManager_ = nullptr;
	// ゲームシーンで使用するカメラ
	InGameCamera* camera_;

public:
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
	// マウス座標用（使ってない）
	Vector3 mouseV3Position_ = {};
	Vector2 mouseV2Position_ = {};

	// 障害物の最大数
	int kMaxObstacleCount_ = 0;

	// 敵の最大数
	int kMaxEnemyCount_ = 0;
	// for文の際の値
	int indexCount_ = 0;

	// 現在の難易度
	int editNowLevel_ = 0;

private: // 仮配置用のオブジェクト
	// 敵
	std::vector<Enemy*>enemies_;
	std::vector<MidEnemy*>midEnemies_;
	std::vector<HighEnemy*>highEnemies_;
	// ブロック
	std::vector<Obstacle*> obstacles_;


};

