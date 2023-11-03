#pragma once
#include "../../Math/Math.h"
#include "../../GameObject/WorldTransform.h"

// 描画状態列挙子クラス
enum class fillMode : bool {
	Fill,	  // 埋め立て
	WireFrame // ワイヤーフレーム
};
/// <summary>
/// マテリアルクラス
/// </summary>
class Material final
{
public: // メンバ関数

	// コンストラクタ
	Material();
	// デストラクタ
	~Material() = default;

public: // アクセッサ等

	/// <summary>
	/// インデックス情報ゲッター
	/// </summary>
	/// <returns>コマンドマネージャー上でのインデックス情報(定数値)</returns>
	int GetIndex() const { return index_; }
	/// <summary>
	/// インデックス情報セッター
	/// </summary>
	/// <param name="index">設定するインデックス</param>
	void SetIndex(int index) { index_ = index; }

public: // パブリックなメンバ変数

	// uvトランスフォーム
	WorldTransform uvTransform_;

	// ライティングを行うか
	bool enableLighting_ = false;

	// 形状描画設定
	fillMode fillMode_ = fillMode::Fill;

private: // メンバ変数

	// コマンドマネージャー上でのインデックス情報
	int index_;
};

