#pragma once
#include "Engine/GameObject/QuatWorldTransform.h"

#include <vector>
#include <string>

// 構造体の前方宣言
struct aiNode;

/// <summary>
/// ノード情報クラス
/// </summary>
class Node
{
public: // コンストラクタ等

	/// <summary>
	/// コンストラクタ
	/// </summary>
	Node() = default;
	/// <summary>
	/// デストラクタ
	/// </summary>
	~Node() = default;

public: // メンバ関数

	/// <summary>
	/// assimpが読み込んだノード情報を読み込む関数
	/// </summary>
	/// <param name="node">読み込むノード</param>
	void Load(aiNode* node);

public: // アクセッサ等

	/// <summary>
	/// LocalMatrixゲッター
	/// </summary>
	/// <returns>LocalMatrix</returns>
	Matrix4x4 GetLocalMatrix() const;

public: // パブリックメンバ変数

	// ノード名
	std::string name_ = "";

	// ノードワールド座標
	QuatWorldTransform transform_;
	// 子ノード配列
	std::vector<Node> children_;

};

