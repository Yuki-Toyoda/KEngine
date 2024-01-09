#pragma once
#include "../BaseScene.h"
#include "../SceneList.h"
/// <summary>
/// ゲームシーンクラス
/// </summary>
class GameScene final : public BaseScene {
public: // メンバ関数

	/// <summary>
	/// 初期化関数
	/// </summary>
	void Init() override;

	/// <summary>
	/// 更新関数
	/// </summary>
	void Update() override;

private: // メンバ変数
	void AtackAfterInit();

private:
	Player*player_;
	Wepon* wepon_;
	std::list<Enemy*>enemies_;
	std::vector<Obstacle*> obstacles_;
	Chain* chain_;
	InGameCamera* camera_;
};

