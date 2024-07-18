#pragma once
#include <string>
#include "../../Externals/imgui/ImGui.h"
#include "../Utility/Lerp/Lerp.h"
#include "../Math/Vector3.h"
#include "../Math/Matrix4x4.h"
#include "../Math/Quaternion.h"
#include <vector>
#include <map>
#include <optional>

/// <summary>
/// ワールド座標計算クラス
/// </summary>
class WorldTransform {
public: // パブリックメンバ変数

	// 拡縮
	Vector3 scale_;
	// 回転
	Vector3 rotate_;
	// 位置
	Vector3 translate_;

public: // パブリックメンバ関数

	/// <summary>
	/// コンストラクタ
	/// </summary>
	WorldTransform();
	/// <summary>
	/// コンストラクタ
	/// </summary>
	/// <param name="scale">拡縮</param>
	/// <param name="rotate">回転</param>
	/// <param name="translate">位置</param>
	WorldTransform(Vector3 scale, Vector3 rotate, Vector3 translate);
	/// <summary>
	/// コンストラクタ
	/// </summary>
	/// <param name="rotate">回転</param>
	/// <param name="translate">位置</param>
	WorldTransform(Vector3 rotate, Vector3 translate);
	/// <summary>
	/// コンストラクタ
	/// </summary>
	/// <param name="translate">位置</param>
	WorldTransform(Vector3 translate);

	/// <summary>
	/// 初期化
	/// </summary>
	void Init();

	/// <summary>
	/// デバッグ用ImGui表示関数
	/// </summary>
	void DisplayImGui();

	/// <summary>
	/// デバッグ用ImGui表示関数
	/// </summary>
	/// <param name="id">ウィンドウ表示名</param>
	void DisplayImGui(const std::string& id);
	/// <summary>
	/// デバッグ用ImGui表示関数
	/// </summary>
	/// <param name="id">ツリーノード名</param>
	void DisplayImGuiWithTreeNode(const std::string& id);

private: // メンバ変数

	// 親子関係
	const WorldTransform* parent_ = nullptr;

	// 親子関係タイプ
	// 0x111 ... scale, rotate, translate 全て
	// それぞれ scale rotate translate
	uint8_t parentType_;

	// 直接指定を行うワールド行列
	const Matrix4x4* worldMat_ = nullptr;

public: // アクセッサ等

	/// <summary>
	/// 親子関係のセッター
	/// </summary>
	/// <param name="parent">セットするワールド座標</param>
	/// <param name="parentType">親子関係フラグ</param>
	void SetParent(WorldTransform* parent, uint8_t parentType = 0b111);
	/// <summary>
	/// 親のゲッター
	/// </summary>
	/// <returns>親</returns>
	const WorldTransform* GetParent() const;

	/// <summary>
	/// ワールド行列セッター
	/// </summary>
	/// <param name="mat">セットするワールド行列</param>
	void SetWorldMat(Matrix4x4* mat) { worldMat_ = mat; };

	/// <summary>
	/// 現在のワールド行列のゲッター
	/// </summary>
	/// <returns>ワールド行列</returns>
	Matrix4x4 GetMatWorld() const;

	/// <summary>
	/// 現在のワールド座標のゲッター
	/// </summary>
	/// <returns>ワールド座標</returns>
	Vector3 GetWorldPos() const;

public: // 演算子オーバーロード

	// 演算子オーバーロード(+)
	WorldTransform operator+(const WorldTransform wt) const;

	// 演算子オーバーロード(-)
	WorldTransform operator-(const WorldTransform wt) const;

	// 演算子オーバーロード(*)
	WorldTransform operator*(const WorldTransform wt) const;
	// 演算子オーバーロード(*)(float)
	WorldTransform operator*(const float f) const;

	// 演算子オーバーロード(/)
	WorldTransform operator/(const WorldTransform wt) const;

	// 演算子オーバーロード(=)
	bool operator==(const WorldTransform wt) const;
};