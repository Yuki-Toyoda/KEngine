#include "TutrialManager.h"
#include "../../../Resource/Texture/TextureManager.h"
#include "../UIManager/InGameUIManager.h"

void TutrialManager::Init()
{
	Vector2 uiPosition_ = {640.0f,64.0f};
	AddSprite("yokero", uiPosition_ , {768.0f,64.0f}, TextureManager::Load("./Resources/UI/Tutorial", "Tutorial_01.png"));
	sprites_[0]->anchorPoint_ = { 0.5f,0.5f };
	AddSprite("hiroe", uiPosition_, { 832.0f,64.0f }, TextureManager::Load("./Resources/UI/Tutorial", "Tutorial_02.png"));
	sprites_[1]->anchorPoint_ = { 0.5f,0.5f };
	AddSprite("butukare", uiPosition_, { 944.0f,64.0f }, TextureManager::Load("./Resources/UI/Tutorial", "Tutorial_03.png"));
	sprites_[2]->anchorPoint_ = { 0.5f,0.5f };
	AddSprite("onakasuita", uiPosition_, { 1136.0f,80.0f }, TextureManager::Load("./Resources/UI/Tutorial", "Tutorial_04.png"));
	sprites_[3]->anchorPoint_ = { 0.5f,0.5f };
	AddSprite("kaihuku", {648.0f,128.0f}, {832.0f,64.0f}, TextureManager::Load("./Resources/UI/Tutorial", "Tutorial_05.png"));
	sprites_[4]->anchorPoint_ = { 0.5f,0.5f };
	AddSprite("owari", uiPosition_, { 768.0f,64.0f }, TextureManager::Load("./Resources/UI/Tutorial", "Tutorial_08.png"));
	sprites_[5]->anchorPoint_ = { 0.5f,0.5f };
	AddSprite(" plactice", uiPosition_, { 768.0f,64.0f }, TextureManager::Load("./Resources/UI/Tutorial", "Tutorial_09.png"));
	sprites_[6]->anchorPoint_ = { 0.5f,0.5f };
	AddSprite("dash", uiPosition_, { 1072.0f,64.0f }, TextureManager::Load("./Resources/UI/Tutorial", "Tutorial_07.png"));
	sprites_[7]->anchorPoint_ = { 0.5f,0.5f };
	AddSprite("skipGauge", { 0.0f,0.0f }, { 554.0f,51.0f }, TextureManager::Load("./Resources/UI/Tutorial", "tutorialSkipGauge.png"));
	sprites_[8]->anchorPoint_ = { 0.5f,0.5f };
	AddSprite("skipFrame ", { 1000.0f,684.0f }, { 554.0f,51.0f }, TextureManager::Load("./Resources/UI/Tutorial", "tutorialSkipFram.png"));
	sprites_[9]->anchorPoint_ = { 0.5f,0.5f };
	AddSprite("GameOver", uiPosition_, { 894.0f,64.0f }, TextureManager::Load("./Resources/UI/Tutorial", "Ttutorial_06.png"));
	sprites_[10]->anchorPoint_ = { 0.5f,0.5f };
	sprites_[8]->SetParent(sprites_[9]->GetWorldTransform());
	ArroeBottom_ = { 133.0f,500.0f };
	ArrowUpper_ = { 133.0f,460.0f };
	AddSprite("Arrow", ArrowUpper_, { 144.0f,160.0f }, TextureManager::Load("./Resources/UI/Tutorial", "tutorialArrow.png"));
	sprites_[11]->anchorPoint_ = { 0.5f,0.5f };
	AddSprite("Power", {648.0f,128.0f}, {1072.0f,64.0f}, TextureManager::Load("./Resources/UI/Tutorial", "Tutorial_10.png"));
	sprites_[12]->anchorPoint_ = { 0.5f,0.5f };
	AddSprite("PlacticeEnd", { 0.0f,0.0f }, { 752.0f,80.0f }, TextureManager::Load("./Resources/UI/Tutorial", "practiceSkipGauge.png"));
	AddSprite("PlacticeEnd", { 648.0f,128.0f }, { 752.0f,80.0f }, TextureManager::Load("./Resources/UI/Tutorial", "practiceSkipFram.png"));
	sprites_[13]->anchorPoint_ = { 0.5f,0.5f };
	
	sprites_[14]->anchorPoint_ = { 0.5f,0.5f };
	sprites_[13]->SetParent(sprites_[14]->GetWorldTransform());
	step = 0;
	skipCount_ = 0;
	isEnd_ = false;
	stopTimer_.Start(1.0f);
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
	stopTimer_.Update();

	switch (step)
	{

	case yokero:
		if (stopTimer_.GetIsFinish()) {
			boss_->SetTutrialStart(true);
			stopTimer_.Start(300.0f);
		}
	//6.5秒間表示したら集めるチュートリアルへ
		if (timer_.GetIsFinish()) {
			timer_.Start(6.5f);
		}
		timer_.Update();
		sprites_[0]->SetIsActive(true);
		if (timer_.GetIsFinish()) {
			step++;
			stopTimer_.Stop();
			stopTimer_.Start(stopTime_);
		}

	break;
	case atumero:
		if (stopTimer_.GetIsFinish()) {
			boss_->SetTutrialStart(true);
			stopTimer_.Start(300.0f);
		}

		if (arrowTimer_.GetIsFinish()) {
			if (sprites_[11]->translate_.y == ArroeBottom_.y) {
				//矢印が下なら下降フラグをfalse
				arrowIsDown = false;
				arrowGoal_ = ArrowUpper_;
				arrowStart_ = ArroeBottom_;
			}
			else {
				//上ならtrue
				arrowIsDown = true;
				arrowGoal_ = ArroeBottom_;
				arrowStart_ = ArrowUpper_;
			}
			arrowTimer_.Start(1.0f);
		}
		arrowTimer_.Update();
		//五個集めたら回復へ
		sprites_[1]->SetIsActive(true);
		sprites_[11]->SetIsActive(true);
		sprites_[11]->translate_.y = Math::Linear(KLib::EaseInOutQuad(arrowTimer_.GetProgress()), arrowStart_.y, arrowGoal_.y);
		if (player_->GetAbsorptionCount() >= 10) {
			step++;
			//うりぼーの体力を減らす
			uribo_->Tutrial();
			tCamera_->SetIsUseThisCamera(true);
			tCamera_->transform_ = iCamera_->transform_;
			tCamera_->fov_ = iCamera_->fov_;

			// UIマネージャーにセットされているカメラを変更
			inGameUIManager_->SetCamera(tCamera_);

			timer_.Start(1.5f);
			boss_->SetTutrialStop(true);
			stopTimer_.Stop();
			stopTimer_.Start(stopTime_);
		}
		break;
	case cameraMove:
		timer_.Update();
		tCamera_->transform_.translate_ = Math::Linear(KLib::EaseInOutSine( timer_.GetProgress()), iCamera_->transform_.translate_, tCamera_->goalTransform_.translate_);
		
		if (timer_.GetIsFinish()) {
			step++;
			timer_.Start(3.0f);
			uribo_->SetTutrialStart(true);
		}
		break;
	case onakasuita:
		timer_.Update();
		sprites_[3]->SetIsActive(true);
		if (timer_.GetIsFinish()) {
			step++;
			timer_.Start(1.5f);
		}
		break;
	case cameraBack:
		timer_.Update();
		tCamera_->transform_.translate_ = Math::Linear(KLib::EaseInOutSine(timer_.GetProgress()),  tCamera_->goalTransform_.translate_, iCamera_->transform_.translate_ );
		if (timer_.GetIsFinish()) {
			step++;
			iCamera_->SetIsUseThisCamera(true);
			tCamera_->SetIsUseThisCamera(false);

			// UIマネージャーにセットされているカメラを変更
			inGameUIManager_->SetCamera(iCamera_);
			boss_->SetTutrialStart(true);
		}
		break;
	case kaihuku:
		
		//うりぼーの体力が半分以上で攻撃へ
		sprites_[10]->SetIsActive(true);
		sprites_[4]->SetIsActive(true);
		if (uribo_->TutrialEnd()) {
			step++;
			
			player_->SetIsTutrialDash(false);
			player_->SetCanDash(true);
			stopTimer_.Stop();
			stopTimer_.Start(stopTime_);
		}
		break;
	case Dash:
		if (stopTimer_.GetIsFinish()) {
			boss_->SetTutrialStart(true);
			stopTimer_.Start(300.0f);
		}
		sprites_[7]->SetIsActive(true);
		if (player_->GetISTutrialDash()) {
			boss_->SetTutrialAtackEnd(false);
			step++;
			stopTimer_.Stop();
			stopTimer_.Start(stopTime_);
		}
		break;
	case kougeki:
		if (stopTimer_.GetIsFinish()) {
			boss_->SetTutrialStart(true);
			stopTimer_.Start(300.0f);
		}
		//攻撃が終わったらendへ
		sprites_[2]->SetIsActive(true);
		sprites_[12]->SetIsActive(true);

		if (boss_->GetTutrialAtackEnd()) {
			step++;
			stopTimer_.Stop();
			stopTimer_.Start(stopTime_);
		}
		break;
	case end:
		if (stopTimer_.GetIsFinish()) {
			boss_->SetTutrialStart(true);
			stopTimer_.Start(300.0f);
		}
		sprites_[5]->SetIsActive(true);
		if (timer_.GetIsFinish()) {
			timer_.Start(2.0f);
		}
		timer_.Update();
		if (timer_.GetIsFinish()) {
			step++;
			uribo_->SetTutrialPlactice(true);
		}
		break;
	case plactice:
		sprites_[6]->SetIsActive(true);
		sprites_[13]->SetIsActive(true);
		sprites_[14]->SetIsActive(true);
	

		sprites_[13]->scale_.y = Math::Linear((float)skipCount_, 0.0f, 80.0f, 60.0f);
		sprites_[13]->texBase_.y = Math::Linear((float)skipCount_, 80.0f, 0.0f, 60.0f);
		sprites_[13]->texSize_.y = Math::Linear((float)skipCount_, 0.0f, 80.0f, 60.0f);
		sprites_[13]->translate_.y = Math::Linear((float)skipCount_, 40.0f, 0.0f, 60.0f);

		if (sprites_[13]->texSize_.y > 80.0f) {
			sprites_[13]->texSize_.y = 80.0f;
		}

		if (sprites_[13]->texBase_.y < 0.0f) {
			sprites_[13]->texBase_.y = 0.0f;
		}
		// 操作によるカメラ回転を有効に
		iCamera_->SetCanRotate(true);
		
		break;
	default:
		break;
	}
	if (step != plactice) {
	sprites_[8]->SetIsActive(true);
	sprites_[9]->SetIsActive(true);
	sprites_[8]->scale_.y = Math::Linear((float)skipCount_, 0.0f, 51.0f, 60.0f);
	sprites_[8]->texBase_.y = Math::Linear((float)skipCount_, 80.0f, 0.0f, 60.0f);
	sprites_[8]->texSize_.y = Math::Linear((float)skipCount_, 0.0f, 80.0f, 60.0f);
	sprites_[8]->translate_.y = Math::Linear((float)skipCount_, 25.5f, 0.0f, 60.0f);

	if (sprites_[8]->texSize_.y > 80.0f) {
		sprites_[8]->texSize_.y = 80.0f;
	}

	if (sprites_[8]->texBase_.y < 0.0f) {
		sprites_[8]->texBase_.y = 0.0f;
	}
	}
	
	if (uribo_->GetHP() <= 0) {
		uribo_->Reset();
		
	}
}

void TutrialManager::DisplayImGui()
{
	
	for (Sprite* s : sprites_) {
		s->DisplayImGui();
	}
}

void TutrialManager::SetIngameCamera(InGameCamera* camera)
{
	// インゲームカメラをセット
	iCamera_ = camera;
	// 操作によるカメラ回転を無効に
	iCamera_->SetCanRotate(false);
}
