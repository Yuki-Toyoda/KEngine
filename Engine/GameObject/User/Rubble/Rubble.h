#pragma once
#include "../../BaseObject.h"
#include "../../../Utility/KLib.h"
#include "../../User/GameManager/GameManager.h"
#include "../../../Particle/ParticleEmitterManager.h"

class Rubble:public BaseObject
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
	/// ImGui表示関数
	/// </summary>
	void DisplayImGui() override;
	void SetgameManager(GameManager* gamemanager) { gameManager_ = gamemanager; }
	// 遷移にかかる秒数
	float lerpTime_ = 5.0f;
	float moveTime_ = 0.0f;
	
public://アクセッサ
	void  SetGoal(Vector3 pos) { goalPos_ = pos; }
	void  SetStart(Vector3 pos) { startPos_ = pos; }
	void SetCollisionScale(Vector3 scale) { CollisionScale_ = scale; }
	void SetUpParticle();
private:
	void OnCollisionEnter(Collider* collider)override;
	
private:
	// 線形補間用タイマー
	KLib::DeltaTimer lerpTimer_;

	KLib::DeltaTimer aliveTimer_;
	GameManager* gameManager_;
	
	Vector3 startPos_;
	Vector3 goalPos_;
	Vector3 CollisionScale_;

private: // パーティクル
	// エミッタマネージャー
	ParticleEmitterManager* emitterManager_ = nullptr;

	// 消えるときのパーティクル
	IParticleEmitter* fadeParticleEmitter_ = nullptr;
	// フラグで出現開始を取得
	bool isGenerateParticle_ = false;

};

