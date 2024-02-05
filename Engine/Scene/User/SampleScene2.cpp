#include "SampleScene2.h"
#include "../../GameObject/SampleList.h"
#include "../SceneManager.h"

void SampleScene2::Init()
{
	// オブジェクトの生成
	gameObjectManager_->CreateInstance<Camera>("Camera", BaseObject::TagCamera);

	// スカイドームの生成
	SkyDome* sk = gameObjectManager_->CreateInstance<SkyDome>("SkyDome", BaseObject::TagNone);
	sk->transform_.rotate_.y = -(float)std::numbers::pi / 2.0f;

	// 複雑なモデルの描画
	gameObjectManager_->CreateInstance<Rifle>("Rifle", BaseObject::TagNone);

	// 該当シーンのマネージャーを生成
	ObjectSceneManager* osm = gameObjectManager_->CreateInstance<ObjectSceneManager>("ObjectSceneManager", BaseObject::TagNone);
	osm->SetSceneNo(true);

}

void SampleScene2::Update()
{
	// デバッグ遷移
	if (input_->TriggerKey(DIK_RSHIFT)) {
		SceneManager::GetInstance()->ChangeScene("");
	}
}
