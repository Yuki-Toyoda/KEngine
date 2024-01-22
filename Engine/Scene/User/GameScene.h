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

private:

	Player*player_;
	Weapon* weapon_;
	std::vector<Obstacle*> obstacles_;
	Chain* chain_;
	Ground* ground_;
	InGameCamera* camera_;

};
