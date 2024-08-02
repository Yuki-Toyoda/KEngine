#include "SampleParticle.h"
#include "../../../Particle/CPU/ParticleEmitterManager.h"

void SampleParticle::Init()
{
	
}

void SampleParticle::Update()
{
	
}

void SampleParticle::DisplayImGui()
{
	if (ImGui::Button("GenerateParticle")) {
		ParticleEmitterManager::GetInstance()->CreateEmitter<IParticleEmitter, IParticle>(
			"SampleParticle", 50, 2, 
			transform_.translate_, 5.0f, 0.5f, 
			modelManager_->CreateParticleModel(modelDataManager_->GetModelData("./Engine/Resource/Samples/Box", "Box.obj"), 50));
	}
}
