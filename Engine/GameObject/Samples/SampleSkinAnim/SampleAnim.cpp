#include "SampleAnim.h"

void SampleAnim::Init()
{
	// メッシュ追加関数
	AddSkiningModel(&transform_, "./Engine/Resource/Samples/TestMultiMeshAnim", "TestMultiMeshAnim.gltf");
	skiningModels_[0]->animationManager_.PlayAnimation("Action", true);
}

void SampleAnim::Update()
{
}
