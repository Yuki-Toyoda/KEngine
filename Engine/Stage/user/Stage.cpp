#include "Stage.h"
#include "../StageManager.h"
#include "../../GameObject/Player/Player.h"

Stage::Stage()
{
}

void Stage::Initialize(const StageInfo& info)
{
	stageInfo_ = info;
	ApplyStageInfo();
}

void Stage::Update()
{
	float gearRotate{ 0 };
	gearRotate += player_->GetGearRotateSpeed();
	if (gearRotate >= player_->GetAddGearSpeedLimit() || gearRotate <= -player_->GetAddGearSpeedLimit())
		if (isAddGearCondition_)
			gearCondition_ += 0 < gearRotate ? gearRotate : gearRotate * -1;
}
