#pragma once
#include "../../IObject.h"

/// <summary>
/// レベルローダー
/// </summary>
class SampleLevelObjects : public IObject
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
	/// ImGuiを表示させる関数
	/// </summary>
	void DisplayImGui() override;

public: // 機能関数群

	/// <summary>
	/// オブジェクト追加関数
	/// </summary>
	/// <param name="filePath">オブジェクトまでのファイルパス</param>
	/// <param name="fileName">オブジェクトファイル名</param>
	/// <param name="t">参照するトランスフォーム</param>
	void AddObjects(const std::string filePath, const std::string fileName, WorldTransform t);

private: // メンバ変数

	// レベル内のオブジェクトごとのワールドトランスフォーム配列
	std::vector<std::unique_ptr<WorldTransform>> transforms_;

};

