#include "WaitTimeState.h"
#include "../IEnemy.h"
#include "../../Random/RandomEngine.h"
void WaitTimeState::Init()
{
	name_ = "Wait";
	waitTimer_.Start(enemy_->GetWaitTime());
}

void WaitTimeState::Update()
{
	waitTimer_.Update();
	//タイマーが一定以上且つリストの最後じゃなければ
	if (waitTimer_.GetIsFinish() && enemy_->StateNumber_ < enemy_->stateList_.state_.at(enemy_->patternNumber_).size() - 1) {
		enemy_->StateNumber_++;
		//リスト内の次の行動状態に移行
		enemy_->ChangeState(enemy_->stateList_.state_.at(enemy_->patternNumber_).at(enemy_->StateNumber_)->name_);
		return;
	}
	if (enemy_->StateNumber_ == enemy_->stateList_.state_.at(enemy_->patternNumber_).size() - 1) {
		//リストが最後まで終わったら最初に戻す
		enemy_->StateNumber_ = 0;
		//enemy_->patternNumber_ = (int)RandomEngine::GetRandom(0.0f, (float)enemy_->stateList_.state_.size() );
		enemy_->ChangeState(enemy_->stateList_.state_.at(enemy_->patternNumber_).at(enemy_->StateNumber_)->name_);
		return;
	}
}

void WaitTimeState::DisplayImGui()
{
}
