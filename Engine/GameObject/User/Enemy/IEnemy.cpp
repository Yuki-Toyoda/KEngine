#include "IEnemy.h"

void IEnemy::PreInit()
{
	// インスタンス取得
	audio_ = Audio::GetInstance();

	// 武器とのペアレントフラグfalse
	isParent_ = false;

	// ワールド座標を取得
	worldPos_ = transform_.GetWorldPos();

	/// 各トランスフォームの初期化
	// 身体
	bodyTransform_.Init();
	bodyTransform_.SetParent(&transform_);
	// 左羽
	wingTransform_L_.Init();
	wingTransform_L_.SetParent(&transform_);
	wingTransform_L_.translate_ = { 0.35f, 0.0f, 0.75f };
	// 右羽
	wingTransform_R_.Init();
	wingTransform_R_.SetParent(&transform_);
	wingTransform_R_.translate_ = { -0.35f, 0.0f, 0.75f };

	// メッシュの追加
	AddMesh(&bodyTransform_, color_, "./Resources/Enemy", "Body.obj");
	AddMesh(&wingTransform_L_, color_, "./Resources/Enemy", "Wing_L.obj");
	AddMesh(&wingTransform_R_, color_, "./Resources/Enemy", "Wing_R.obj");
}
