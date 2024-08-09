#pragma once
#include "../Math/Vector2.h"
#include "../Math/Vector3.h"
#include "../Math/Vector4.h"
#include "../Math/Quaternion.h"
#include "../GameObject/WorldTransform.h"
#include "../Model/ParticleModel.h"

#include <string>

/// <summary>
/// 共通パーティクルデータ
/// </summary>
struct CommonParticleData {
	Vector3 translate;	 // 位置座標
	Vector3 scale;		 // 大きさ
	Vector3 velocity;	 // 速度ベクトル
	float	currentTime; // 現在時間
	float	lifeTime;	 // 生存時間全体
	Vector4 color;		 // 色
};

/// <summary>
/// GPUパーティクル
/// </summary>
class Particle
{
public: // コンストラクタ等

	// デフォルトコンストラクタ削除
	Particle() = default;

	/// <summary>
	/// コンストラクタ
	/// </summary>
	/// <param name="maxParticleSize">パーティクル最大数</param>
	Particle(uint32_t maxParticleSize = 1024) : kMaxParticleCount_(maxParticleSize) {};

	/// <summary>
	/// デストラクタ
	/// </summary>
	~Particle() = default;

public: // メンバ関数

	/// <summary>
	/// 初期化関数
	/// </summary>
	/// <param name="emitShaderPath">パーティクル生成をする際に使用するシェーダーパス</param>
	/// <param name="initShaderPath">初期化に使用するシェーダーパス</param>
	/// <param name="updateShaderPath">更新に使用するシェーダーパス</param>
	void Init(const std::string& emitShaderPath, const std::string& initShaderPath, const std::string& updateShaderPath);

	/// <summary>
	/// 更新関数
	/// </summary>
	void Update();

	/// <summary>
	/// 描画関数
	/// </summary>
	/// <param name="cmdList">コマンドリスト</param>
	void Draw(ID3D12GraphicsCommandList6* cmdList);

private: // メンバ変数

	// 粒子最大数
	const uint32_t kMaxParticleCount_;

	// パーティクル用バッファ
	std::unique_ptr<RWStructuredBuffer<CommonParticleData>> data_;

	// エミッタシェーダーまでのフルパス
	std::string emitShaderPath_ = "";
	// 初期化シェーダーまでのフルパス
	std::string emitShaderPath_ = "";
	// 
	std::string emitShaderPath_ = "";

};

