#pragma once
#include "Joint.h"

#include <map>

/// <summary>
/// スケルトンクラス
/// </summary>
class Skelton
{
public: // コンストラクタ

	/// <summary>
	/// コンストラクタ
	/// </summary>
	Skelton() = default;

	/// <summary>
	/// デストラクタ
	/// </summary>
	~Skelton() = default;

public: // メンバ関数

	/// <summary>
	/// 更新関数
	/// </summary>
	void Update();

	/// <summary>
	/// スケルトン生成関数
	/// </summary>
	/// <param name="node">読み込むノード</param>
	void Create(const Node& node);

public: // パブリックメンバ変数

	// 親ジョイントのインデックス
	int32_t						   root_{};
	// ジョイント名検索用
	std::map<std::string, int32_t> jointMap_;
	// ジョイント配列
	std::vector<Joint>			   joints_;

};

