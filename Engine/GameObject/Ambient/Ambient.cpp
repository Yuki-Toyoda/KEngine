#include "Ambient.h"

void Ambient::Initialize(std::string name, Tag tag)
{
	// 基底クラス初期化
	BaseObject::Initialize(name, tag);
	isActive_ = true;

	// モデル読み込み
	objects_.push_back(OBJ::Create(&transform_, { 1.0f, 1.0f, 1.0f, 1.0f }, "./Resources", "SkyDome.obj"));

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
	// オブジェクトの描画
	for (OBJ* obj : objects_) {
		if (isActive_) {
			obj->Draw();
		}
	}
}

void Ambient::AddGlobalVariables()
{
}

void Ambient::ApplyGlobalVariables()
{
}
