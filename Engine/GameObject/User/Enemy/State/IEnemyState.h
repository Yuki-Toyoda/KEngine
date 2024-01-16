#pragma once
#include <string>
#include "../IEnemy.h"
#include "../../../../Utility/KLib.h"

/// <summary>
/// 敵の行動基底クラス
/// </summary>
class IEnemyState
{
public: // メンバ関数

	/// <summary>
	/// (ユーザー呼び出し禁止)共通初期化関数
	/// </summary>
	/// <param name="enemy">敵本体</param>
	void PreInit(IEnemy* enemy);
	/// <summary>
	/// 初期化関数
	/// </summary>
	virtual void Init() = 0;

	/// <summary>
	/// 更新関数
	/// </summary>
	/// <returns></returns>
	virtual void Update() = 0;

	/// <summary>
	/// ImGuiの表示関数
	/// </summary>
	virtual void DisplayImGui();

	/// <summary>
	/// パラメーター保存関数
	/// </summary>
	/// <param name="levelName">レベル名</param>
	/// <param name="enemyName">敵名</param>
	virtual void SaveParameter(const std::string& levelName, const std::string& enemyName);
	/// <summary>
	/// パラメーター読み込み関数
	/// </summary>
	/// <param name="levelName">レベル名</param>
	/// <param name="enemyName">敵名</param>
	virtual void LoadParameter(const std::string& levelName, const std::string& enemyName);

public: // パブリックなメンバ変数
	
	// 行動名
	std::string name_ = "Default";

protected: // 継承先メンバ変数

	// 敵の本体
	IEnemy* enemy_ = nullptr;

};

