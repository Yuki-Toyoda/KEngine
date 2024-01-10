#pragma once
#include "../../BaseObject.h"
#include"../Player/Wepon.h"
#include "../../../Audio/Audio.h"
#include "../../GameObjectManager.h"
#include "Enemy.h"
class MidEnemy:public BaseObject
{
	
	public:
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

		void Reset();
	public:
		void SetStartTransform() { startTranslate_ = transform_.translate_; }
		//追従対象
		void SetWepon(Wepon* target) { wepon_ = target; }
		void Setveclocity(Vector3 velocity) { hitvelocity_ = velocity; }
	private:
		/// <summary>
		/// 衝突した瞬間にコールバックされる関数
		/// </summary>
		/// <param name="collider">衝突したコライダー</param>
		void OnCollisionEnter(Collider* collider) override;

		void OnCollisionExit(Collider* collider) override;
		void SubtractVelocity();
	private:
		Audio* audio_;
		// 追従対象のワールド座標
		Wepon* wepon_ = nullptr;
		//初期化時の座標
		Vector3 startTranslate_;
		Vector3 worldPos_;
		bool isParent_;
		Vector3 velocity_;
		int HP_;
		bool isAlive_;
		bool isCollision_;
		bool preIsCollision_;
		GameObjectManager* gameObjectManager_ = nullptr;

		int collisionCount_;
		Vector3 hitvelocity_;
		Enemy* enemy_[2];
};

