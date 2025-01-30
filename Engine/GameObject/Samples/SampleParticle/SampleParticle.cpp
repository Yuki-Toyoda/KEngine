#include "SampleParticle.h"
#include "../../../Particle/CPU/CPUParticleEmitterManager.h"

void SampleParticle::Init()
{
	
}

void SampleParticle::Update()
{
	
}

void SampleParticle::DisplayImGui()
{
	if (ImGui::Button("GenerateParticle")) {
		/*CPUParticleEmitterManager::GetInstance()->CreateEmitter<ICPUParticleEmitter, ICPUParticle>(
			"SampleParticle", 50, 2,
			transform_.translate_, 5.0f, 0.5f,
			modelManager_->CreateParticleModel(modelDataManager_->GetModelData("./Engine/Resource/System/Box", "Box.obj"), 50));*/
	};
}
