#pragma once
#include <string>
#include "../../Externals/imgui/ImGui.h"
#include "../math/Math.h"

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

	// 行列を直接代入したい場合はこれらの変数に代入する
	const Matrix4x4* rotateMat_ = nullptr; // 現在は回転行列のみ対応

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
	void Initialize();

	/// <summary>
	/// デバッグ用ImGui表示関数
	/// </summary>
	void DisplayImGui();

private: // メンバ変数

	// 親子関係
	const WorldTransform* parent_;

	// 親子関係タイプ
	// 0x111 ... scale, rotate, translate 全て
	// それぞれ scale rotate translate
	uint8_t parentType_;

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
	const WorldTransform* GetParent();

	/// <summary>
	/// 回転行列セッター
	/// </summary>
	/// <param name="mat">セットする回転行列</param>
	void SetRotateMat(const Matrix4x4& mat) { rotateMat_ = &mat; }

	/// <summary>
	/// セットされている回転行列を削除する関数
	/// </summary>
	void ResetRotateMat() { rotateMat_ = nullptr; }

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

};