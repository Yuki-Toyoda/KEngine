#include "LockOn.h"
#include "../FollowCamera/FollowCamera.h"
#include "../Enemy/Enemy.h"
#include "../../GameObjectManager.h"

void LockOn::Init()
{
	// ロックオントリガーfalse
	isLockOn_ = false;

	// 敵リストクリア
	enemies_.clear();

	target_ = nullptr;
}

void LockOn::Update()
{
	// 敵をすべて取得する
	enemies_ = GameObjectManager::GetInstance()->GetGameObjectList<Enemy>();

	// ロックオン対象がいる、かつ対象が破壊されていないとき
	if (target_ != nullptr && !target_->GetIsDestroy()) {	
		// ロックオン対象が範囲外だった場合、ロックオンを外す
		if (IsOutOfRange()) {
			// ロックオン解除
			DisableLockOn();
		}
	}
}

void LockOn::DisplayImGui()
{

}

bool LockOn::EnableLockOn()
{
	// ターゲットがないときのみ
	if (target_ == nullptr) {
		// ロックオン対象検索、およびロックオン
		SerchEnemy();
	}

	// ロックオンが有効になったか
	return isLockOn_;
}

void LockOn::DisableLockOn()
{
	// ロックオン対象を削除
	target_ = nullptr;
	// ロックオン中でない
	isLockOn_ = false;
}

void LockOn::SerchEnemy()
{
	// ロックオン目標
	std::list<std::pair<float, const Enemy*>> targets_;

	for (const Enemy* enemy : enemies_) {
		// ワールド座標の取得
		Vector3 worldPosition = enemy->transform_.GetWorldPos();
		// ワールドからビュー座標に変換する
		Vector3 positionView = Math::Transform(worldPosition, camera_->GetViewMatrixNoOffset());

		// 距離条件にあっているかチェック
		if (minDistance_ <= positionView.z && positionView.z <= maxDistance_) {
			Vector3 viewXZ = Math::Normalize(Vector3{ positionView.x,0.0f,positionView.z });
			Vector3 viewZ = Math::Normalize(Vector3{ 0.0f,0.0f,positionView.z });
			float cos = Math::Length(Math::Cross(viewXZ, viewZ));
			
			// 角度条件チェック
			if (std::abs(cos) <= angleRange_) {
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
		isLockOn_ = true;

		/*for (std::pair<float, const Enemy*>enemy : targets_) {
			lockableEnemies_.push_back(enemy.second);
		}*/
	}
	else {
		isLockOn_ = false;
	}
}

bool LockOn::IsOutOfRange()
{
	// ワールド座標の取得
	Vector3 worldPosition = target_->transform_.GetWorldPos();
	// ワールドからビュー座標に変換する
	Vector3 positionView = Math::Transform(worldPosition, camera_->GetViewMatrixNoOffset());

	// 距離条件にあっているかチェック
	if (minDistance_ <= positionView.z && positionView.z <= maxDistance_) {
		// あっていたら範囲外じゃない
		return false;
	}

	// 合ってなかったら範囲外
	return true;
}
