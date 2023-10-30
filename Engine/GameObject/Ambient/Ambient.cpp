#include "Ambient.h"

void Ambient::Initialize(std::string name, Tag tag)
{
	// 基底クラス初期化
	BaseObject::Initialize(name, tag);
	isActive_ = true;

	// モデル読み込み
	AddOBJ(&transform_, color_, "./Engine/Resource/Samples/SkyDome", "SkyDome.obj",false);

	// グローバル変数に調整したい値を追加
	AddGlobalVariables();
}

void Ambient::Update()
{
	// 基底クラス更新
	BaseObject::Update();
}

void Ambient::Draw()
{
	DrawAllOBJ();
}

void Ambient::AddGlobalVariables()
{
}

void Ambient::ApplyGlobalVariables()
{
}
