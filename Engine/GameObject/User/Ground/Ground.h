#pragma once
#include "../../BaseObject.h"
#include"../Player/Weapon.h"
class Ground:public BaseObject
{
public:
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

	void Damage( Weapon* wepon);
private:

	int HP_;

};

