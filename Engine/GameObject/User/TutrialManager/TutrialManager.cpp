#include "TutrialManager.h"
#include "../../../Resource/Texture/TextureManager.h"
void TutrialManager::Init()
{
	Vector2 uiPosition_ = {640.0f,64.0f};
	AddSprite("yokero", uiPosition_ , {768.0f,64.0f}, TextureManager::Load("./Resources/UI/Tutorial", "Tutorial_01.png"));
	sprites_[0]->anchorPoint_ = { 0.5f,0.5f };
	AddSprite("hiroe", uiPosition_, { 768.0f,64.0f }, TextureManager::Load("./Resources/UI/Tutorial", "Tutorial_02.png"));
	sprites_[1]->anchorPoint_ = { 0.5f,0.5f };
	AddSprite("butukare", uiPosition_, { 768.0f,64.0f }, TextureManager::Load("./Resources/UI/Tutorial", "Tutorial_03.png"));
	sprites_[2]->anchorPoint_ = { 0.5f,0.5f };
	AddSprite("onakasuita", uiPosition_, { 768.0f,64.0f }, TextureManager::Load("./Resources/UI/Tutorial", "Tutorial_04.png"));
	sprites_[3]->anchorPoint_ = { 0.5f,0.5f };
	AddSprite("kaihuku", {648.0f,128.0f}, {768.0f,64.0f}, TextureManager::Load("./Resources/UI/Tutorial", "Tutorial_05.png"));
	sprites_[4]->anchorPoint_ = { 0.5f,0.5f };
	step = yokero;
	skipCount_ = 0;
	isEnd_ = false;
}

void TutrialManager::Update()
{
	//ボタンを押してたらカウントを加算
	if (InputManager::TutrialSkip()) {
		skipCount_++;
	}
	else {
		skipCount_ = 0;
	}
	//一定以上でスキップ
	if (skipCount_ >= 120) {
		isEnd_ = true;
	}
	//全てのスプライトを非表示
	for (Sprite* sprite : sprites_) {
		sprite->SetIsActive(false);
	}
	switch (step)
	{

	case yokero:
	//6.5秒間表示したら集めるチュートリアルへ
		if (timer_.GetIsFinish()) {
			timer_.Start(6.5f);
		}
		timer_.Update();
		sprites_[0]->SetIsActive(true);
		if (timer_.GetIsFinish()) {
			step++;
		}
	break;
	case atumero:
		//五個集めたら回復へ
		sprites_[1]->SetIsActive(true);
		if (player_->GetAbsorptionCount() >= 5) {
			step++;
			uribo_->Tutrial();
		}
		break;
	case kaihuku:
		//うりぼーの体力が半分以上で攻撃へ
		sprites_[3]->SetIsActive(true);
		sprites_[4]->SetIsActive(true);
		if (uribo_->TutrialEnd()) {
			step++;
		}
		break;
	case kougeki:
		//攻撃が終わったらendへ
		sprites_[2]->SetIsActive(true);
		if (boss_->GetTutrialAtackEnd()) {
			step++;
		}
		break;
	case end:

		break;
	default:
		break;
	}

}

void TutrialManager::DisplayImGui()
{
	
	for (Sprite* s : sprites_) {
		s->DisplayImGui();
	}
}
