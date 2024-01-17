#include "EnemySampleState.h"
#include "../../IEnemy.h"

void EnemySampleState::Init()
{
	// 行動名称設定
	name_ = "Sample";
	
	// 開始地点の設定
	start_ = enemy_->transform_.translate_;
	// 終端地点の設定
	end_ = start_;

	// 線形補間用タイマーを動かす
	lerpTimer_.Start(lerpTime_);
}

void EnemySampleState::Update()
{
	// 敵を線形補間に酔って移動させる
	enemy_->transform_.translate_ = KLib::Lerp(start_, end_, KLib::EaseInOutQuad(lerpTimer_.GetProgress()));

	// 線形補間タイマー更新
	lerpTimer_.Update();
}

void EnemySampleState::DisplayImGui()
{
	// ImGuiの表示
	ImGui::DragFloat("LerpTime", &lerpTime_, 0.01f, 10.0f);
	ImGui::DragFloat3("Start", &start_.x);
	ImGui::DragFloat3("End", &end_.x);

	// ボタンによって行動リセット
	if (ImGui::Button("Restart")) {
		// 線形補間用タイマーを動かす
		lerpTimer_.Start(lerpTime_);
	}
}
