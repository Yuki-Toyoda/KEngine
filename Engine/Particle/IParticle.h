#pragma once
#include "../GameObject/WorldTransform.h"
#include "../Resource/Texture/TextureManager.h"
#include "../Primitive/PrimitiveManager.h"
#include "../Primitive/2d/Plane.h"
#include "../Utility/KLib.h"

/// <summary>
/// 粒子１つ１つの基底クラス
/// </summary>
class IParticle
{
public: // コンストラクタ等

	// コンストラクタ
	IParticle() = default;
	// 仮想デストラクタ
	virtual ~IParticle();

public: // メンバ関数

	/// <summary>
	/// 共通初期化関数
	/// </summary>
	/// <param name="aliveTime">粒子の生存時間</param>
	/// <param name="position">生成座標</param>
	/// <param name="scale">初期サイズ</param>
	/// <param name="velocity">速度</param>
	/// <param name="texture">テクスチャ</param>
	/// <param name="color">色</param>
	void PreInit(float aliveTime, const Vector3& position, const Vector2& scale, const Vector3& velocity, Texture* texture, const Vector4& color);

	/// <summary>
	/// 初期化関数
	/// </summary>
	virtual void Init();

	/// <summary>
	/// 更新関数
	/// </summary>
	void Update();

	/// <summary>
	/// 更新後処理関数
	/// </summary>
	void PostUpdate();

public: // アクセッサ等
	
	/// <summary>
	/// 終了トリガー状態ゲッター
	/// </summary>
	/// <returns>終了トリガー状態</returns>
	const bool GetIsEnd() { return isEnd_; }
	/// <summary>
	/// 終了トリガー状態セッター
	/// </summary>
	/// <param name="isEnd">設定する状態</param>
	void SetIsEnd(bool isEnd) { isEnd_ = isEnd; }

public: // その他関数群

protected: // メンバ変数

	// 平面
	Plane* plane_ = nullptr;

	// パーティクル座標
	WorldTransform transform_;
	// 速度
	Vector3 velocity_ = { 0.0f, 0.0f, 0.0f };

	// 大きさ
	Vector2 scale_ = { 0.0f, 0.0f };
	// 回転
	float rotate_ = 0.0f;

	// アンカーポイント
	Vector2 anchorPoint_ = { 0.5f, 0.5f };

	// 色
	Vector4 color_;

	// テクスチャ始点
	Vector2 texBase_ = { 0.0f, 0.0f };
	// テクスチャ幅と高さ
	Vector2 texSize_ = { 100.0f, 100.0f };

	// 粒子の生存時間
	KLib::DeltaTimer aliveTimer_;

	// 粒子の終了トリガー
	bool isEnd_ = false;
};

/// <summary>
/// IParticleを継承したクラスを選択できるテンプレート
/// </summary>
template <class SelectParticle>
concept IsIParticle = std::is_base_of<IParticle, SelectParticle>::value;

