#include "Stage.h"
#include "../StageManager.h"

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
	
}
