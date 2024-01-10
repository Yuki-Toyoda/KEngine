#include "SampleSprite.h"
#include "../../../Resource/Texture/TextureManager.h"

void SampleSprite::Init()
{
	/*=======================================================
		初期化です。メッシュ読み込み、当たり判定の初期化等はここで。
	=========================================================*/

	/// ~スプライトの追加~
	// スプライトを表示したい場合、下記のAddSprite関数を呼び出してください
	// 引数については関数の定義から参照してください
	AddSprite("TestSprite", {0.0f, 0.0f}, {512.0f, 512.0f}, TextureManager::Load("./Engine/Resource/Samples/Box", "uvChecker.png"));
}

void SampleSprite::Update()
{
	/*=======================================================
		更新処理です。毎フレーム行いたい処理はここに記述してください。
	=========================================================*/
}

void SampleSprite::DisplayImGui()
{
	//　スプライトのImGuiの表示
	sprites_[0]->DisplayImGui();
}
