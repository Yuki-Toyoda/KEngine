#pragma once
#include <fstream>
#include <cassert>
#include "../../../Externals/nlohmann/json.hpp"

/// <summary>
/// Json経由のレベルローダー
/// </summary>
class LevelLoader
{
public: // メンバ関数

	/// <summary>
	/// ロード関数
	/// </summary>
	/// <param name="filePath">レベルまでのファイルパス</param>
	/// <param name="fileName">レベルファイル名</param>
	void Load(const std::string& filePath, const std::string fileName);

};

