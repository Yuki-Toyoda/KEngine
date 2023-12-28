#pragma once
#include"../../BaseObject.h"
#include "../../../Utility/Line/Line.h"
#include "../Player/Player.h"
#include "../Player/Wepon.h"
class Chain :public BaseObject
{
public: // メンバ関数

	/// <summary>
	/// 初期化関数
	/// </summary>
	void Init() override;

	/// <summary>
	/// 更新関数
	/// </summary>
	void Update() override;

	/// <summary>
	/// ImGui表示関数
	/// </summary>
	void DisplayImGui() override;

	
public:
	void SetWepon(Wepon* wepon) { wepon_ = wepon; }
	void SetPlayer(Player* player) { player_ = player; }

private:
	/// <summary>
	/// 衝突した瞬間にコールバックされる関数
	/// </summary>
	/// <param name="collider">衝突したコライダー</param>
	void OnCollisionEnter(Collider* collider) override;
private:

	std::unique_ptr<Line> line_;
	Player* player_;
	Wepon* wepon_;
	float length_;
};
