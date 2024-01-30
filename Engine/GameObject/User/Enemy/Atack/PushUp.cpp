#include "PushUp.h"


void PushUp::Init()
{
	// モデル用トランスフォームの初期化
	modelTransform_.Init();
	// モデル座標を大元のトランスフォームに追従させる
	modelTransform_.SetParent(&transform_, 0b011);
	// モデルのスケールを変更する
	modelTransform_.scale_ = { 2.0f, 2.0f, 2.0f };

	// メッシュ追加
	//AddMesh(&transform_, color_, "./Engine/Resource/Samples/Box", "Box.obj");
	color_ = { 1.0f,0.0f,0.0f,1.0f };
	
	// モデル追加
	AddMesh(&modelTransform_, modelColor_, "./Resources/Carrot", "Carrot.obj");

	// AABBのコライダー追加
	AddColliderAABB("pushUp", &transform_.translate_, &transform_.scale_);
	
	// スケールを調整する
	transform_.scale_.y = 4.0f;
	
	//通常時は少しだけ見えるように
	defaultPosition_ = -transform_.scale_.y + 2.5f;
	//最初はdefaultから
	transform_.translate_.y = defaultPosition_;
	
	//下にある時は見えないように
	rawPosition_ = transform_.translate_.y - transform_.scale_.y - 1.0f;
	//上にある時は全て見えるように
	upPosition_ = transform_.translate_.y + (transform_.scale_.y * 1.15f);

	// コライダーを無効化する
	colliders_.front()->SetIsActive(false);
}

void PushUp::Update()
{
	// 座標、スケールに合わせてモデルのスケールを変更する
	modelTransform_.scale_ = { transform_.scale_.x, transform_.scale_.y / 2.0f, transform_.scale_.z };

	switch (step_)
	{
	case PushUp::kPreAttack:
		// 攻撃前処理
		PreAttack();
		break;
	case PushUp::kAttacking:
		// 攻撃中処理
		Atacking();
		break;
	case PushUp::kPostAttack:
		// 攻撃後処理
		PostAttack();
		break;
	case PushUp::kAttackEnd:
		// 攻撃終了処理
		AtackEnd();
		break;
	}

	// タイマー更新
	timer_.Update();
}

void PushUp::DisplayImGui()
{
	transform_.DisplayImGui();
	defaultPosition_ = -transform_.scale_.y + 2.5f;
	if (ImGui::Button("SetPosition")) {
		transform_.translate_.y = defaultPosition_;
	}
}

void PushUp::Reset()
{
	// 中心座標をデフォルト座標に合わせる
	transform_.translate_.y = defaultPosition_;
	// モデルの回転角を元に戻す
	modelTransform_.rotate_.x = 0.0f;
	
	// 攻撃段階リセット
	step_ = kNone;

	// 表示させる
	isActive_ = true;

	// 攻撃トリガーをリセット
	isAttack_ = false;

	// コライダーを無効化する
	colliders_.front()->SetIsActive(false);
}

void PushUp::PreAttack()
{
	// 攻撃準備のために地面の下へ
	transform_.translate_.y = KLib::Lerp<float>(defaultPosition_, rawPosition_, KLib::EaseInQuad(timer_.GetProgress()));
	//タイマーが終了、かつ攻撃トリガーがtrueだったら攻撃開始
	if (timer_.GetIsFinish() && isAttack_) {

		// モデルを回転させる
		modelTransform_.rotate_.x = (float)std::numbers::pi;

		// タイマーを開始させる
		timer_.Start(attackTime_);

		// コライダーを有効にする
		colliders_.front()->SetIsActive(true);

		// 次のステップへ
		step_++;
	}
}

void PushUp::Atacking()
{
	// 線形補間で地面から突き出させる
	transform_.translate_.y = KLib::Lerp<float>(rawPosition_, upPosition_, KLib::EaseOutBack(timer_.GetProgress()));
	//タイマーが終了してたら攻撃中に変更
	if (timer_.GetIsFinish()) {
		// タイマーを開始させる
		timer_.Start(remainTime_);

		// 次のステップへ
		step_++;
	}
}

void PushUp::PostAttack()
{
	//タイマーが終了してたら非表示にする
	if (timer_.GetIsFinish()) {
		// タイマーを開始させる
		timer_.Start(attackTime_);

		// 次のステップへ
		step_++;
	}
}

void PushUp::AtackEnd()
{
	// 攻撃後に地面に潜りこませる
	transform_.translate_.y = KLib::Lerp<float>(upPosition_, rawPosition_, KLib::EaseInOutBack(timer_.GetProgress()));
	
	//タイマーが終了してたら攻撃終了
	if (timer_.GetIsFinish()) {
		// 終了トリガー
		Destroy();
	}
}

void PushUp::SetActive()
{
	// リセットを行う
	Reset();

	// 攻撃前処理を行う
	step_ = kPreAttack;

	// タイマーを開始
	timer_.Start(preAttackTime_);
}
