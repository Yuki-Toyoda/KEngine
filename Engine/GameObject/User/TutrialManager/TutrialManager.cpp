#include "TutrialManager.h"
#include "../../../Resource/Texture/TextureManager.h"
void TutrialManager::Init()
{
	AddSprite("yokero", { 0.0f,0.0f }, { 768.0f,64.0f }, TextureManager::Load("./Resources/UI/Tutorial", "Tutorial_01.png"));
	sprites_[0]->anchorPoint_ = { 0.5f,0.5f };
	AddSprite("hiroe", { 0.0f,0.0f }, { 768.0f,64.0f }, TextureManager::Load("./Resources/UI/Tutorial", "Tutorial_02.png"));
	sprites_[1]->anchorPoint_ = { 0.5f,0.5f };
	AddSprite("butukare", { 0.0f,0.0f }, { 768.0f,64.0f }, TextureManager::Load("./Resources/UI/Tutorial", "Tutorial_03.png"));
	sprites_[2]->anchorPoint_ = { 0.5f,0.5f };
	AddSprite("onakasuita", { 0.0f,0.0f }, { 768.0f,64.0f }, TextureManager::Load("./Resources/UI/Tutorial", "Tutorial_04.png"));
	sprites_[3]->anchorPoint_ = { 0.5f,0.5f };
	AddSprite("kaihuku", { 0.0f,0.0f }, { 768.0f,64.0f }, TextureManager::Load("./Resources/UI/Tutorial", "Tutorial_05.png"));
	sprites_[4]->anchorPoint_ = { 0.5f,0.5f };
	step = tutrial1;
}

void TutrialManager::Update()
{
	
	for (Sprite* sprite : sprites_) {
		sprite->SetIsActive(false);
	}
	sprites_[step]->SetIsActive(true);


}

void TutrialManager::DisplayImGui()
{
}
