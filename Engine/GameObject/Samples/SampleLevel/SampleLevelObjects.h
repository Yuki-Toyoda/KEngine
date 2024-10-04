#pragma once
#include "Engine/GameObject/IObject.h"
#include "App/Level/LevelLoader.h"

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
	/// レベルロード関数
	/// </summary>
	/// <param name="filePath">レベルデータまでのファイルパス</param>
	/// <param name="fileName">レベルデータのファイル名</param>
	void LoadLevel(const std::string& filePath, const std::string& fileName);

	/// <summary>
	/// オブジェクト追加関数
	/// </summary>
	/// <param name="filePath">オブジェクトまでのファイルパス</param>
	/// <param name="fileName">オブジェクトファイル名</param>
	/// <param name="t">参照するトランスフォーム</param>
	void AddObjects(const std::string filePath, const std::string fileName, WorldTransform t);

protected: // 継承先メンバ変数

	// レベルローダー
	LevelLoader loader_;

	// レベル内のオブジェクトごとのワールドトランスフォーム配列
	std::vector<std::unique_ptr<WorldTransform>> transforms_;

	// ImGui用変数群
	char imGuiFilePath_[50] = "./Resources/Level";
	char imGuiFileName_[20] = "Stage.json";

};

