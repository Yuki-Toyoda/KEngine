#pragma once
#include "IParticle.h"
#include <list>

/// <summary>
/// パーティクル発生箇所
/// </summary>
class IParticleEmitter
{
public: // コンストラクタ等

	// コンストラクタ
	IParticleEmitter() = default;
	// デストラクタ
	virtual ~IParticleEmitter() = default;

public: // メンバ関数

	/// <summary>
	/// 共通初期化関数
	/// </summary>
	/// <param name="name">名称</param>
	/// <param name="maxCount">粒子最大数</param>
	/// <param name="maxGenerateCount">一度に生成する粒子数</param>
	/// <param name="translate">発生座標</param>
	/// <param name="aliveTime">エミッタの生存時間</param>
	/// <param name="frequency">粒子生成間隔</param>
	/// <param name="texture">粒子のテクスチャ</param>
	void PreInit(const std::string& name, int32_t maxCount, int32_t maxGenerateCount, const Vector3& translate, float aliveTime, float frequency, Texture* texture);

	/// <summary>
	/// 初期化関数
	/// </summary>
	virtual void Init();

	/// <summary>
	/// 共通更新関数
	/// </summary>
	void PreUpdate();

	/// <summary>
	/// 更新処理
	/// </summary>
	virtual void Update();

	/// <summary>
	/// 更新後処理関数
	/// </summary>
	void PostUpdate();

public: // アクセッサ等

	/// <summary>
	/// 生成する粒子の型を設定する関数
	/// </summary>
	/// <typeparam name="SelectParticle">生成する粒子の型</typeparam>
	template<IsIParticle SelectParticle>
	void SetParticleType() {
		type_ = []()->std::unique_ptr<IParticle> {
			return std::make_unique<SelectParticle>();
		};
	}

	/// <summary>
	/// エミッタの終了状態ゲッター
	/// </summary>
	/// <returns>エミッタの終了状態</returns>
	bool GetIsEnd() { return isEnd_; }
	/// <summary>
	/// エミッタの終了状態セッター
	/// </summary>
	/// <param name="isEnd">エミッタを終了させるか</param>
	void SetIsEnd(const bool& isEnd) { isEnd_ = isEnd; }

	/// <summary>
	/// ループトリガーセッター
	/// </summary>
	/// <param name="isLoop">ループ状態</param>
	void SetIsLoop(const bool& isLoop) { isLoop_ = isLoop; }

	/// <summary>
	/// 再生トリガーセッター
	/// </summary>
	/// <param name="isPlay">再生トリガー</param>
	void SetIsPlay(const bool& isPlay) { isPlay_ = isPlay; }

protected: // 継承先メンバ関数

	/// <summary>
	/// 粒子生成関数
	/// </summary>
	virtual void GenerateParticle();

public: // その他関数群

	/// <summary>
	/// ImGui表示関数
	/// </summary>
	virtual void DisplayImGui();

public: // パブリックなメンバ変数

	// 発生座標
	WorldTransform transform_;

protected: // メンバ変数

	// パーティクル達
	std::list<std::unique_ptr<IParticle>> particles_;

	// パーティクル名
	std::string name_;

	// 粒子最大数
	int32_t maxCount_;

	// 一度に生成する粒子数
	int32_t generateParticleCount_;

	// エミッタの生存時間タイマー
	KLib::DeltaTimer aliveTimer_;
	// エミッタの終了トリガー
	bool isEnd_ = false;

	// ループトリガー
	bool isLoop_ = false;

	// 再生トリガー
	bool isPlay_ = true;

	// 生成する粒子のテクスチャ
	Texture* texture_;

	// 生成する粒子の型
	std::function<std::unique_ptr<IParticle>()> type_;

	// 粒子生成間隔タイマー
	KLib::DeltaTimer frequencyTimer_;
	float frequency_ = 0.0f;

};

template<class SelectEmiiter>
concept IsIParticleEmitter = std::is_base_of<IParticleEmitter, SelectEmiiter>::value;