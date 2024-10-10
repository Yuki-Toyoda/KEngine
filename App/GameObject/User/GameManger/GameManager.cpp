#include "GameManager.h"

void GameManager::Init()
{
	// フェード演出用スプライト
	AddSprite("Fade", {
		(float)KEngine::Config::Window::KWindowWidth / 2.0f,
		(float)KEngine::Config::Window::KWindowHeight / 2.0f, },
		{ (float)KEngine::Config::Window::KWindowWidth,
		(float)KEngine::Config::Window::KWindowHeight },
		TextureManager::Load("white2x2.png"));
	// アンカーポイントの設定
	sprites_[0]->anchorPoint_ = { .5f, .5f };
	// 色、透明度設定
	sprites_[0]->color_ = { 0.0f, 0.0f, 0.0f, 1.0f };
}

void GameManager::Update()
{
}

void GameManager::DisplayImGui()
{
}
