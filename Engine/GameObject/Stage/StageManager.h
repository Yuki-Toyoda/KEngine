#pragma once
#include <list>
#include <string>
#include "Floor.h"
#include "MoveFloor.h"
#include "Goal.h"

// クラスの前方宣言
class GameObjectManager;
class GlobalVariables;

/// <summary>
/// ステージマネージャー
/// </summary>
class StageManager
{
public: // メンバ関数

	// 初期化
	void Initialize();

	// 更新
	void Update();

public: // その他関数

	/// <summary>
	/// ステージ床を生成する関数
	/// </summary>
	/// <param name="translate">生成する床の座標</param>
	/// <param name="scale">生成する床の大きさ</param>
	void AddStageFloor(Vector3 translate, Vector3 scale);

	/// <summary>
	/// ゴール状態の取得
	/// </summary>
	/// <returns>ゴールしているか</returns>
	bool GetIsGoal() {return goal_->GetIsGoaled();}

private: // メンバ変数

	// ゲームオブジェクトマネージャー
	GameObjectManager* gameObjectManager_ = nullptr;
	// 調整項目クラス
	GlobalVariables* globalVariables_ = nullptr;

	// ゴール座標
	Vector3 goalPos_;
	// ステージのゴール
	std::unique_ptr<Goal> goal_;

	std::unique_ptr<StageMoveFloor> moveFloor1_;
	std::unique_ptr<StageMoveFloor> moveFloor2_;

	// 床のリスト
	std::list<BaseObject*> stageFloorList_;
	// 床個数
	int32_t floorCount_;

	// ImGui用変数
	Vector3 AddFloorTranslate_; // 追加する床の座標
	Vector3 AddFloorScale_; // 追加する床の大きさ

};

