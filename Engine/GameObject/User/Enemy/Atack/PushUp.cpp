#include "PushUp.h"


void PushUp::Init()
{
	AddMesh(&transform_, color_, "./Engine/Resource/Samples/Box", "Box.obj");
	color_ = { 1.0f,0.0f,0.0f,1.0f };
	AddColliderOBB("pushUp", &transform_.scale_, &transform_.rotate_, &transform_.translate_);
	//座標は一旦適当
	transform_.scale_.y = 4.0f;
	//transform_.translate_.x = 10.0f;
	//通常時は少しだけ見えるように
	defaultPosition_ = -transform_.scale_.y + 1.0f;
	//最初はdefaultから
	transform_.translate_.y = defaultPosition_;
	
	//下にある時は見えないように
	rawPosition_ = transform_.translate_.y - transform_.scale_.y - 1.0f;
	//上にある時は全て見えるように
	upPosition_ = transform_.translate_.y + (transform_.scale_.y * 2.0f);
	isRotate_ = false;
	isAtack_ = false;
	isAtackEnd_ = false;
	isRemain_ = false;
}

void PushUp::Update()
{
	colliders_.front()->SetIsActive(false);
	if (isRotate_) {
		PreparationAtack();
	}
	else if (isAtack_) {
		Atack();
	}
	else if (isRemain_) {
		AtackRemain();
	}
	else if (isAtackEnd_) {
		AtackEnd();
	}
}

void PushUp::DisplayImGui()
{
	transform_.DisplayImGui();
	defaultPosition_ = -transform_.scale_.y + 1.0f;
	if (ImGui::Button("SetPosition")) {
		transform_.translate_.y = defaultPosition_;
	}
}

void PushUp::Reset()
{
	transform_.translate_.y = defaultPosition_;
	transform_.rotate_.x = 0.0f;
	isRotate_ = false;
	isAtack_ = false;
	isAtackEnd_ = false;
	isRemain_ = false;
	isActive_ = true;
}

void PushUp::PreparationAtack()
{

	if (timer_.GetIsFinish()) {
		timer_.Start(RotateTime_);

	}
	timer_.Update();
	//回転
	transform_.rotate_.x= KLib::Lerp<float>(0.0f, 3.141592f, timer_.GetProgress());
	transform_.translate_.y = KLib::Lerp<float>(defaultPosition_, rawPosition_, timer_.GetProgress());
	//タイマーが終了してたら攻撃開始
	if (timer_.GetIsFinish()) {
		isRotate_ = false;
		isAtack_ = true;
	}
}

void PushUp::Atack()
{
	colliders_.front()->SetIsActive(true);
	if (timer_.GetIsFinish()) {
		timer_.Start(AtackTime_);

	}
	timer_.Update();
	transform_.translate_.y = KLib::Lerp<float>(rawPosition_, upPosition_, timer_.GetProgress());
	//タイマーが終了してたら攻撃中二変更
	if (timer_.GetIsFinish()) {
		isAtack_ = false;
		isRemain_ = true;
	}
}

void PushUp::AtackRemain()
{
	colliders_.front()->SetIsActive(true);
	if (timer_.GetIsFinish()) {
		timer_.Start(remainTime_);

	}
	timer_.Update();

	//タイマーが終了してたら破壊する
	if (timer_.GetIsFinish()) {
		isRemain_ = false;
		isAtackEnd_ = true;
		isActive_ = false;
		


	}
}

void PushUp::AtackEnd()
{
	colliders_.front()->SetIsActive(true);
	if (timer_.GetIsFinish()) {
		timer_.Start(AtackTime_);

	}
	timer_.Update();
	transform_.translate_.y = KLib::Lerp<float>(upPosition_, rawPosition_, timer_.GetProgress());
	//タイマーが終了してたら攻撃開始
	if (timer_.GetIsFinish()) {
		Reset();
	}
}

void PushUp::SetActive()
{
	Reset();
	isRotate_ = true;
	
}
