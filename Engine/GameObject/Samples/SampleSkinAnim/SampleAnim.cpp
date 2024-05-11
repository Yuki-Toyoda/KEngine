#include "SampleAnim.h"

void SampleAnim::Init()
{
	// メッシュ追加関数
	AddMesh(&transform_, color_, "./Engine/Resource/Samples/Human", "walk.gltf");

	// 拡大率調整
	transform_.scale_ = Vector3(0.01f, 0.01f, 0.01f);
	transform_.rotate_.x = static_cast<float>(std::numbers::pi / 2.0f);

	// アニメーションのループ
	transform_.animations_[0].isLoop = true;
	// 再生
	transform_.animations_[0].isPlay = true;
}

void SampleAnim::Update()
{

}

void SampleAnim::DisplayImGui()
{
	transform_.DisplayImGui();
}
