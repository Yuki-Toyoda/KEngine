#pragma once
#include "../IPrimitive.h"

/// <summary>
/// 3Dモデル
/// </summary>
class Mesh final : public IPrimitive
{
public: // メンバ関数

	/// <summary>
	/// コンストラクタに関しては形状基底クラスのを使用
	/// </summary>
	using IPrimitive::IPrimitive;

	/// <summary>
	/// (呼び出し禁止)頂点配列を形状の頂点数にリサイズする関数
	/// </summary>
	void ResizeVertices() override { /*使用しないため記述なし*/ };
	/// <summary>
	/// (呼び出し禁止)インデックス情報配列を形状の情報をもとにリサイズする関数
	/// </summary>
	void ResizeIndexes() override { /*使用しないため記述なし*/ };

	/// <summary>
	/// モデルデータ読み込み関数
	/// </summary>
	/// <param name="filePath">モデルまでのディレクトリ</param>
	/// <param name="fileName">モデル名</param>
	void LoadFile(const std::string& filePath, const std::string& fileName);

public: // アクセッサ等

	/// <summary>
	/// 頂点数ゲッター
	/// </summary>
	/// <returns>頂点数</returns>
	int GetVertexCount() const override { return static_cast<int>(vertices_.size()); }
	/// <summary>
	/// インデックス情報数ゲッター
	/// </summary>
	/// <returns>インデックス情報の数</returns>
	int GetIndexCount() const override { return static_cast<int>(indexes_.size()); }

private: // プライベートなメンバ関数

	/// <summary>
	/// objの読み込み関数
	/// </summary>
	/// <param name="filePath">モデルまでのディレクトリ</param>
	/// <param name="fileName">モデル名</param>
	void LoadObj(const std::string& filePath, const std::string& filename);

	/// <summary>
	/// マテリアル読み込み関数
	/// </summary>
	/// <param name="filePath">マテリアルまでのディレクトリ</param>
	/// /// <param name="fileName">マテリアル名</param>
	void LoadMaterial(const std::string& filePath, const std::string& fileName);

};

