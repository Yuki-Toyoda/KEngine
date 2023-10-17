#include "TitleObjects.h"

void TitleObjects::Initialize(std::string name, Tag tag)
{
	// 基底クラス初期化
	BaseObject::Initialize(name, tag);
	isActive_ = true;

	// モデル読み込み
	AddOBJ(&transform_, color_, "./Resources/TitleGear", "TitleGear.obj");
	AddOBJ(&transform_, color_, "./Resources/Gear", "Gear_L.obj");
	AddOBJ(&transform_, color_, "./Resources/Gear", "Gear_M.obj");
	AddOBJ(&transform_, color_, "./Resources/Gear", "Gear_S.obj");
}

void TitleObjects::Update()
{
	
}

void TitleObjects::Draw()
{
}
