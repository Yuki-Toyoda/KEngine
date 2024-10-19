#pragma once
#include <string>
#include "Engine/GlobalVariables/GlobalVariables.h"
#include "Engine/Utility/Timer/DeltaTimer.h"
#include "Engine/Model/SkiningModel.h"

/// <summary>
/// コンボ基底クラス
/// </summary>
class ICombo
{
public: // コンストラクタ等

	/// <summary>
	/// コンストラクタ
	/// </summary>
	/// <param name="name">読み込むコンボ名称</param>
	ICombo(const std::string& name, SkiningModel* skiningModel);
	/// <summary>
	/// デストラクタ
	/// </summary>
	virtual ~ICombo() = default;

public: // メンバ関数

	/// <summary>
	/// 初期化関数
	/// </summary>
	void Init();

	void Update();

private: // メンバ変数

	// コンボ名
	std::string name_ = "";

	// プレイヤーモデル
	SkiningModel* model_ = nullptr;

	// 硬直時間タイマー
	KLib::DeltaTimer stunTimer_{};
	
	// 当該コンボ数
	int32_t comboCount_ = 0;
	// コンボ中再生されるアニメーション名
	std::string animName_ = "";
};

