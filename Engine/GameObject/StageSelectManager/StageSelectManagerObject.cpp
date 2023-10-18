#include "StageSelectManagerObject.h"

void StageSelectManagerObject::Initialize(std::string name, Tag tag)
{
	// 基底クラス初期化
	BaseObject::Initialize(name, tag);
	isActive_ = true;
}

void StageSelectManagerObject::Update()
{

	// 基底クラス更新
	BaseObject::Update();

}

void StageSelectManagerObject::Draw()
{
	// 全てのobjを描画
	DrawAllOBJ();
}

void StageSelectManagerObject::SpriteDraw()
{

}

void StageSelectManagerObject::AddGlobalVariables()
{

}

void StageSelectManagerObject::ApplyGlobalVariables()
{

}
