#include "FirstStage.h"

void FirstStage::Initialize()
{

}

void FirstStage::Update()
{
	if (input_->TriggerKey(DIK_RCONTROL)) {
		nextStage_ = new SecondeStage();
	}
}
