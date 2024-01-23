#pragma once
#include "../../BaseObject.h"

/// <summary>
/// フェンス描画クラス
/// </summary>
class Fences : public BaseObject
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

	/// <summary>
	/// 柵を追加する関数
	/// </summary>
	/// <param name="translate">追加する柵の座標</param>
	/// <param name="rotate">追加する柵の回転角</param>
	/// <param name="scale">追加する柵の大きさ</param>
	void Add(const Vector3& translate, const Vector3& rotate, const Vector2& scale);

private: // メンバ変数

	// 柵のトランスフォーム配列
	std::vector<std::unique_ptr<WorldTransform>> fenceTransforms_;

};

