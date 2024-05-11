#include "Fairy.h"

void Fairy::Init()
{
	// メッシュ追加関数
	AddMesh(&transform_, color_, "./Resources/Fairy", "Fairy.gltf");

	// アニメーションのループ
	transform_.animations_[0].isLoop = true;
	// 再生
	transform_.animations_[0].isPlay = true;
}

void Fairy::Update()
{

}

void Fairy::DisplayImGui()
{
	transform_.DisplayImGui();
}
