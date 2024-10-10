#include "EnemyDead.h"
#include "App/GameObject/User/Enemy/Enemy.h"
#include "App/GameObject/User/Player/Player.h"
#include "App/GameObject/User/GameManger/GameManager.h"
#include "Engine/GameObject/GameObjectManager.h"
#include "Engine/GameObject/Core/Camera.h"

void EnemyDead::Init()
{
	// 行動名設定
	stateName_ = "Dead";

	// ループを切れる
	enemy_->enemyAnim_->isLoop_ = false;
	// 敵のアニメーションを変更
	enemy_->enemyAnim_->ChangeParameter("Enemy_Dead", true);
	enemy_->transform_.rotate_.y = 0.037f;

	// プレイヤー強制移動
	Player* p = GameObjectManager::GetInstance()->GetGameObject<Player>("Player");
	p->transform_.translate_ = { -0.544f, 0.0f, -4.65f };
	p->transform_.rotate_.y = 0.28f;
	p->canAction_ = false;
	
	// 待機アニメーション再生
	p->skiningModels_["Player"]->animationManager_.PlayAnimation("00_Idle", 0.0f, true);

	// カメラ移動
	camera_ = GameObjectManager::GetInstance()->CreateInstance<Camera>("StagingCamera", IObject::TagCamera);
	camera_->transform_.translate_ = { -2.777f, 0.3f, -6.4f };
	camera_->transform_.rotate_ = { -0.285f, 0.55f, 0.0f };
	camera_->fov_ = 0.6f;
	camera_->UseThisCamera();
	camera_->ppProcessor_.bloom_.threshold_ = 0.25f;
}

void EnemyDead::Update()
{
	// 全通常モデルを透明に
	for (std::map<std::string, NormalModel*>::const_iterator it = enemy_->normalModels_.cbegin(); it != enemy_->normalModels_.cend(); ++it) {
		// マテリアルに色を適用
		it->second->materials_[1].dissolveStrength_ = enemy_->GetColor().w;
		it->second->materials_[1].dissolveEdgeColor_ = { 0.0f, 1.0f, 0.75f };
		it->second->materials_[1].dissolveEdgeThreshold_ = 0.025f;
	}

	// アニメーションが終了していたら
	if (enemy_->enemyAnim_->isEnd_) {
		// フェードアウト演出が行われていない場合
		if (!isFadeOut_) {
			// フェードアウト開始
			enemy_->gameManager_->StartFade(GameManager::FADEOUT, 1.5f);
			// フェードアウト開始済み
			isFadeOut_ = true;
		}
		else {
			// フェード演出中で無ければ
			if (!enemy_->gameManager_->GetIsFadeStaging()) {
				// ゲーム終了
				enemy_->gameManager_->SetIsGameEnd(true);
			}
		}
	}
}

void EnemyDead::DisplayImGui()
{
}
