#include "Fairy.h"
#include "../Player/Player.h"

void Fairy::Init()
{
	// メッシュ追加関数
	AddSkiningModel(&transform_, "./Resources/Fairy", "Fairy.gltf");

	// スケール調整
	transform_.scale_ = { 0.25f, 0.25f, 0.25f };

	// アニメーションのループ
	skiningModels_[0]->animationManager_.PlayAnimation("00_Idle", true);

	// オフセットなしのトランスフォームの初期化
	noOffsetTransform_.Init();
}

void Fairy::Update()
{
	// 追従対象の更新
	UpdateTarget();
}

void Fairy::DisplayImGui()
{
	transform_.DisplayImGui();
}

void Fairy::UpdateTarget()
{
	// 追従対象が存在するときのみ
	if (target_) {
		// 遷移後の座標を求める
		interTarget_ = KLib::Lerp<Vector3>(interTarget_, target_->translate_, trackingDelay_);

		// オフセットなしのトランスフォームの値を設定
		noOffsetTransform_.rotate_ = transform_.rotate_;

		// カメラの角度から回転行列を生成
		Matrix4x4 rotateMat = Matrix4x4::MakeRotate(transform_.rotate_);

		// 遷移後の座標を求める
		Vector3 interTargetA = Vector3(0.0f, 0.0f, 0.0f);
		interTargetA = KLib::Lerp<Vector3>(target_->translate_, interTargetA, trackingDelay_);
		noOffsetTransform_.translate_ = KLib::Lerp<Vector3>(target_->translate_, interTargetA, trackingDelay_) + (kOffset_ * rotateMat);
	}

	// Y軸の回転をプレイヤーと同期
	transform_.rotate_.y = player_->transform_.rotate_.y;

	// オフセットの取得
	Vector3 offset = CalcOffset();
	// カメラの座標をオフセットを元に変更
	transform_.translate_ = interTarget_ + offset;
}

Vector3 Fairy::CalcOffset() const
{
	// 追従対象からカメラまでのオフセットを設定
	Vector3 offset = offset_;

	// カメラの角度から回転行列を生成
	Matrix4x4 rotateMat = Matrix4x4::MakeRotate(transform_.rotate_);

	// オフセットをカメラの回転に合わせて回転させる
	offset = offset * rotateMat;

	// 計算したオフセットを返す
	return offset;
}
