#include "SampleAnim.h"

void SampleAnim::Init()
{
	// メッシュ追加関数
	AddSkiningModel("SkiningAnim", &transform_, "./Engine/Resource/Samples/TestMultiMeshAnim", "TestMultiMeshAnim.gltf");
	skiningModels_["SkiningAnim"]->animationManager_.PlayAnimation("Action", true);
}

void SampleAnim::Update()
{
}
