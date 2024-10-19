#pragma once
#include "Action.h"

/// <summary>
/// コンボクラス
/// </summary>
class Combo
{
public: // コンストラクタ等

	/// <summary>
	/// コンストラクタ
	/// </summary>
	/// <param name="name">読み込むコンボ名称</param>
	/// <param name="player">プレイヤー本体</param>
	Combo(const std::string& name, Player* player);
	/// <summary>
	/// デストラクタ
	/// </summary>
	virtual ~Combo() = default;

public: // メンバ関数

	/// <summary>
	/// 初期化関数
	/// </summary>
	void Init();

	/// <summary>
	/// 更新関数
	/// </summary>
	void Update();

private: // メンバ変数

	
};

