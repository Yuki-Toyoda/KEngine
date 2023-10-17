#include "SecondStage.h"

void SecondeStage::Initialize() {

}

void SecondeStage::Update() {
	if (input_->TriggerKey(DIK_RCONTROL)) {
		nextStage_ = new FirstStage();
	}
}