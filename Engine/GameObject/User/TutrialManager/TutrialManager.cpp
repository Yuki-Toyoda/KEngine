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
	AddSprite("owari", uiPosition_, { 768.0f,64.0f }, TextureManager::Load("./Resources/UI/Tutorial", "Tutorial_08.png"));
	sprites_[5]->anchorPoint_ = { 0.5f,0.5f };
	AddSprite(" plactice", uiPosition_, { 768.0f,64.0f }, TextureManager::Load("./Resources/UI/Tutorial", "Tutorial_09.png"));
	sprites_[6]->anchorPoint_ = { 0.5f,0.5f };
	AddSprite("dash", uiPosition_, { 768.0f,64.0f }, TextureManager::Load("./Resources/UI/Tutorial", "Tutorial_07.png"));
	sprites_[7]->anchorPoint_ = { 0.5f,0.5f };
	AddSprite("skipGauge", { 0.0f,0.0f }, { 554.0f,51.0f }, TextureManager::Load("./Resources/UI/Tutorial", "tutorialSkipGauge.png"));
	sprites_[8]->anchorPoint_ = { 0.5f,0.5f };
	AddSprite("skipFrame ", { 1000.0f,684.0f }, { 554.0f,51.0f }, TextureManager::Load("./Resources/UI/Tutorial", "tutorialSkipFram.png"));
	sprites_[9]->anchorPoint_ = { 0.5f,0.5f };
	
	sprites_[8]->SetParent(sprites_[9]->GetWorldTransform());
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
	if (skipCount_ >= 60) {
		isEnd_ = true;
	}
	//全てのスプライトを非表示
	for (Sprite* sprite : sprites_) {
		sprite->SetIsActive(false);
	}
	sprites_[8]->SetIsActive(true);
	sprites_[9]->SetIsActive(true);
	sprites_[8]->scale_.y = Math::Linear((float)skipCount_,0.0f,51.0f,60.0f);
	sprites_[8]->translate_.y = Math::Linear((float)skipCount_, 25.5f, 0.0f, 60.0f);
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
		if (player_->GetAbsorptionCount() >= 10) {
			step++;
			//うりぼーの体力を減らす
			uribo_->Tutrial();
		}
		break;
	case kaihuku:
		//うりぼーの体力が半分以上で攻撃へ
		sprites_[3]->SetIsActive(true);
		sprites_[4]->SetIsActive(true);
		if (uribo_->TutrialEnd()) {
			step++;
			boss_->SetTutrialAtackEnd(false);
			player_->SetIsTutrialDash(false);
		}
		break;
	case Dash:
		sprites_[7]->SetIsActive(true);
		if (player_->GetISTutrialDash()) {
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
		sprites_[5]->SetIsActive(true);
		if (timer_.GetIsFinish()) {
			timer_.Start(2.0f);
		}
		timer_.Update();
		if (timer_.GetIsFinish()) {
			step++;
		}
		break;
	case plactice:
		sprites_[6]->SetIsActive(true);
		sprites_[9]->translate_ = { 648.0f,128.0f };
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
