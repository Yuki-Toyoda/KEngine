#pragma once
#include "../GameObject/WorldTransform.h"
#include "../Resource/Texture/TextureManager.h"
#include "../Primitive/PrimitiveManager.h"
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
	virtual ~IParticle() = default;

public: // メンバ関数

	
	/// <param name="color">色</param>
	/// <summary>
	/// 共通初期化関数
	/// </summary>
	/// <param name="aliveTime">粒子の生存時間</param>
	/// <param name="position">生成座標</param>
	/// <param name="scale">初期サイズ</param>
	/// <param name="velocity">速度</param>
	/// <param name="material">マテリアル</param>
	/// <param name="color">色</param>
	void PreInit(float aliveTime, const Vector3& position, const Vector3& scale, const Vector3& velocity, const Material& material, const Vector4& color);

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

public: // パブリックメンバ変数

	// パーティクル座標
	WorldTransform transform_;

	// 粒子単体のマテリアル
	Material material_;

protected: // メンバ変数

	// 速度
	Vector3 velocity_ = { 0.0f, 0.0f, 0.0f };

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
