#include "LockOn.h"
#include "../../../Resource/Texture/TextureManager.h"
#include "../SampleEnemy/SampleEnemy.h"
#include "../SampleThirdPersonCamera/ThirdPersonCamera.h"

void LockOn::Initialize()
{
	// 入力検知
	input_ = Input::GetInstance();

	// モデル読み込み
	AddMesh(&transform_, color_, "./Engine/Resource/Samples/Plane", "Plane.obj", false);
	meshes_[0]->isUI_ = true;
	meshes_[0]->texture_ = TextureManager::Load("./Engine/Resource/Samples/Texture", "Reticle.png");

	// 座標を移動
	transform_.translate_ = { 640.0f, 360.0f };

	// ロックオンしていない
	isLockOn_ = false;

	enemies_.clear();
}

void LockOn::Update()
{
	// 入力取得
	preJoyState_ = joyState_; // 前フレームの入力取得
	input_->GetJoystickState(0, joyState_); // 現在フレームの入力取得

	if (target_ != nullptr && !target_->isDestroy_) {
		meshes_[0]->isActive_ = true;
		// ロックオン対象のワールド座標を取得
		Vector3 positionWorld = target_->transform_.GetWorldPos();
		Matrix4x4 matViewPort = Math::MakeViewportMatrix(0, 0, 1280.0f, 720.0f, 0, 1);
		Matrix4x4 matViewProjectionMat = camera_->GetViewProjectionMatrix() * matViewPort;
		// ワールド座標をスクリーン座標に変換
		transform_.translate_ = Math::Transform(positionWorld, matViewProjectionMat);

		if ((joyState_.Gamepad.bLeftTrigger > 10) &&
			!(preJoyState_.Gamepad.bLeftTrigger > 10)) {
			target_ = nullptr;
		}
		else if (IsOutOfRange()) {
			target_ = nullptr;
		}

		if ((joyState_.Gamepad.wButtons & XINPUT_GAMEPAD_DPAD_LEFT) &&
			!(preJoyState_.Gamepad.wButtons & XINPUT_GAMEPAD_DPAD_LEFT)) {
			if (lockableEnemies_.size() > 1) {
				// ロックオン配列番号インクリメント
				LockOnIndex_++;
				if (LockOnIndex_ >= lockableEnemies_.size()) {
					LockOnIndex_ = 0;
				}
				target_ = lockableEnemies_[LockOnIndex_];
			}
		}
	}
	else {
		meshes_[0]->isActive_ = false;

		if ((joyState_.Gamepad.bLeftTrigger > 10) &&
			!(preJoyState_.Gamepad.bLeftTrigger > 10)) {
			SerchEnemy();
		}
	}
}

void LockOn::DisplayImGui()
{
	transform_.DisplayImGui();
}

void LockOn::SetEnemyList(const std::list<SampleEnemy*>& enemies)
{
	enemies_ = enemies;
}

void LockOn::SerchEnemy()
{
	// ロックオン目標
	std::list<std::pair<float, const SampleEnemy*>> targets_;

	// ロックオン番号リセット
	LockOnIndex_ = 0;
	// ロックオン可能敵リストをクリア
	lockableEnemies_.clear();

	for(const SampleEnemy* enemy : enemies_){
		// ワールド座標の取得
		Vector3 worldPosition = enemy->transform_.GetWorldPos();
		// ワールドからビュー座標に変換する
		Vector3 positionView = Math::Transform(worldPosition, camera_->GetViewMatrix());

		// 距離条件にあっているかチェック
		if (minDisntance_ <= positionView.z && positionView.z <= maxDistance_) {
			// カメラ前方との角度を計算する
			float arcTan = std::atan2(
				std::sqrt(positionView.x * positionView.x + positionView.y * positionView.y), positionView.z);

			// 角度条件チェック
			if (std::abs(arcTan) <= angleRange_) {
				// 条件に当てはまる敵をリストに追加する
				targets_.push_back(std::make_pair(positionView.z, enemy));
			}
		}
	}

	// ロックオン対象をリセット
	target_ = nullptr;
	// 対象が一体でもいれば
	if (targets_.size() > 0) {
		// 距離で昇順にソート
		targets_.sort([](auto& pair1, auto& pair2) {return pair1.first < pair2.first; });
		// ソートの結果一番近い敵をロックオン対象とする
		target_ = targets_.front().second;
		
		for (std::pair<float, const SampleEnemy*>enemy : targets_) {
			lockableEnemies_.push_back(enemy.second);
		}
	}
}

bool LockOn::IsOutOfRange()
{
	// ワールド座標の取得
	Vector3 worldPosition = target_->transform_.GetWorldPos();
	// ワールドからビュー座標に変換する
	Vector3 positionView = Math::Transform(worldPosition, camera_->GetViewMatrix());

	// 距離条件にあっているかチェック
	if (minDisntance_ <= positionView.z && positionView.z <= maxDistance_) {
		// あっていたら範囲外じゃない
		return false;
	}

	// 合ってなかったら範囲外
	return true;
}
