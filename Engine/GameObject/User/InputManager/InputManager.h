#pragma once
#include "../../../Input/Input.h"
#include "../../../Math/Math.h"

/// <summary>
/// 入力マネージャ
/// </summary>
class InputManager
{
public: // メンバ関数

	/// <summary>
	/// 初期化関数
	/// </summary>
	static void Init();

	/// <summary>
	/// 更新関数
	/// </summary>
	static void Update();

public:
	
	/*static bool MoveLeft();
	static bool MoveUp();
	static bool MoveDown();
	static bool MoveRight();
	static bool RotateRight();
	static bool RotateLeft();*/

	/// <summary>
	/// 移動関数
	/// </summary>
	/// <returns>移動ベクトル</returns>
	static Vector3 Move(Vector3 velocity);

	/// <summary>
	/// 回転方向変化関数
	/// </summary>
	/// <returns></returns>
	static bool ChangeRotate();

	/// <summary>
	/// 攻撃関数
	/// </summary>
	/// <returns></returns>
	static bool Atack();
	
private:

	// 入力
	static XINPUT_STATE joyState_;
	// 前フレーム入力
	static XINPUT_STATE preJoyState_;
};

