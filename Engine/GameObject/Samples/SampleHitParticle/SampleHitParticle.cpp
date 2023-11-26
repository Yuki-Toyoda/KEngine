#include "SampleHitParticle.h"

void SampleHitParticle::Initialize()
{
	for (int i = 0; i < kMaxParticle; i++) {
		// パーティクルの初期化
		particleTransform_[i].Initialize();
		particleTransform_[i].scale_ = { .25f, .25f, .25f };
		// パーティクルの親を登録
		particleTransform_[i].SetParent(&transform_);
		// tの初期化
		t_[i] = 0.0f;
		// 演出時間初期化
		time_[i] = Math::RandomF(1.5f, 2.5f, 2);

		// 演出座標を取得
		startTranslate_[i] = particleTransform_[i].translate_;
		endTranslate_[i] = {
			Math::RandomF(startTranslate_[i].x - kMaxSpred, startTranslate_->x + kMaxSpred, 2),
			Math::RandomF(startTranslate_[i].y - kMaxSpred, startTranslate_->y + kMaxSpred, 2),
			Math::RandomF(startTranslate_[i].z - kMaxSpred, startTranslate_->z + kMaxSpred, 2)
		};

		// 粒子の終了フラグfalse
		isEnd_[i] = false;

		// モデル読み込み
		AddMesh(&particleTransform_[i], color_, "./Engine/Resource/Samples/Box", "Box.obj", false);
	}
}

void SampleHitParticle::Update()
{
	// 追従対象がある場合はそれに追従
	if (target_ != nullptr) {
		transform_.translate_ = *target_;
	}

	for (int i = 0; i < kMaxParticle; i++) {
		if (t_[i] <= time_[i]) {
			// 線形補間で粒子を移動させる
			particleTransform_[i].translate_ = Math::EaseInOut(t_[i], startTranslate_[i], endTranslate_[i], time_[i]);
			particleTransform_[i].scale_ = Math::EaseOut(t_[i], { .25f, .25f, .25f }, { 0.0f, 0.0f, 0.0f }, time_[i]);

			// t加算
			t_[i] += 1.0f / 60.0f;
		}
		else {

			// 大きさを0で固定
			particleTransform_[i].scale_ = { 0.0f, 0.0f, 0.0f };

			// 終了を伝える
			isEnd_[i] = true;
		}
	}

	for (int i = 0; i < kMaxParticle; i++) {
		// １つでも終了していなければ処理を抜ける
		if (!isEnd_[i])
			break;

		if (i == kMaxParticle - 1) {
			// このパーティクルを破棄
			Destroy();
		}
	}
}

void SampleHitParticle::DisplayImGui()
{
	transform_.DisplayImGui();
}
