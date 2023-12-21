#pragma once
#include"../../BaseObject.h"
#include "../InputManager/InputManager.h"
class Player:public BaseObject
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


	private:

		Vector3 velocity_;
		float moveSpeed_ = 0.1f;
};

