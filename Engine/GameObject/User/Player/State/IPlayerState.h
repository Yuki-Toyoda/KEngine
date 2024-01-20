#pragma once
#include <string>
#include "../../../../Utility/KLib.h"
class Player;
class IPlayerState
{
public: // コンストラクタ等

	// 仮想デストラクタ
	virtual ~IPlayerState() = default;
public: // メンバ関数

	/// <summary>
	/// (ユーザー呼び出し禁止)共通初期化関数
	/// </summary>
	/// <param name="enemy">敵本体</param>
	void PreInit(Player* player);
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
	/// パラメーターを調整項目クラスに追加する関数
	/// </summary>
	/// <param name="levelName">レベル名</param>
	/// <param name="enemyName">敵名</param>
	virtual void AddParameter(const std::string& levelName, const std::string& enemyName);
	/// <summary>
	/// パラメーター保存関数
	/// </summary>
	/// <param name="levelName">レベル名</param>
	/// <param name="enemyName">敵名</param>
	virtual void SetParameter(const std::string& levelName, const std::string& enemyName);
	/// <summary>
	/// パラメーター読み込み関数
	/// </summary>
	/// <param name="levelName">レベル名</param>
	/// <param name="enemyName">敵名</param>
	virtual void ApplyParameter(const std::string& levelName, const std::string& enemyName);

public: // パブリックなメンバ変数

	// 行動名
	std::string name_ = "Default";

protected: // 継承先メンバ変数

	// プレイヤーの本体
	 Player* player_= nullptr;
	 Vector3 velocity_;
};

